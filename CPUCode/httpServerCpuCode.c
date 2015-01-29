#define _GNU_SOURCE
#define CEILING_POS(X) ((X-(uint64_t)(X)) > 0 ? (uint64_t)(X+1) : (uint64_t)(X));

#include <string.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/tcp.h>

#include <MaxSLiCInterface.h>
#include "Maxfiles.h"

typedef struct {
	uint16_t total_items;
	uint32_t sum;
	uint16_t test;
	uint8_t bajt1;
} __attribute__ ((__packed__)) frame_t;

typedef struct {
	uint64_t data;
	//uint32_t data2;
//	uint8_t  mod:3;
//	uint8_t  eof:1;
//	uint8_t  sof:1;
//	uint8_t  socket;
} __attribute__ ((__packed__)) frame_rx_udp;


static int create_cpu_tcp_socket() {
	int sock = socket(AF_INET, SOCK_STREAM, 0);

	int state = 1;
	setsockopt(sock, IPPROTO_TCP, TCP_NODELAY, &state, sizeof(state));

	return sock;
}


//static void exchangeItems(int sock, const uint8_t* items, uint8_t num_items) {
static void exchangeItems(int sock, const uint8_t* items, uint16_t num_items) {

	static uint32_t total_items = 0;

	//uint8_t data_to_send[1 + UINT8_MAX];
	//data_to_send[0] = num_items;
	//memcpy(data_to_send, items, num_items);

	//send(sock, &data_to_send,num_items,0);  //num_items + 1

	int remainingSend=sizeof(items);
	unsigned int sendCount;
	//while(remainingSend>0) {
		sendCount=send(sock,items,num_items,0);  //num_items + 1
		remainingSend=remainingSend-sendCount;
	//}

//
	printf("sizeof(items) = %u\n", num_items);

	//printf("Broj elemenata niza = %u\n", num_items);
	printf("Broj poslatih sendCount = %u\n", sendCount);
	//getchar();

	frame_rx_udp data_received;
	uint8_t word;
	int L=8192; //8KB (KB=kilo bytes)
	uint8_t recvniz[L];
	for(int i=0;i<L-1;i++)
		recvniz[i]=0;

	int y=0;
//	while (y<8)
//	{
	unsigned int recvCount=recv(sock, recvniz, sizeof(recvniz), 0);
//	y=y+8
//	}
	//printf("sizeof: frame_rx_udp=%u\n",sizeof(data_received));
	//exit(1);


	//printf("Received: data=%u, data2=%u, mod=%u, eof=%u, sof=%u, socket=%u \n",
//			printf("Received: data=%x, data2=%x\n",
//			data_received.data
//				,
//			data_received.data2
//
////			data_received.mod,
////			data_received.eof,
////			data_received.sof,
////			data_received.socket
//			);

//	for(unsigned int i=0;i<recvCount;i++)
//	{
////		printf("Received: data0=%x, data1=%x, data2=%x, data3=%x, data4=%x, data5=%x, data6=%x, data7=%x\n",
////				recvniz[0],
////				recvniz[1],
////				recvniz[2],
////				recvniz[3],
////				recvniz[4],
////				recvniz[5],
////				recvniz[6],
////				recvniz[7]
////		);
//		printf("Received: %d data=%c , data(hex)=%x\n",i,recvniz[i],recvniz[i]);
//	}



	printf("Broj poslatih sendCount = %u, Broj primljenih recvCount = %u\n", sendCount,recvCount);

	total_items += num_items;

	uint32_t sum = 0;
//	for (int i = 0; i < num_items; i++)
//		sum += items[i];
//
//	if (data_received.total_items != total_items || data_received.sum != sum) {
//		printf("Error! Expected: total_items = %u, sum = %d\n", total_items, sum);
//		//exit(1);
//	}
}

void generateInputData(int size, uint64_t *inA)
{
	srand(time(0));
	for (int i = 0; i < size; i++) {
		inA[i] = i;
	}
}

typedef struct {
	uint8_t byte1;
	uint8_t byte2;
	uint8_t byte3;
	uint8_t byte4;
	uint8_t byte5;
	uint8_t byte6;
	uint8_t byte7;
	uint8_t byte8;
} __attribute__ ((__packed__)) disconnect_format;

typedef struct tx_event_type_s {
    uint16_t  socketID : 16;
    uint64_t  frameCount;
    uint64_t  reserved : 48;
} __attribute__ ((__packed__)) tx_event_type_t;


