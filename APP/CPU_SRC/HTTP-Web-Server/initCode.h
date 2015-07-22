#ifndef INITCODE_H_INCLUDED
#define INITCODE_H_INCLUDED

typedef struct {
	uint32_t startAddressBurst:19;
	uint32_t fileLengthBursts:19;
	uint32_t fileLengthBytes:26;
} StructRomCrcIndexData;

uint64_t reverseBytes(uint64_t value);
int initCode(char* cdir);

#endif  // INITCODE_H_INCLUDED
