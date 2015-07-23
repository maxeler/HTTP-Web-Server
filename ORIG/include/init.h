#ifndef INIT_H_INCLUDED
#define INIT_H_INCLUDED

#include "element.h"

unsigned int calculate_crc16(unsigned char *Data, unsigned char len);
int init_code(struct Element *crc_table, char* cdir, unsigned int *crc_page_not_found);

#endif  // INIT_H_INCLUDED