void writeDataToLMem(uint64_t *dataIn, int size, int sizeBytes, int burstLengthInBytes, max_engine_t *engine, max_file_t *maxfile){

	max_actions_t *actions = max_actions_init(maxfile, NULL);

	max_set_ticks(actions, "KernelLMem_Write_CommandAndDataStream", size);
	max_set_uint64t( actions, "KernelLMem_Write_CommandAndDataStream", "totalBursts",size*8/burstLengthInBytes);
	max_set_uint64t( actions, "KernelLMem_Write_CommandAndDataStream", "wordsPerBurst", burstLengthInBytes/8);
	max_set_ticks(actions, "KernelLMem_Read_CommandAndDataStream", 0);
	max_set_uint64t( actions, "KernelLMem_Read_CommandAndDataStream", "totalBursts",size*8/burstLengthInBytes);
	max_set_uint64t( actions, "KernelLMem_Read_CommandAndDataStream", "wordsPerBurst", burstLengthInBytes/8);
	max_run(engine, actions);

	max_reset_engine(engine);

	max_queue_input(actions, "fromCpu", dataIn,sizeBytes);
	max_lmem_set_interrupt_on(actions, "toLmem");
	max_run(engine, actions);
	max_actions_free(actions);

}

void fillRomHashIndex(FILE *fpHash, uint64_t **pHash, long Lhash) {

	size_t resultHash;
	char *tr;
	double diffHash = ceil(Lhash / 8.0) - Lhash / 8.0; //padding with NULL character - integer multiple of bytes (1B=8 bits)

	if (diffHash != 0) {
		Lhash = (int) ceil(Lhash / 8.0) * 8;
	}

	//allocate memory to contain the whole file
	size_t NelemHash = sizeof(uint64_t) * (Lhash / 8);
	uint64_t* nizHash = (uint64_t*) malloc(NelemHash);

	//for(int i=0;i<L/8;i++)
	{
		resultHash = fread(nizHash, 1, Lhash, fpHash); //read 8 bytes (64 bits) ie. 8 elements of 1 byte in size
	}

	for (int i = 0; i < Lhash / 8; i++) {
		tr = (char*) (nizHash + i);

		//byte, not bit!!! reverse
		for (int j = 0; j < 4; j++) {
			char wt = tr[j];
			tr[j] = tr[7 - j];
			tr[7 - j] = wt;
		}

		//printf("Element niz[%d]=%016llx\n", i, nizHash[i]);
	}

	*pHash = nizHash;

}


