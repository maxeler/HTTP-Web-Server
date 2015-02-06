/*
File: main.c
Author: Milan Vorkapic
Description: Generates LMEM and CRC index tables initialization files
Inputs: folder with files to be stored in the LMEM, 'cdir' variable should contain the folder path
Outputs: generated files - lmem_generated_file.html, romHashIndex1_init.html, romHashIndex2_init.html
*/


#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <stdio.h>
#include <sys/types.h>
#include <dirent.h>
#include <sys/stat.h>
#include <string.h>
#include <math.h>
#define N_hashindex 32768

typedef struct
{
    uint32_t startAddressBurst:19;
    uint32_t fileLengthBursts:19;
    uint32_t fileLengthBytes:26;
} StructRomHashIndexData;


//crc16: http://www.embeddedrelated.com/

/***** crc16.c *****/

#include "crc16.h"

unsigned int crc16(unsigned int crcValue, unsigned char newByte)
{
    unsigned char i;

    for (i = 0; i < 8; i++)
    {

        if (((crcValue & 0x8000) >> 8) ^ (newByte & 0x80))
        {
            crcValue = (crcValue << 1)  ^ POLYNOM;
        }
        else
        {
            crcValue = (crcValue << 1);
        }

        newByte <<= 1;
    }

    return crcValue;
}

//exampleOfUseCRC16: http://www.embeddedrelated.com/

/***** EXAMPLE *****/

unsigned int exampleOfUseCRC16 (unsigned char *Data, unsigned char len)
{

    //unsigned int crc;
    uint16_t crc;
    unsigned char aux = 0;

    //crc = 0x0000; //Initialization of crc to 0x0000 for DNP
    crc = 0xFFFF; //Initialization of crc to 0xFFFF for CCITT


    while (aux < len)
    {
        crc = crc16(crc,Data[aux]);
        aux++;
    }

    //return (~crc); //The crc value for DNP it is obtained by NOT operation

    return crc; //The crc value for CCITT
}

uint64_t reverseBytes(uint64_t value)
{
    return
        (value & 0xFF) << 56 |
        (value >> 8 & 0xFF) << 48 |
        (value >> 16 & 0xFF) << 40 |
        (value >> 24 & 0xFF) << 32 |
        (value >> 32 & 0xFF) << 24 |
        (value >> 40 & 0xFF) << 16 |
        (value >> 48 & 0xFF) << 8 |
        (value >> 56 & 0xFF);
}


int main (void)
{
    unsigned char buf[64];

    static uint16_t crc_hash[649];
    static uint64_t romHashIndex1[N_hashindex];
    static uint64_t romHashIndex2[N_hashindex];
    StructRomHashIndexData memory_location;
    char lmem_generated_file[]="./lmem_generated_file.html";
    int pass_rom1=0, pass_rom2=0;

    char romHashIndex1_init[]="./romHashIndex1_init.html";
    char romHashIndex2_init[]="./romHashIndex2_init.html";

    FILE *fp_lmem = fopen (lmem_generated_file,"wb");
    if (!fp_lmem)
    {
        printf("Error with file\n");
        exit(0);
    }

    FILE *fp_romHashIndex1 = fopen (romHashIndex1_init,"wb");
    if (!fp_romHashIndex1)
    {
        printf("Error with file\n");
        exit(0);
    }

    FILE *fp_romHashIndex2 = fopen (romHashIndex2_init,"wb");
    if (!fp_romHashIndex2)
    {
        printf("Error with file\n");
        exit(0);
    }

    DIR *dp;
    struct dirent *ep;
    char fname[512]="";
    char fnamecopy[512]="";
    char fsize[64]="";
    char cdir[256]="./images1/"; //name of the current directory
    struct stat st;

    dp = opendir (cdir);
    if (dp != NULL)
    {
        int i=0;
        int currentBurst=0;
        while (ep = readdir (dp))
        {

            memset(buf,0,sizeof(buf));
            /* browse through the files in current directory and gets file attributes */
            int scompare1=strcmp(ep->d_name,".");
            int scompare2=strcmp(ep->d_name,"..");
            if(scompare1 && scompare2)
            {
                strcat(fname,cdir);
                strcat(fname,ep->d_name);
                stat(fname, &st);
                int size = st.st_size;
                sprintf(fsize," - size(bytes): %d",size);
                strcpy(fnamecopy,fname);
                strcat(fnamecopy,fsize);
                puts (fnamecopy);

                unsigned char *t1,*t2;
                t1=buf;
                t2=fname+1;

                //copy string to another string without first character
                while((*t2)!='\0')
                {
                    *t1=*t2;
                    t1++;
                    t2++;
                }

                //char array reverse
                unsigned int c;
                char t;
                int n = sizeof(buf);
                int end= n-1;
                for (c = 0; c < n/2; c++)
                {
                    t = buf[c];
                    buf[c] = buf[end];
                    buf[end] = t;
                    end--;
                }

                crc_hash[i]=exampleOfUseCRC16 (buf, sizeof(buf)); //data from file

                /* populate romHashIndex tables with file length, burst length and start burst */
                memory_location.startAddressBurst = currentBurst; //0x7FFFF;//currentBurst;
                memory_location.fileLengthBursts = ceil(size/192.0); //0x3FFFF; //ceil(size/192.0);
                memory_location.fileLengthBytes = size; //0x1FFFFFF;//size;

                currentBurst+=memory_location.fileLengthBursts;
                uint16_t index = crc_hash[i] & 0x7FFF;
                uint8_t selection_bit = (crc_hash[i]>>15) & 1;
                uint64_t data_hashindex_c=0;
                uint64_t t1data = memory_location.startAddressBurst;
                uint64_t t2data = memory_location.fileLengthBursts;
                uint64_t t3data = memory_location.fileLengthBytes;
                data_hashindex_c |= (t1data << (64-19)) | (t2data << 26) | (t3data << 0);

                uint64_t data_hashindex = reverseBytes(data_hashindex_c);

                if(selection_bit)
                {
                    romHashIndex2[index]=data_hashindex;
                    pass_rom2++;
                }
                else
                {
                    romHashIndex1[index]=data_hashindex;
                    pass_rom1++;
                }

                /* generate LMem initialization file */
                char *file_content = (char *) malloc(size);

                FILE *fp_file = fopen (fname,"rb");
                if (!fp_file)
                {
                    printf("Error with file\n");
                    exit(0);
                }

                fread(file_content, 1, size, fp_file);   //load file content
                fwrite(file_content, 1, size, fp_lmem); //write file content

                static uint8_t padding[192];
                int padd_bytes = memory_location.fileLengthBursts*192-size;
                fwrite(padding, 1, padd_bytes, fp_lmem); //write padding data - to set position on the beginning of the next LMEM burst
                printf("Checksum value: 0x%x\n", crc_hash[i]);
                i++;

                fname[0]='\0'; //set empty string
                fclose(fp_file);
                free(file_content);

            }
        }

        fwrite(romHashIndex1, sizeof(uint64_t), N_hashindex, fp_romHashIndex1);
        fwrite(romHashIndex2, sizeof(uint64_t), N_hashindex, fp_romHashIndex2);
        fclose(fp_romHashIndex1);
        fclose(fp_romHashIndex2);
        fclose(fp_lmem);

        (void) closedir (dp);
    }
    else
        perror ("Couldn't open the directory");

    return 0;
}

