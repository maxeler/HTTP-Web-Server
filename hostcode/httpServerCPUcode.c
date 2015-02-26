#define _GNU_SOURCE
#define CEILING_POS(X) ((X-(uint64_t)(X)) > 0 ? (uint64_t)(X+1) : (uint64_t)(X));

#include <string.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/tcp.h>

#include <MaxSLiCInterface.h>
#include "httpServer.h"
#include <math.h>

void generateInputData(int size, uint64_t *inData) {
	srand(time(0));
	for (int i = 0; i < size; i++) {
		inData[i] = i;
	}
}

typedef struct tx_event_type_s {
	uint16_t socketID :16;
	uint64_t frameCount;
	uint64_t reserved :48;
}__attribute__ ((__packed__)) tx_event_type_t;

void writeDataToLMem(uint64_t *dataIn, int size, int sizeBytes, int burstLengthInBytes, max_engine_t *engine, max_file_t *maxfile) {

	max_actions_t *actions = max_actions_init(maxfile, NULL);

	max_set_ticks(actions, "KernelLMem_Write_CommandAndDataStream", size);
	max_set_uint64t(actions, "KernelLMem_Write_CommandAndDataStream", "totalBursts", size * 8 / burstLengthInBytes);
	max_set_uint64t(actions, "KernelLMem_Write_CommandAndDataStream", "wordsPerBurst", burstLengthInBytes / 8);
	max_set_ticks(actions, "KernelLMem_Read_CommandAndDataStream", 0);
	max_set_uint64t(actions, "KernelLMem_Read_CommandAndDataStream", "totalBursts", size * 8 / burstLengthInBytes);
	max_set_uint64t(actions, "KernelLMem_Read_CommandAndDataStream", "wordsPerBurst", burstLengthInBytes / 8);
	max_run(engine, actions);

	max_reset_engine(engine);

	max_queue_input(actions, "fromCpu", dataIn, sizeBytes);
	max_lmem_set_interrupt_on(actions, "toLmem");
	max_run(engine, actions);
	max_actions_free(actions);

}

void readDataFromLMem(uint64_t *dataOut, int size, int sizeBytes, int burstLengthInBytes, max_engine_t *engine, max_file_t *maxfile) {

	max_actions_t *actions = max_actions_init(maxfile, NULL);

	max_set_ticks(actions, "KernelLMem_Write_CommandAndDataStream", 0);
	max_set_uint64t(actions, "KernelLMem_Write_CommandAndDataStream", "totalBursts", size * 8 / burstLengthInBytes);
	max_set_uint64t(actions, "KernelLMem_Write_CommandAndDataStream", "wordsPerBurst", burstLengthInBytes / 8);
	max_set_ticks(actions, "KernelLMem_Read_CommandAndDataStream", size);
	max_set_uint64t(actions, "KernelLMem_Read_CommandAndDataStream", "totalBursts", size * 8 / burstLengthInBytes);
	max_set_uint64t(actions, "KernelLMem_Read_CommandAndDataStream", "wordsPerBurst", burstLengthInBytes / 8);
	max_run(engine, actions);

	max_reset_engine(engine);

	max_queue_output(actions, "toCpu", dataOut, sizeBytes);
	max_run(engine, actions);
	max_actions_free(actions);

}

void fillRomCrcIndex(FILE *fpCrc, uint64_t **pCrc, long Lcrc) {

	size_t resultCrc;
	char *tr;
	double diffCrc = ceil(Lcrc / 8.0) - Lcrc / 8.0; //padding with NULL character - integer multiple of bytes (1B=8 bits)

	if (diffCrc != 0) {
		Lcrc = (int) ceil(Lcrc / 8.0) * 8;
	}

	//allocate memory to contain the whole file
	size_t NelemCrc = sizeof(uint64_t) * (Lcrc / 8);
	uint64_t* nizCrc = (uint64_t*) malloc(NelemCrc);

	resultCrc = fread(nizCrc, 1, Lcrc, fpCrc); //read 8 bytes (64 bits) ie. 8 elements of 1 byte in size


	for (int i = 0; i < Lcrc / 8; i++) {
		tr = (char*) (nizCrc + i);
		//byte reverse
		for (int j = 0; j < 4; j++) {
			char wt = tr[j];
			tr[j] = tr[7 - j];
			tr[7 - j] = wt;
		}
	}

	*pCrc = nizCrc;

}

