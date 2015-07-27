//crc16: http://www.embeddedrelated.com/showcode/295.php

#include <stdint.h>
#include "crc16.h"

unsigned int crc16(unsigned int crc_value, unsigned char new_byte) 
{
    unsigned char i;

    for (i = 0; i < 8; i++) {

        if (((crc_value & 0x8000) >> 8) ^ (new_byte & 0x80)) {
            crc_value = (crc_value << 1) ^ POLYNOM;
        } else {
            crc_value = (crc_value << 1);
        }

        new_byte <<= 1;
    }

    return crc_value;
}

unsigned int calculate_crc16(unsigned char *data, unsigned char len)
{
    uint16_t crc;
    unsigned char aux = 0;

    crc = 0xFFFF; // Initialization of crc to 0xFFFF for CCITT

    while (aux < len) {
        crc = crc16(crc, data[aux]);
        aux++;
    }

    return crc; // The crc value for CCITT
}