int main(int argc, char *argv[]) {
	if(argc != 4) {
		printf("Usage: %s <dfe_ip> <cpu_ip> <netmask>\n", argv[0]);
		return 1;
	}

	struct in_addr dfe_ip;
	inet_aton(argv[1], &dfe_ip);
	struct in_addr cpu_ip;
	inet_aton(argv[2], &cpu_ip);
	struct in_addr netmask;
	inet_aton(argv[3], &netmask);
	const int port = 80;//80;//5007;

	//romHASHindexMapped rom initialization

					//***initialization files for hashIndex table***//
					//char fileHashIndex[]="/home/nemanja/Desktop/hashIndexFinalLongLMem.html";
					//char fileHashIndex[]="/home/nemanja/Desktop/hashIndexFinalLongLMemTest.html";
					//char fileHashIndex[]="/home/nemanja/Desktop/hashIndexFinalLongLMemExactBytes.html";
						//char fileHashIndex[]="/home/nemanja/Desktop/hashIndexFinalLongLMemExactBytes17.html";
					//char fileHashIndex[]="/home/nemanja/Desktop/hashIndexFinalLongLMemExactBytes380.html";
					//char fileHashIndex[]="/home/nemanja/Desktop/hashIndexFinalLongLMemExactBytes1920.html";
					//char fileHashIndex[]="/home/nemanja/Desktop/hashIndexFinalLongLMemExactBytes10560.html";
					//char fileHashIndex[]="/home/nemanja/Desktop/hashIndexFinalLongLMemExactBytes19200.html";
					//char fileHashIndex[]="/home/nemanja/Desktop/hashIndexFinalLongLMemExactBytesVeliko.html";

					char fileHashIndex1[]="/home/mvorkapic/workspace/working/CPUCode/romHashIndex1_init.html";
					char fileHashIndex2[]="/home/mvorkapic/workspace/working/CPUCode/romHashIndex2_init.html";

					uint64_t *arrHash1;
					uint64_t *arrHash2;
					long Lhash;

					FILE *fpHash1 = fopen (fileHashIndex1,"rb");
					FILE *fpHash2 = fopen (fileHashIndex2,"rb");

					// obtain file size:
					fseek (fpHash1 , 0 , SEEK_END);
					Lhash = ftell (fpHash1);
					rewind(fpHash1);

					fillRomHashIndex(fpHash1, &arrHash1, Lhash);
					fillRomHashIndex(fpHash2, &arrHash2, Lhash);

					//getchar();

					max_file_t *maxfile = httpServer_init();
					max_engine_t * engine = max_load(maxfile, "*");
					max_actions_t *actions = max_actions_init(maxfile, NULL);

					int romDepthHash=Lhash/8;
					for(uint32_t i=0;i<romDepthHash;i++)
					{
						//max_set_mem_uint64t(actions, "stamp_state_machine", "romMapped", i, niz[i]);
						//printf("FFFFFFF 1&2 %d: %016llx, %016llx\n", i, nizHash1[i], nizHash2[i]);

						max_set_mem_uint64t(actions, "httpServer_state_machine", "romHashIndex1", i, arrHash1[i]);
						max_set_mem_uint64t(actions, "httpServer_state_machine", "romHashIndex2", i, arrHash2[i]);
					}

					//max_set_mem_uint64t(actions, "stamp_state_machine", "romMapped", 0, niz[0]);
					//exit(0);
					max_run(engine, actions);
					max_actions_free(actions);


	//LMem initialization
				//max_set_mem_uint64t (max_actions_t *actions, const char *block_name, const char *mem_name, size_t index, uint64_t v)

				//char file[]="/home/nemanja/Desktop/htmlTest5LongManja.html";
				//char file[]="/home/nemanja/Desktop/htmlTest5LongManjaNew.html";
				//char file[]="/home/nemanja/Desktop/Test.html";
			    //char file[]="/home/nemanja/Desktop/htmlTest5LongLMem.html";

				char file[]="/home/mvorkapic/workspace/working/CPUCode/lmem_generated_file.html";

				long L;
				size_t result;
				uint64_t* niz;

				FILE *fp = fopen (file,"rb");

				// obtain file size:
				fseek (fp , 0 , SEEK_END);
				L = ftell (fp);
				rewind(fp);

				double diff = ceil(L/8.0) - L/8.0; //dopunjavanje NULL characterom do celobrojonog umnoska bajtova (1B=8 bita)

				if (diff!=0)
				{
					L=(int) ceil(L/8.0)*8;
				}

				//allocate memory to contain the whole file
				size_t Nelem = sizeof(uint64_t)*(L/8);
				niz = (uint64_t*) malloc (Nelem);

				{
					result = fread(niz,1,L,fp); //read 8 bytes (64 bits) ie. 8 elements of 1 byte in size
				}

				for(int i=0;i<L/8;i++)
				{
					//printf("Element niz[%d]=%lu\n",i,niz[i]);
				}

				int romDepth=L/8;

	//LMem
	//NETWORK

		//tcp
		//max_file_t *maxfile = Stamp_init();

		int burstLengthInBytes = max_get_burst_size(maxfile, "cmd_tolmem");
		inline int max( int a, int b ) { return a > b ? a : b; };

		//const int size = 1920; //max(96, burstLengthInBytes/4);
		const int size = romDepth; //max(96, burstLengthInBytes/4);
		int sizeBytes = size * sizeof(uint64_t);
		uint64_t *inA = malloc(sizeBytes);

		generateInputData(size, inA);

		inA=niz;

		printf("Loading DFE memory.\n");

		uint64_t *outData = malloc(sizeBytes);

		//readDataFromLMem()


		//action #1

	    writeDataToLMem(inA,size,sizeBytes,burstLengthInBytes,engine,maxfile);

//		actions = max_actions_init(maxfile, NULL);
//
//		max_set_ticks(actions, "KernelLMem_Write_CommandAndDataStream", size);
//		max_set_uint64t( actions, "KernelLMem_Write_CommandAndDataStream", "totalBursts",size*8/burstLengthInBytes);
//		max_set_uint64t( actions, "KernelLMem_Write_CommandAndDataStream", "wordsPerBurst", burstLengthInBytes/8);
//		max_set_ticks(actions, "KernelLMem_Read_CommandAndDataStream", 0);
//		max_set_uint64t( actions, "KernelLMem_Read_CommandAndDataStream", "totalBursts",size*8/burstLengthInBytes);
//		max_set_uint64t( actions, "KernelLMem_Read_CommandAndDataStream", "wordsPerBurst", burstLengthInBytes/8);
//		max_run(engine, actions);
//
//		max_reset_engine(engine);
//
//		max_queue_input(actions, "fromCpu", inA,sizeBytes);
//		max_lmem_set_interrupt_on(actions, "toLmem");
//		max_run(engine, actions);
//		max_actions_free(actions);

		//sleep(100);

		//action #2
		actions = max_actions_init(maxfile, NULL);
		max_set_ticks(actions, "KernelLMem_Write_CommandAndDataStream", 0);
		max_set_uint64t( actions, "KernelLMem_Write_CommandAndDataStream", "totalBursts",size*8/burstLengthInBytes);
		max_set_uint64t( actions, "KernelLMem_Write_CommandAndDataStream", "wordsPerBurst", burstLengthInBytes/8);
		max_set_ticks(actions, "KernelLMem_Read_CommandAndDataStream", size);
		max_set_uint64t( actions, "KernelLMem_Read_CommandAndDataStream", "totalBursts",size*8/burstLengthInBytes);
		max_set_uint64t( actions, "KernelLMem_Read_CommandAndDataStream", "wordsPerBurst", burstLengthInBytes/8);
		max_run(engine, actions);

		max_reset_engine(engine);

		max_queue_output(actions, "toCpu", outData, sizeBytes);
		max_run(engine, actions);
		max_actions_free(actions);

		printf("Reading DFE memory.\n");

		for(int i=0;i<size;i++)
		{
			//printf("Element %d, outData[%d]=%016llx\n",i,i,outData[i]);
		}

		//return 0;

		//getchar();


	max_ip_config(engine, MAX_NET_CONNECTION_QSFP_BOT_10G_PORT1 , &dfe_ip, &netmask); //MAX_NET_CONNECTION_CH2_SFP1

	//deal with multiple incoming connections ie. server-like behavior

	//all sockets MUST be created before first call to max_tcp_connect or max_tcp_listen
	//zato su deo create_socket i listen razdvoijeni,

	//prvo se kreiraju socketi, pa se tek onda radi sa funkcijama CONNECT i LISTEN
	//1)
	uint16_t Nsockets = 64; //SLiC Error #446 @ toe_internal.c:2321 - Cannot create more than 64 sockets
	//define array of pointers
	max_tcp_socket_t *(dfe_socket[Nsockets]);
	uint16_t socketNumber[Nsockets];

	for(int i=0;i<Nsockets;i++)
	{
		dfe_socket[i] = max_tcp_create_socket(engine, "tcp_ISCA_QSFP_BOT_10G_PORT1");  //socket 0 //"tcp_ch2_sfp1"
		//dfe_socket[i] = max_tcp_create_socket_with_number(engine, "tcp_ch2_sfp1",i);  //socket 0
		socketNumber[i] = max_tcp_get_socket_number(dfe_socket[i]);
		printf("Dodeljeni broj socket%d=%u\n",i,socketNumber[i]);
	}

	//exit(0);
	//getchar();

	//2) listen for incoming connections (from remote_host -> to DFE)
	for(int i=0;i<Nsockets;i++)
	{
		max_tcp_listen(dfe_socket[i], port);
		max_tcp_await_state(dfe_socket[i], MAX_TCP_STATE_LISTEN, NULL); //ceka dok tcp SOCKET ne dodje u LISTEN connection state
	}


	int cpu_socket = create_cpu_tcp_socket(); //otvara CPU socket //&dfe_ip, port

	//connect to the remote ip
	struct sockaddr_in cpu;
	struct in_addr remote_ip;
	remote_ip=dfe_ip;
	memset(&cpu, 0, sizeof(cpu));
	cpu.sin_family = AF_INET;
	cpu.sin_port = htons(port);
	cpu.sin_addr = remote_ip;


	//monitoring connections, and closing sockets
	uint16_t *num_changed_sockets=NULL;
	max_tcp_socket_t *changed_sockets[1];
	for(int i=0;i<Nsockets;i++)
	{
		changed_sockets[i]=NULL;
	}

	struct timeval *timeout;
	//max_tcp_select(1,dfe_socket[0],num_changed_sockets,changed_sockets,NULL);
//
//	void *read_ptr;
//	uint8_t *read_buffer;
//	max_llstream_t *read_llstream;
//	printf("Setting up output streams.\n");
//	posix_memalign ((void *) &read_buffer, 16, 16 * 1);
//	read_llstream = max_llstream_setup (engine, "toCpuDataSentFlag", 1, 16, read_buffer);
////
//	uint64_t *num_rx_bytes=malloc(sizeof(uint64_t)*1);
//
//
//	uint64_t *num_tx_bytes=malloc(sizeof(uint64_t)*1);
//	uint8_t	 *session_id=malloc(sizeof(uint8_t)*1);

//	while(1) //
//	{
//
//		max_tcp_get_num_bytes_transmitted(dfe_socket[0],num_tx_bytes,session_id);
//	    if((*num_tx_bytes)!=0) printf("Number of bytes transffered: %u\n", *num_tx_bytes);
//
//
//		uint8_t ii= max_llstream_read(read_llstream,1,&read_ptr);
//		if (ii)
//		{
//			// check data integrity
//
//			if(memcmp(read_ptr, read_ptr , 16))
//			{
//			}
//
//			uint16_t socket_returned=((disconnect_format*) read_ptr)->byte1;
//			printf("Read: %u\n", socket_returned);
//
//
//			//for(int i=0;i<Nsockets;i++)
//			{
//				//max_tcp_await_state(dfe_socket[0], MAX_TCP_STATE_ESTABLISHED, NULL);
//
//
//				//sleep(2);
//
//				printf("Closing socket=%u\n", socket_returned);
//				max_tcp_close(dfe_socket[socket_returned]);
//
//				printf("Waiting for MAX_TCP_STATE_CLOSED\n");
//				max_tcp_await_state(dfe_socket[socket_returned], MAX_TCP_STATE_CLOSED, NULL);
//				//dfe_socket[0] = max_tcp_create_socket(engine, "tcp_ISCA_QSFP_BOT_10G_PORT1");  //socket 0 //"tcp_ch2_sfp1"
//
//				printf("Set LISTEN state\n");
//				max_tcp_listen(dfe_socket[socket_returned], port);
//
//				printf("Waiting for MAX_TCP_STATE_LISTEN\n");
//				max_tcp_await_state(dfe_socket[socket_returned], MAX_TCP_STATE_LISTEN, NULL); //ceka dok tcp SOCKET ne dodje u LISTEN connection state
//
//				printf("Opened again socket=%u\n", socket_returned);
//			}
//
//
//			max_llstream_read_discard(read_llstream,1);
//
//		}
//
//	}

	uint64_t frameCounter=0; //
	uint64_t frameCounterPrevious=0;

	void *read_ptr;
	uint8_t *read_buffer;
	max_llstream_t *read_llstream;
	uint64_t *byteNumber;
	printf("Setting up output streams.\n");
	posix_memalign ((void *) &read_buffer, 16, 16 * 1);
	read_llstream = max_llstream_setup (engine, "toCpuByteNumber", 1, 16, read_buffer);




	size_t txMgr_buffer_size = 512 * sizeof(tx_event_type_t);
	void *txMgr_buffer;
	posix_memalign(&txMgr_buffer, 4096, txMgr_buffer_size);
	max_llstream_t *llstream = max_llstream_setup(engine, "toCpuOutputTxEvent", 512, sizeof(tx_event_type_t), txMgr_buffer);
	void *txMgrSlot;
	tx_event_type_t *event;

	while(1){

		//part 1: receive fileLength byteNumber

		int FoundByteNumber=0;
		while(FoundByteNumber!=1) //first wait to receive LengthBytes number
		{
			uint8_t ii= max_llstream_read(read_llstream,1,&read_ptr);
			if (ii)
			{
				byteNumber=(uint64_t*) read_ptr;
				printf("CPU byteNumber=%u\n", *byteNumber);
				max_llstream_read_discard(read_llstream,1);
				FoundByteNumber=1;
			}
		}

		printf("PART 2\n");

		//part 2: receive total number of data transfered


		int exitFlagw=0;
		while(exitFlagw==0)
		{
			while (max_llstream_read(llstream, 1, &txMgrSlot)==0);

			//while (max_llstream_read(llstream, 1, &txMgrSlot)==1) {
			event = txMgrSlot;
			printf("Got ack from TxManager: Total frames: %ld\n", event->frameCount);
			max_llstream_read_discard(llstream, 1);
			//frameCounter+=1;
			frameCounter=(event->frameCount) + 1;
			printf("frameCounter=%ld\n", frameCounter);

			double tt=(*byteNumber)/8.0;
			uint64_t y=CEILING_POS(tt);
			if((frameCounter-frameCounterPrevious)==y)
			{

				sleep(2);
				printf("frameCounter=%ld, frameCounterPrevious=%ld, ceil(fileLength)=%u\n", frameCounter, frameCounterPrevious, y);

				uint16_t socket_returned=event->socketID;
				printf("Closing socket=%u\n", socket_returned);
				max_tcp_close(dfe_socket[socket_returned]);
				//max_tcp_close_mode_t close_mode=MAX_TCP_CLOSE_ABORT_RESET;
				//max_tcp_close_advanced(dfe_socket[socket_returned],close_mode);

				printf("Waiting for MAX_TCP_STATE_CLOSED\n");
				max_tcp_await_state(dfe_socket[socket_returned], MAX_TCP_STATE_CLOSED, NULL);
				//dfe_socket[0] = max_tcp_create_socket(engine, "tcp_ISCA_QSFP_BOT_10G_PORT1");  //socket 0 //"tcp_ch2_sfp1"

				printf("Set LISTEN state\n");
				max_tcp_listen(dfe_socket[socket_returned], port);

				printf("Waiting for MAX_TCP_STATE_LISTEN\n");
				max_tcp_await_state(dfe_socket[socket_returned], MAX_TCP_STATE_LISTEN, NULL); //ceka dok tcp SOCKET ne dodje u LISTEN connection state

				printf("Opened again socket=%u\n", socket_returned);

				frameCounterPrevious=frameCounter;
				exitFlagw=1;
			}
		}
	}

//	while(1)
//	{
//		//for(int i=0;i<Nsockets;i++)
//		{
//			max_tcp_await_state(dfe_socket[0], MAX_TCP_STATE_ESTABLISHED, NULL);
//
//			while(nijevracenpodataksaDFE);
//
//
//			//sleep(2);
//
//			max_tcp_close(dfe_socket[0]);
//			max_tcp_await_state(dfe_socket[0], MAX_TCP_STATE_CLOSED, NULL);
//			//dfe_socket[0] = max_tcp_create_socket(engine, "tcp_ISCA_QSFP_BOT_10G_PORT1");  //socket 0 //"tcp_ch2_sfp1"
//			max_tcp_listen(dfe_socket[0], port);
//			max_tcp_await_state(dfe_socket[0], MAX_TCP_STATE_LISTEN, NULL); //ceka dok tcp SOCKET ne dodje u LISTEN connection state
//		}
//	}

//	while(1)
//	{
//	 //void max_tcp_select(uint16_t num_sockets,const max_tcp_socket_t *sockets[],uint16_t *num_changed_sockets,max_tcp_socket_t *changed_sockets[],struct timeval *timeout)
//	 max_tcp_select(Nsockets,dfe_socket,num_changed_sockets,changed_sockets,NULL);
//	 sleep(50);
//
//	 for(int i=0;i<(*num_changed_sockets);i++)
//	 {
//		 max_tcp_close(changed_sockets[i]);
//		 max_tcp_listen(changed_sockets[i], port);
//		 max_tcp_await_state(changed_sockets[i], MAX_TCP_STATE_LISTEN, NULL); //ceka dok tcp SOCKET ne dodje u LISTEN connection state
//		 max_tcp_select(Nsockets,dfe_socket,num_changed_sockets,changed_sockets,NULL);
//	 }
//	}

	getchar(); //block c code execution; when you want touse external tcp application
	//comnnet this line, in case you want to work with external TCP client application eg. Firefox or something else
    int vrednost = connect(cpu_socket, (const struct sockaddr*) &cpu, sizeof(cpu));

	//printf("pauza: vrednost connect=%d\n",vrednost);

	//uint8_t items1[] = { 32, 15, 16,78 ,98}; //uint8_t
	//exchangeItems(cpu_socket, items1, 255);
//	uint8_t items1[] = {
//			'G', //1
//			'E', //2
//			'T', //3
//			' ', //4
//			'/', //5
//			'i', //6
//			'n', //7
//			'd', //8 //#1 word
//	};

//    uint8_t items1[] = {
//    		13, //1
//    		10, //2
//    		13, //3
//    		10, //4
//    		13, //5
//    		10, //6
//    		13, //7
//    		10, //8 //#1 word
//    		13, //1
//    		10, //2
//    		13, //3
//    		10, //4
//    		13, //5
//    		10, //6
//    		13, //7
//    		10, //8 //#1 w
//    };

    uint8_t items1[] = {
    		'G', //1
    		'E', //2
    		'T', //3
    		' ', //4
    		'/', //5
    		'i', //6
    		'n', //7
    		'd', //8 //#1 word

    		'e', //1
    		'x', //2
    		'1', //3
    		'.', //4
    		'h', //5
    		't', //6
    		'm', //7
    		'l', //8 //#1 word

    		' ', //1
    		'H', //2
    		'T', //3
    		'T', //4
    		'P', //5
    		'/', //6
    		'1', //7
    		'.', //8 //#1 word

    		13, //8 //#1 word
    		10, //8 //#1 word
    		13, //8 //#1 word
    		10, //8 //#1 word

    	};


	//for(unsigned int i=0;i<sizeof(items1);i++)
	{
		exchangeItems(cpu_socket, items1, sizeof(items1));
		//printf("Prolaz %d\n",i);
	}

	//uint8_t
	//exchangeItems(cpu_socket, items1, sizeof(items1));
	//exchangeItems(cpu_socket, items1,sizeof(items1));
	printf("Prva petlja \n");

	uint8_t items2[] = {
			'G', //1
			'E', //2
			'T', //3
			' ', //4				//pocetak URIja
			'/', //5
			'i', //6
			'n', //7
			'd', //8 //#1 word

			'e', //1
			'x', //2
			'.', //3
			'h', //4
			't', //5
			'm', //6
			'l', //7
			' ', //8 //#2 word		//kraj URIja

			'H', //1
			'T', //2
			'T', //3
			'P', //4
			'/', //5
			'1', //6
			'.', //7
			'0', //8 //#3 word

			 0,  //1
			 0,  //2 //#4 consist of 2 bytes only, blank-line
			'1', //3 //something like headers
			'2', //4
			'3', //5
			'4', //6
			13,  //7
			10,  //8

			13,  //1
			10,  //2 //#4 word
			255, //3
			255, //4 //#4 consist of 2 bytes only, blank-line
			255, //5 //something like headers
			255, //6
			255, //7
			255  //8
	};
	//	for(unsigned int i=0;i<sizeof(items2);i++)
	//	{

	//printf("**** getchar(): Enter key:\n");
	//getchar();

	//exchangeItems(cpu_socket, items2, sizeof(items2));

	//static const
	uint8_t items5[] = {
	            0x47, 0x45, 0x54, 0x20, 0x2f, 0x20, /* GET /  */
	0x48, 0x54, 0x54, 0x50, 0x2f, 0x31, 0x2e, 0x31, /* HTTP/1.1 */
	0x0d, 0x0a, 0x48, 0x6f, 0x73, 0x74, 0x3a, 0x20, /* ..Host:  */
	0x31, 0x32, 0x37, 0x2e, 0x30, 0x2e, 0x30, 0x2e, /* 127.0.0. */
	0x31, 0x0d, 0x0a, 0x55, 0x73, 0x65, 0x72, 0x2d, /* 1..User- */
	0x41, 0x67, 0x65, 0x6e, 0x74, 0x3a, 0x20, 0x4d, /* Agent: M */
	0x6f, 0x7a, 0x69, 0x6c, 0x6c, 0x61, 0x2f, 0x35, /* ozilla/5 */
	0x2e, 0x30, 0x20, 0x28, 0x58, 0x31, 0x31, 0x3b, /* .0 (X11; */
	0x20, 0x4c, 0x69, 0x6e, 0x75, 0x78, 0x20, 0x78, /*  Linux x */
	0x38, 0x36, 0x5f, 0x36, 0x34, 0x3b, 0x20, 0x72, /* 86_64; r */
	0x76, 0x3a, 0x33, 0x31, 0x2e, 0x30, 0x29, 0x20, /* v:31.0)  */
	0x47, 0x65, 0x63, 0x6b, 0x6f, 0x2f, 0x32, 0x30, /* Gecko/20 */
	0x31, 0x30, 0x30, 0x31, 0x30, 0x31, 0x20, 0x46, /* 100101 F */
	0x69, 0x72, 0x65, 0x66, 0x6f, 0x78, 0x2f, 0x33, /* irefox/3 */
	0x31, 0x2e, 0x30, 0x0d, 0x0a, 0x41, 0x63, 0x63, /* 1.0..Acc */
	0x65, 0x70, 0x74, 0x3a, 0x20, 0x74, 0x65, 0x78, /* ept: tex */
	0x74, 0x2f, 0x68, 0x74, 0x6d, 0x6c, 0x2c, 0x61, /* t/html,a */
	0x70, 0x70, 0x6c, 0x69, 0x63, 0x61, 0x74, 0x69, /* pplicati */
	0x6f, 0x6e, 0x2f, 0x78, 0x68, 0x74, 0x6d, 0x6c, /* on/xhtml */
	0x2b, 0x78, 0x6d, 0x6c, 0x2c, 0x61, 0x70, 0x70, /* +xml,app */
	0x6c, 0x69, 0x63, 0x61, 0x74, 0x69, 0x6f, 0x6e, /* lication */
	0x2f, 0x78, 0x6d, 0x6c, 0x3b, 0x71, 0x3d, 0x30, /* /xml;q=0 */
	0x2e, 0x39, 0x2c, 0x2a, 0x2f, 0x2a, 0x3b, 0x71, /* .9,*/ /*;q */
	0x3d, 0x30, 0x2e, 0x38, 0x0d, 0x0a, 0x41, 0x63, /* =0.8..Ac */
	0x63, 0x65, 0x70, 0x74, 0x2d, 0x4c, 0x61, 0x6e, /* cept-Lan */
	0x67, 0x75, 0x61, 0x67, 0x65, 0x3a, 0x20, 0x65, /* guage: e */
	0x6e, 0x2d, 0x55, 0x53, 0x2c, 0x65, 0x6e, 0x3b, /* n-US,en; */
	0x71, 0x3d, 0x30, 0x2e, 0x35, 0x0d, 0x0a, 0x41, /* q=0.5..A */
	0x63, 0x63, 0x65, 0x70, 0x74, 0x2d, 0x45, 0x6e, /* ccept-En */
	0x63, 0x6f, 0x64, 0x69, 0x6e, 0x67, 0x3a, 0x20, /* coding:  */
	0x67, 0x7a, 0x69, 0x70, 0x2c, 0x20, 0x64, 0x65, /* gzip, de */
	0x66, 0x6c, 0x61, 0x74, 0x65, 0x0d, 0x0a, 0x43, /* flate..C */
	0x6f, 0x6e, 0x6e, 0x65, 0x63, 0x74, 0x69, 0x6f, /* onnectio */
	0x6e, 0x3a, 0x20, 0x6b, 0x65, 0x65, 0x70, 0x2d, /* n: keep- */
	0x61, 0x6c, 0x69, 0x76, 0x65, 0x0d, 0x0a, 0x49, /* alive..I */
	0x66, 0x2d, 0x4d, 0x6f, 0x64, 0x69, 0x66, 0x69, /* f-Modifi */
	0x65, 0x64, 0x2d, 0x53, 0x69, 0x6e, 0x63, 0x65, /* ed-Since */
	0x3a, 0x20, 0x54, 0x75, 0x65, 0x2c, 0x20, 0x31, /* : Tue, 1 */
	0x37, 0x20, 0x4a, 0x75, 0x6e, 0x20, 0x32, 0x30, /* 7 Jun 20 */
	0x31, 0x34, 0x20, 0x31, 0x36, 0x3a, 0x30, 0x30, /* 14 16:00 */
	0x3a, 0x34, 0x37, 0x20, 0x47, 0x4d, 0x54, 0x0d, /* :47 GMT. */
	0x0a, 0x49, 0x66, 0x2d, 0x4e, 0x6f, 0x6e, 0x65, /* .If-None */
	0x2d, 0x4d, 0x61, 0x74, 0x63, 0x68, 0x3a, 0x20, /* -Match:  */
	0x22, 0x31, 0x33, 0x31, 0x30, 0x2d, 0x34, 0x66, /* "1310-4f */
	0x63, 0x30, 0x61, 0x33, 0x66, 0x33, 0x32, 0x61, /* c0a3f32a */
	0x39, 0x63, 0x30, 0x22, 0x0d, 0x0a, 0x43, 0x61, /* 9c0"..Ca */
	0x63, 0x68, 0x65, 0x2d, 0x43, 0x6f, 0x6e, 0x74, /* che-Cont */
	0x72, 0x6f, 0x6c, 0x3a, 0x20, 0x6d, 0x61, 0x78, /* rol: max */
	0x2d, 0x61, 0x67, 0x65, 0x3d, 0x30, 0x0d, 0x0a, /* -age=0.. */
	0x0d, 0x0a										/* .. */
	};

	printf("sizeof(items) = %u\n", sizeof(items5));
	//getchar();

	//exchangeItems(cpu_socket, items5, sizeof(items5));
	//		printf("Prolaz %d\n",i);
	//	}

	printf("Druga petlja \n");

	uint8_t items3[] = {
			'H', //1
			'T', //2
			'T', //3
			'P', //4
			'/', //5
			'1', //6
			'.', //7
			'0', //8 //#3 word
	};

	printf("Treca petlja \n");

	//exchangeItems(cpu_socket, items3, sizeof(items3));

	uint8_t items4[] = {
			'1', //1
			'2', //2
			'3', //3
			'4', //4
			'5', //5
			'6', //6
			13,  //7
			10,  //8 //#4 word
	};

	printf("Cetvrta petlja \n");

	//exchangeItems(cpu_socket, items4, sizeof(items4));

	//uint8_t items2[] = { 50, 2 };
	//exchangeItems(cpu_socket, items2, 2);

	//uint8_t items3[] = { 1, 2, 3, 4, 5, 6 };
	//exchangeItems(cpu_socket, items3, sizeof(items3));

	close(cpu_socket);
	printf("close(cpu_socket)\n");

	for(int i=0;i<Nsockets;i++)
	{
		max_tcp_close(dfe_socket[i]);
		printf("max_tcp_close(dfe_socket[i])");
	}

	max_unload(engine);
	printf("max_unload(engine)");
	max_file_free(maxfile);
	printf("max_file_free(maxfile)");

	//getchar(); //ne radi kako je ocekivano

	return 0;

	printf("KRAJ\n");
}
