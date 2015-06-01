#ifndef INIT_H_INCLUDED
#define INIT_H_INCLUDED

#include "element.h"

unsigned int calculateCRC16(unsigned char *Data, unsigned char len);
int initCode(struct Element *crcTable, char* cdir);

#endif  // INIT_H_INCLUDED

