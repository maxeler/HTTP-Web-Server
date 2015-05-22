#ifndef HTTPREQUEST_H_INCLUDED
#define HTTPREQUEST_H_INCLUDED

int GET_Request(int new_fd, char *buffer_x);
void Parse_Request(char *buffer, struct Element *crcTable, int new_fd);

#endif // HTTPREQUEST_H_INCLUDED
