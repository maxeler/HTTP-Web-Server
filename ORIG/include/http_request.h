#ifndef HTTPREQUEST_H_INCLUDED
#define HTTPREQUEST_H_INCLUDED

#include "element.h"

int get_request(int new_fd, char *buffer_x);
void parse_request(char *buffer, struct Element *crc_table, int new_fd, char* path, unsigned int *crc_page_not_found);

#endif // HTTPREQUEST_H_INCLUDED
