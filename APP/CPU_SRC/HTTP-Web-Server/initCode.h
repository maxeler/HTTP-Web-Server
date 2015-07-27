#ifndef INITCODE_H_INCLUDED
#define INITCODE_H_INCLUDED

typedef struct {
	uint32_t start_address_burst:19;
	uint32_t file_length_bursts:19;
	uint32_t file_length_bytes:26;
} StructRomCrcIndexData;

uint64_t reverse_bytes(uint64_t value);
int init_code(char* cdir);

#endif  // INITCODE_H_INCLUDED
