#ifndef HTTPREQUEST_H_INCLUDED
#define HTTPREQUEST_H_INCLUDED

struct Element {
    char* pointer;
    int sizeBytes;
};

unsigned int calculateCRC16 (unsigned char *Data, unsigned char len);
int initCode(struct Element *crcTable, char* cdir);

#endif // HTTPREQUEST_H_INCLUDED

