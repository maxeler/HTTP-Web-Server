#define _GNU_SOURCE
#define CEILING_POS(X) ((X-(uint64_t)(X)) > 0 ? (uint64_t)(X+1) : (uint64_t)(X));

#include <string.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <ctype.h>
#include <math.h>

#include "httpserver.h"
#include "docopt.c"
#include "initCode.h"
#include "initCode.c"


void generate_input_data(int size, uint64_t *in_data)
{
	srand(time(0));
	for (int i = 0; i < size; i++) {
		in_data[i] = i;
	}
}

typedef struct tx_event_type_s {
	uint16_t socket_id :16;
	uint64_t frame_count;
	uint64_t reserved :48;
}__attribute__ ((__packed__)) tx_event_type_t;

void write_data_to_lmem(uint64_t *data_in, int size, int size_bytes, int burst_length_in_bytes, max_engine_t *engine, max_file_t *maxfile)
{
	printf("size=%d, sizeBytes=%d, burstLengthInBytes=%d\n", size, size_bytes, burst_length_in_bytes);

	printf("Performing max_actions_init()\n");
	max_actions_t *actions = max_actions_init(maxfile, NULL);
	printf("Done\n");

	max_set_ticks(actions, "KernelLMem_Write_CommandAndDataStream", size);
	max_set_uint64t(actions, "KernelLMem_Write_CommandAndDataStream", "totalBursts", size * 8 / burst_length_in_bytes);
	max_set_uint64t(actions, "KernelLMem_Write_CommandAndDataStream", "wordsPerBurst", burst_length_in_bytes / 8);
	max_set_ticks(actions, "KernelLMem_Read_CommandAndDataStream", 0);
	max_set_uint64t(actions, "KernelLMem_Read_CommandAndDataStream", "totalBursts", size * 8 / burst_length_in_bytes);
	max_set_uint64t(actions, "KernelLMem_Read_CommandAndDataStream", "wordsPerBurst", burst_length_in_bytes / 8);
	max_run(engine, actions);

	max_reset_engine(engine);

	max_queue_input(actions, "fromCpu", data_in, size_bytes);
	max_lmem_set_interrupt_on(actions, "toLmem");
	printf("Performing max_run()\n");
	max_run(engine, actions);
	printf("Done\n");
	max_actions_free(actions);
}

void read_data_from_lmem(uint64_t *data_out, int size, int size_bytes, int burst_length_in_bytes, max_engine_t *engine, max_file_t *maxfile)
{
	max_actions_t *actions = max_actions_init(maxfile, NULL);

	max_set_ticks(actions, "KernelLMem_Write_CommandAndDataStream", 0);
	max_set_uint64t(actions, "KernelLMem_Write_CommandAndDataStream", "totalBursts", size * 8 / burst_length_in_bytes);
	max_set_uint64t(actions, "KernelLMem_Write_CommandAndDataStream", "wordsPerBurst", burst_length_in_bytes / 8);
	max_set_ticks(actions, "KernelLMem_Read_CommandAndDataStream", size);
	max_set_uint64t(actions, "KernelLMem_Read_CommandAndDataStream", "totalBursts", size * 8 / burst_length_in_bytes);
	max_set_uint64t(actions, "KernelLMem_Read_CommandAndDataStream", "wordsPerBurst", burst_length_in_bytes / 8);
	max_run(engine, actions);

	max_reset_engine(engine);

	max_queue_output(actions, "toCpu", data_out, size_bytes);
	max_run(engine, actions);
	max_actions_free(actions);
}

void fill_rom_crc_index(FILE *fp_crc, uint64_t **p_crc, long l_crc)
{
	size_t result_crc;
	char *tr;
	double diff_crc = ceil(l_crc / 8.0) - l_crc / 8.0; //padding with NULL character - integer multiple of bytes (1B=8 bits)

	if (diff_crc != 0) {
		l_crc = (int) ceil(l_crc / 8.0) * 8;
	}

	//allocate memory to contain the whole file
	size_t num_elem_crc = sizeof(uint64_t) * (l_crc / 8);
	uint64_t* arr_crc = (uint64_t*) malloc(num_elem_crc);

	result_crc = fread(arr_crc, 1, l_crc, fp_crc); //read 8 bytes (64 bits) ie. 8 elements of 1 byte in size

	for (int i = 0; i < l_crc / 8; i++) {
		tr = (char*) (arr_crc + i);
		//byte reverse
		for (int j = 0; j < 4; j++) {
			char wt = tr[j];
			tr[j] = tr[7 - j];
			tr[7 - j] = wt;
		}
	}

	*p_crc = arr_crc;
}