int main(int argc, char *argv[]) {
	if (argc != 3) {
		printf("Usage: %s <dfe_ip> <netmask>\n", argv[0]);
		return 1;
	}

	struct in_addr dfe_ip;
	inet_aton(argv[1], &dfe_ip);
	struct in_addr netmask;
	inet_aton(argv[2], &netmask);
	const int port = 80;

	// initialization files for crcIndex table, generated by init_code
	char fileCrcIndex1[] = "./romCrcIndex1_init.html";
	char fileCrcIndex2[] = "./romCrcIndex2_init.html";

	// LMEM initialization file location, generated by init_code
	char fileLmem[] = "./lmem_generated_file.html";

	uint64_t *arrCrc1;
	uint64_t *arrCrc2;
	long Lcrc;

	FILE *fpCrc1 = fopen(fileCrcIndex1, "rb");
	FILE *fpCrc2 = fopen(fileCrcIndex2, "rb");
	FILE *fpLmem = fopen(fileLmem, "rb");

	if (!(fpCrc1 && fpCrc2 && fpLmem)) {
		printf("Error with file\n");
		exit(0);
	}

	// obtain file size
	fseek(fpCrc1, 0, SEEK_END);
	Lcrc = ftell(fpCrc1);
	rewind(fpCrc1);

	fillRomCrcIndex(fpCrc1, &arrCrc1, Lcrc);
	fillRomCrcIndex(fpCrc2, &arrCrc2, Lcrc);

	max_file_t *maxfile = httpServer_init();
	max_engine_t * engine = max_load(maxfile, "*");
	max_actions_t *actions = max_actions_init(maxfile, NULL);

	int romDepthCrc = Lcrc / 8;
	for (uint32_t i = 0; i < romDepthCrc; i++) {
		max_set_mem_uint64t(actions, "crcIndex", "romCrcIndex1", i, arrCrc1[i]);
		max_set_mem_uint64t(actions, "crcIndex", "romCrcIndex2", i, arrCrc2[i]);
	}

	max_run(engine, actions);
	max_actions_free(actions);

	long L;
	size_t result;
	uint64_t* arrLmem;

	// obtain file size
	fseek(fpLmem, 0, SEEK_END);
	L = ftell(fpLmem);
	rewind(fpLmem);

	double diff = ceil(L / 8.0) - L / 8.0; // NULL character padding

	if (diff != 0) {
		L = (int) ceil(L / 8.0) * 8;
	}

	// allocate memory to contain the whole file
	size_t Nelem = sizeof(uint64_t) * (L / 8);
	arrLmem = (uint64_t*) malloc(Nelem);

	result = fread(arrLmem, 1, L, fpLmem); //read 8 bytes (64 bits) ie. 8 elements of 1 byte in size

	int romDepth = L / 8;

	int burstLengthInBytes = max_get_burst_size(maxfile, "cmd_tolmem");
	inline int max(int a, int b) {
		return a > b ? a : b;
	}
	;

	const int size = romDepth;
	int sizeBytes = size * sizeof(uint64_t);
	uint64_t *inData = malloc(sizeBytes);

	generateInputData(size, inData);

	//action #1

	printf("Writing to DFE memory.\n");
	inData = arrLmem;
	writeDataToLMem(inData, size, sizeBytes, burstLengthInBytes, engine, maxfile);

	//action #2

	//  printf("Reading DFE memory.\n");
	//	uint64_t *outData = malloc(sizeBytes);
	//	readDataFromLMem(outData, size, sizeBytes, burstLengthInBytes, engine, maxfile);
	//	for (int i = 0; i < size; i++) {
	//	printf("Element %d, outData[%d]=%016llx\n",i,i,outData[i]);
	//	}

	max_ip_config(engine, MAX_NET_CONNECTION_QSFP_BOT_10G_PORT1, &dfe_ip, &netmask);

	//all sockets MUST be created before first call to max_tcp_connect or max_tcp_listen
	uint16_t Nsockets = 1;
	max_tcp_socket_t *( dfe_socket[Nsockets]);
	uint16_t socketNumber[Nsockets];

	for (int i = 0; i < Nsockets; i++) {
		dfe_socket[i] = max_tcp_create_socket(engine, "tcp_ISCA_QSFP_BOT_10G_PORT1");
		socketNumber[i] = max_tcp_get_socket_number(dfe_socket[i]);
		printf("Socket %d was assigned socket number %u\n", i, socketNumber[i]);
	}

	for (int i = 0; i < Nsockets; i++) {
		max_tcp_listen(dfe_socket[i], port);
		max_tcp_await_state(dfe_socket[i], MAX_TCP_STATE_LISTEN, NULL);
	}

	uint64_t frameCounter = 0;
	uint64_t frameCounterPrevious = 0;

	void *read_ptr;
	uint8_t *read_buffer;
	max_llstream_t *read_llstream;
	uint64_t *byteNumber;
	printf("CPU code: Setting up output streams.\n");
	posix_memalign((void *) &read_buffer, 16, 16 * 1);
	read_llstream = max_llstream_setup(engine, "toCpuByteNumber", 1, 16, read_buffer);

	size_t txMgr_buffer_size = 512 * sizeof(tx_event_type_t);
	void *txMgr_buffer;
	posix_memalign(&txMgr_buffer, 4096, txMgr_buffer_size);
	max_llstream_t *llstream = max_llstream_setup(engine, "toCpuOutputTxEvent", 512, sizeof(tx_event_type_t), txMgr_buffer);
	void *txMgrSlot;
	tx_event_type_t *event;

	while (1) {

		//part 1: first wait to receive LengthBytes number

		printf("CPU code: PART 1 - waiting to receive LengthBytes number\n");

		int FoundByteNumber = 0;
		while (FoundByteNumber != 1) //first wait to receive LengthBytes number
		{
			uint8_t ii = max_llstream_read(read_llstream, 1, &read_ptr);
			if (ii) {
				byteNumber = (uint64_t*) read_ptr;
				printf("CPU code: LengthBytes=%u\n",(unsigned int) *byteNumber);
				max_llstream_read_discard(read_llstream, 1);
				FoundByteNumber = 1;
			}
		}

		//part 2: receive total number of data transfered

		printf("CPU code: PART 2 - receive total number of data transfered\n");

		int exitFlagw = 0;
		while (exitFlagw == 0) {
			while (max_llstream_read(llstream, 1, &txMgrSlot) == 0)
				;

			event = txMgrSlot;
			printf("CPU code: Got ack from TxManager: Total frames: %ld\n", event->frameCount);
			max_llstream_read_discard(llstream, 1);
			frameCounter = (event->frameCount) + 1;
			printf("CPU code: frameCounter=%ld\n", frameCounter);

			double tt = (*byteNumber) / 8.0;
			uint64_t y = CEILING_POS(tt);
			if ((frameCounter - frameCounterPrevious) == y) {

				sleep(2);
				printf("CPU code: frameCounter=%ld, frameCounterPrevious=%ld, ceil(fileLength)=%u\n", frameCounter, frameCounterPrevious,(unsigned int) y);

				uint16_t socket_returned = event->socketID;
				printf("CPU code: Closing socket=%u\n", socket_returned);
				max_tcp_close(dfe_socket[socket_returned]);
				//max_tcp_close_mode_t close_mode=MAX_TCP_CLOSE_ABORT_RESET;
				//max_tcp_close_advanced(dfe_socket[socket_returned],close_mode);

				printf("CPU code: Waiting for MAX_TCP_STATE_CLOSED\n");
				max_tcp_await_state(dfe_socket[socket_returned], MAX_TCP_STATE_CLOSED, NULL);

				printf("CPU code: Set LISTEN state\n");
				max_tcp_listen(dfe_socket[socket_returned], port);

				printf("CPU code: Waiting for MAX_TCP_STATE_LISTEN\n");
				max_tcp_await_state(dfe_socket[socket_returned], MAX_TCP_STATE_LISTEN, NULL);

				printf("CPU code: Again opened socket=%u\n", socket_returned);

				frameCounterPrevious = frameCounter;
				exitFlagw = 1;
			}
		}
	}

	for (int i = 0; i < Nsockets; i++) {
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