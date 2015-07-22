/*
File: main.c
Description: Generates LMEM and CRC index tables initialization files
Inputs: folder with files to be stored in the LMEM, 'cdir' variable should contain the folder path
Outputs: generated files - lmem_generated_file.html, romCrcIndex1_init.html, romCrcIndex2_init.html
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

#include "crc16.h"

#define N_crcindex 32768

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

int initCode(char* cdir)
{
	unsigned char buf[64];

	static uint16_t crc_index[649];
	static uint64_t romCrcIndex1[N_crcindex];
	static uint64_t romCrcIndex2[N_crcindex];
	StructRomCrcIndexData memory_location;
	char lmem_generated_file[] = "./results/lmem_generated_file.html";
	int pass_rom1 = 0, pass_rom2 = 0;

	char romCrcIndex1_init[] = "./results/romCrcIndex1_init.html";
	char romCrcIndex2_init[] = "./results/romCrcIndex2_init.html";

	FILE *fp_lmem = fopen (lmem_generated_file,"wb");
	if (!fp_lmem)
	{
		printf("Error with file\n");
		exit(0);
	}

	FILE *fp_romCrcIndex1 = fopen (romCrcIndex1_init,"wb");
	if (!fp_romCrcIndex1)
	{
		printf("Error with file\n");
		exit(0);
	}

	FILE *fp_romCrcIndex2 = fopen (romCrcIndex2_init,"wb");
	if (!fp_romCrcIndex2)
	{
		printf("Error with file\n");
		exit(0);
	}

	DIR *dp;
	struct dirent *ep;
	char fname[512] = "";
	char fnamecopy[512] = "";
        char fnamefile[512] = "";
	char fsize[64] = "";
	struct stat st;

	dp = opendir (cdir);
	if (dp == NULL) {
		perror ("Couldn't open the directory");
		exit(-4);
	}

	int i = 0;
        int currentBurst = 0;
        while ((ep = readdir(dp))) {
            memset(buf, 0, sizeof (buf));
            /* browse through the files in current directory and gets file attributes */
            int scompare1 = strcmp(ep->d_name, ".");
            int scompare2 = strcmp(ep->d_name, "..");
            if (scompare1 && scompare2) {
                strcat(fnamefile, cdir);
                strcat(fnamefile, ep->d_name);            
                strcat(fname, "/");                
                strcat(fname, ep->d_name);
                stat(fnamefile, &st);
                
                int size = st.st_size;
                sprintf(fsize, " - size(bytes): %d", size);
                strcpy(fnamecopy, fname);
                strcat(fnamecopy, fsize);
                puts(fnamecopy);
 
                unsigned char *t1;
                char *t2;
                t1 = buf;
                t2 = fname;

                //copy string to another string
                while ((*t2) != '\0') {
                    *t1 = *t2;
                    t1++;
                    t2++;
                }

                //char array reverse
                unsigned int c;
                char t;
                int n = sizeof (buf);
                int end = n - 1;
                for (c = 0; c < n / 2; c++) {
                    t = buf[c];
                    buf[c] = buf[end];
                    buf[end] = t;
                    end--;
                }

                crc_index[i] = calculateCRC16(buf, sizeof (buf)); //data from file

                /* populate romCrcIndex tables with file length, burst length and start burst */
                memory_location.startAddressBurst = currentBurst; //0x7FFFF;//currentBurst;
                memory_location.fileLengthBursts = ceil(size / 192.0); //0x3FFFF; //ceil(size/192.0);
                memory_location.fileLengthBytes = size; //0x1FFFFFF;//size;

                currentBurst += memory_location.fileLengthBursts;
                uint16_t index = crc_index[i] & 0x7FFF;
                uint8_t selection_bit = (crc_index[i] >> 15) & 1;
                uint64_t data_crcindex_c = 0;
                uint64_t t1data = memory_location.startAddressBurst;
                uint64_t t2data = memory_location.fileLengthBursts;
                uint64_t t3data = memory_location.fileLengthBytes;
                data_crcindex_c |= (t1data << (64 - 19)) | (t2data << 26) | (t3data << 0);

                uint64_t data_crcindex = reverseBytes(data_crcindex_c);

                if (selection_bit) {
                    romCrcIndex2[index] = data_crcindex;
                    pass_rom2++;
                } else {
                    romCrcIndex1[index] = data_crcindex;
                    pass_rom1++;
                }

                /* generate LMem initialization file */
                char *file_content = (char *) malloc(size);

                FILE *fp_file = fopen(fnamefile, "rb");
                if (!fp_file) {
                    printf("Error with file\n");
                    exit(0);
                }

                fread(file_content, 1, size, fp_file); //load file content
                fwrite(file_content, 1, size, fp_lmem); //write file content

                static uint8_t padding[192];
                int padd_bytes = memory_location.fileLengthBursts * 192 - size;
                fwrite(padding, 1, padd_bytes, fp_lmem); //write padding data - to set position on the beginning of the next LMEM burst
                printf("Checksum value: 0x%x\n", crc_index[i]);
                printf("\n"); //
                i++;

                fname[0] = '\0'; //set empty string
                fnamefile[0] = '\0';
                fclose(fp_file);
                free(file_content);

            }
        }
        printf("Done writing file: /results/lmem_generated_file.html\n\n");

	printf("Writing file: /results/romCrcIndex1_init.html\n");
	fwrite(romCrcIndex1, sizeof(uint64_t), N_crcindex, fp_romCrcIndex1);
	printf("Done\n\n");

	printf("Writing file: /results/romCrcIndex2_init.html\n");
	fwrite(romCrcIndex2, sizeof(uint64_t), N_crcindex, fp_romCrcIndex2);
	printf("Done\n\n");

	fclose(fp_romCrcIndex1);
	fclose(fp_romCrcIndex2);
	fclose(fp_lmem);

	(void) closedir (dp);

return 0;
}