int main(int argc, char *argv[])
{
	DocoptArgs args = docopt(argc, argv, /* help */ 1, /* version */ "2.0rc2");
        
	char *path;
        int port;

	if (args.port) {
		int len = strlen(args.port);
		int i;
		for (i = 0; i < len; i++) {
			if (!isdigit(args.port[i])) {
				printf("Entered port value is not a number\n");
				exit(1);
			}
		}
		port = atoi(args.port);
	}

	if (args.path) {
		path = args.path;
	}
        
        init_code(path);

	uint16_t n_sockets = 1;
        
	printf("args.dfe_ip=%s, args.netmask=%s, args.port=%s, args.path=%s\n", args.dfe_ip, args.netmask, args.port, args.path);
	struct in_addr dfe_ip;
	inet_aton(args.dfe_ip, &dfe_ip);
	struct in_addr netmask;
	inet_aton(args.netmask, &netmask);

	// initialization files for crcIndex table, generated by init_code
	char file_crc_index1[] = "./results/romCrcIndex1_init";
	char file_crc_index2[] = "./results/romCrcIndex2_init";

	// LMEM initialization file location, generated by init_code
	char file_lmem[] = "./results/lmem_generated_file";

	uint64_t *arr_crc1;
	uint64_t *arr_crc2;
	long l_crc;

	FILE *fp_crc1 = fopen(file_crc_index1, "rb");
	FILE *fp_crc2 = fopen(file_crc_index2, "rb");
	FILE *fp_lmem = fopen(file_lmem, "rb");

	if (!(fp_crc1 && fp_crc2 && fp_lmem)) {
		printf("Error with file\n");
		exit(0);
	}

	// obtain file size
	fseek(fp_crc1, 0, SEEK_END);
	l_crc = ftell(fp_crc1);
	rewind(fp_crc1);
        fill_rom_crc_index(fp_crc1, &arr_crc1, l_crc);
        fill_rom_crc_index(fp_crc2, &arr_crc2, l_crc);

	printf("Preparing for init() and max_load()\n");
	max_file_t *maxfile = httpserver_init();
	max_engine_t * engine = max_load(maxfile, "*");
	printf("Done\n");

	max_actions_t *actions = max_actions_init(maxfile, NULL);

	int rom_depth_crc = l_crc / 8;
	for (uint32_t i = 0; i < rom_depth_crc; i++) {
		max_set_mem_uint64t(actions, "CrcIndexTable", "romCrcIndex1", i, arr_crc1[i]);
		max_set_mem_uint64t(actions, "CrcIndexTable", "romCrcIndex2", i, arr_crc2[i]);
	}

	max_run(engine, actions);
	max_actions_free(actions);

	long L;
	size_t result;
	uint64_t* arr_lmem;

	// obtain file size
	fseek(fp_lmem, 0, SEEK_END);
	L = ftell(fp_lmem);
	rewind(fp_lmem);

	double diff = ceil(L / 8.0) - L / 8.0; // NULL character padding

	if (diff != 0) {
		L = (int) ceil(L / 8.0) * 8;
	}

	// allocate memory to contain the whole file
	size_t n_elem = sizeof(uint64_t) * (L / 8);
	arr_lmem = (uint64_t*) malloc(n_elem);

	result = fread(arr_lmem, 1, L, fp_lmem); 

	int rom_depth = L / 8;

	int burst_length_in_bytes = max_get_burst_size(maxfile, "cmd_tolmem");
	inline int max(int a, int b) 
        {
		return a > b ? a : b;
	};

	const int size = rom_depth;
	int size_bytes = size * sizeof(uint64_t);
	uint64_t *in_data; 

	printf("Writing to DFE memory.\n");
	in_data = arr_lmem;

	write_data_to_lmem(in_data, size, size_bytes, burst_length_in_bytes, engine, maxfile);
	printf("Done\n");

	max_ip_config(engine, MAX_NET_CONNECTION_QSFP_BOT_10G_PORT1, &dfe_ip, &netmask);

	//all sockets MUST be created before first call to max_tcp_connect or max_tcp_listen
	max_tcp_socket_t *(dfe_socket[n_sockets]);
	uint16_t socket_number[n_sockets];

	for (int i = 0; i < n_sockets; i++) {
		dfe_socket[i] = max_tcp_create_socket_with_number(engine, "tcp_ISCA_QSFP_BOT_10G_PORT1", i);
		socket_number[i] = max_tcp_get_socket_number(dfe_socket[i]);
		printf("Socket %d was assigned socket number %u\n", i, socket_number[i]);
	}

	for (int i = 0; i < n_sockets; i++) {
		max_tcp_listen(dfe_socket[i], port + i);
		max_tcp_await_state(dfe_socket[i], MAX_TCP_STATE_LISTEN, NULL);
	}

	printf("CPU code: Total %u socket(s), listening on the port(s) %u-%u\n\n", n_sockets, port, port + n_sockets - 1);

	void *read_ptr;
	uint8_t *read_buffer;
	max_llstream_t *read_llstream;
	uint64_t *byte_number;
	printf("CPU code: Setting up 'toCpuByteNumber' stream.\n");
	int n_slots_byte_number = 512;
	size_t tCBN_buffer_size = n_slots_byte_number * 16;
	posix_memalign((void *) &read_buffer, 4096, tCBN_buffer_size);
	read_llstream = max_llstream_setup(engine, "toCpuFileSizeBytes", n_slots_byte_number, 16, read_buffer);

	uint8_t *read_buffer_socket;
	max_llstream_t *read_llstream_socket;
	printf("CPU code: Setting up 'toCpuSocketNumber' stream.\n");
	int n_slots_socket_number = 512;
	size_t tCSB_buffer_size = n_slots_socket_number * 16;
	posix_memalign((void *) &read_buffer_socket, 4096, tCSB_buffer_size);
	read_llstream_socket = max_llstream_setup(engine, "toCpuSocketNumber", n_slots_socket_number, 16, read_buffer_socket);
	void *read_ptr_socket_slot;

	uint16_t ti = 10;
	while(ti > 0)
	{
		printf("CPU code: time=%u, waiting file size and socket numbers stream data to be sent to CPU\n", ti);
		usleep(1000*1000*1);
		ti--;
	}

	uint64_t num_rx_bytes;
	uint64_t num_tx_bytes;
	uint8_t session_id;
	while (1) {

		//part 1: first wait to receive LengthBytes number

		printf("CPU code: PART 1 - waiting to receive LengthBytes number\n");

		int found_byte_number = 0;
		ti=0;
		while (found_byte_number != 1) //first wait to receive LengthBytes number
		{
			usleep(1000*1000*1);

			for (int i = 0; i < n_sockets; i++) {
				max_tcp_get_num_bytes_received(dfe_socket[i], &num_rx_bytes);
				max_tcp_get_num_bytes_transmitted(dfe_socket[i], &num_tx_bytes, &session_id);
				printf("CPU code: waiting, time=%u, port=%u, socket=%i, max_tcp_get_num_bytes_received=%llu, max_tcp_get_num_bytes_transmitted=%llu\n", ti, port + i, i, (long long unsigned int) num_rx_bytes, (long long unsigned int) num_tx_bytes);
			}
			ti++;

			uint8_t ii = max_llstream_read(read_llstream, 1, &read_ptr);
			if (ii) {
				byte_number = (uint64_t*) read_ptr;
				printf("CPU code: number of slots found to contain new data=%u,  fileSizeBytes=%u\n", ii, (unsigned int) *byte_number);
				max_llstream_read_discard(read_llstream, 1);
				found_byte_number = 1;
			}
		}

		//part 2: receive total number of data transfered

		printf("CPU code: PART 2 - receive socket number\n");
		while (max_llstream_read(read_llstream_socket, 1, &read_ptr_socket_slot) == 0);
		
		uint16_t socket_returned = (uint16_t) *((uint16_t*) read_ptr_socket_slot); //event->socketID;
		unsigned int file_bytes = (unsigned int) *byte_number;

		printf("CPU code: fileBytes=%u, socket_returned=%u\n", file_bytes, socket_returned);

		ti = 0;
		while (1) {
			{
				for (int i = 0; i < n_sockets; i++)
				{
					max_tcp_get_num_bytes_received(dfe_socket[i], &num_rx_bytes);
					max_tcp_get_num_bytes_transmitted(dfe_socket[i], &num_tx_bytes, &session_id);
					printf("CPU code: time=%i, port=%u, socket=%i, max_tcp_get_num_bytes_received=%llu, max_tcp_get_num_bytes_transmitted=%llu\n", ti, port + i, i, (long long unsigned int) num_rx_bytes, (long long unsigned int) num_tx_bytes);
				}
				ti++;
				printf("\n");

				max_tcp_get_num_bytes_transmitted(dfe_socket[socket_returned], &num_tx_bytes, &session_id);
				printf("CPU code: fileSizeBytes=%u, socketReturned=%u, num_tx_bytes=%llu\n", file_bytes, socket_returned, (long long unsigned int) num_tx_bytes);
			}

			if (num_tx_bytes == file_bytes) {
				printf("CPU code: MATCH num_tx_bytes==fileBytes, socket_returned=%u, fileBytes=%u, num_tx_bytes(max_tcp_get_num_bytes_transmitted)=%llu\n", socket_returned, file_bytes, (long long unsigned int) num_tx_bytes);
				printf("CPU code: Closing socket=%u\n", socket_returned);
				max_tcp_close(dfe_socket[socket_returned]);

				printf("CPU code: Waiting for MAX_TCP_STATE_CLOSED\n");
				max_tcp_await_state(dfe_socket[socket_returned], MAX_TCP_STATE_CLOSED, NULL);

				printf("CPU code: Set LISTEN state\n");
				max_tcp_listen(dfe_socket[socket_returned], port);

				printf("CPU code: Waiting for MAX_TCP_STATE_LISTEN\n");
				max_tcp_await_state(dfe_socket[socket_returned], MAX_TCP_STATE_LISTEN, NULL);

				printf("CPU code: Again opened socket=%u\n", socket_returned);

				printf("\nCPU code: State of rx/tx after socket closing\n");

				break;
			}

			usleep(1000*1000*1);
		}
	}

	for (int i = 0; i < n_sockets; i++) {
		max_tcp_close(dfe_socket[i]);
		printf("max_tcp_close(dfe_socket[i])");
	}

	max_unload(engine);
	printf("max_unload(engine)");
	max_file_free(maxfile);
	printf("max_file_free(maxfile)");

	printf("The end\n");

	return 0;
}
