/*
File: init.c
Description: creates CRC index table and fills RAM with content of hosted files.
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
#include "init.h"

int init_code(struct Element *crc_table, char* cdir, unsigned int *crc_page_not_found)
{
    unsigned char buf[512];
    DIR *dp;
    struct dirent *ep;
    char fname[512] = "";
    char fname_file[512] = "";
    char fname_copy[512] = "";
    char fsize[64] = "";
    struct stat st;
    char default_page_name[] = "HTTPError404NotFound.html";

    dp = opendir(cdir);
    if (dp == NULL) {
        perror("Couldn't open the directory");
        exit(-4);
    }

    int i = 0;
    while (ep = readdir(dp)) {
        memset(buf, 0, sizeof (buf));
        int jk = 0;
        // browse through the files in current directory and gets file attributes
        int scompare1 = strcmp(ep->d_name, ".");
        int scompare2 = strcmp(ep->d_name, "..");
        if (scompare1 && scompare2) {
            strcat(fname_file, cdir);
            strcat(fname_file, ep->d_name);
            strcat(fname, ep->d_name);
            stat(fname_file, &st);
            int size = st.st_size;
            sprintf(fsize, " - size(bytes): %d", size);
            strcpy(fname_copy, fname);
            strcat(fname_copy, fsize);
            puts(fname_copy);

            // HTTP response: status-line; header fields
            int num_fields = 4;
            int ip;
            char *response_fields[num_fields];
            response_fields[0] = "HTTP/1.0 200 OK\r\n"; // status line
            response_fields[1] = "Connection: close\r\n"; // header field //Close //keep-alive
            char thdr[50];
            sprintf(thdr, "Content-Length: %d\r\n", size); // header field
            response_fields[2] = thdr;
            // Content-Type: image/svg+xml\r\n
            response_fields[3] = "\r\n"; //CRLF
            char joined[512] = "";

            for (ip = 0; ip < num_fields; ip++) {
                strcat(joined, response_fields[ip]);
            }

            printf("Joined: %s\n", joined);

            int size_only_headers = strlen(joined);
            int size_with_headers = size_only_headers + size;

            // copy string to another string without first character
            unsigned char *t1, *t2;
            t1 = buf;
            t2 = fname;
            while ((*t2) != '\0') {
                *t1 = *t2;
                t1++;
                t2++;
            }

            unsigned int crc_value = calculate_crc16(buf, strlen(buf)); // get crc of the current url

            if (!strcmp(ep->d_name, default_page_name)) {
                *crc_page_not_found = crc_value;
            }

            char *file_content = (char *) malloc(size);
            char *file_content_write = (char *) malloc(size_with_headers);

            if (file_content == NULL | file_content_write == NULL) {
                printf("Error with allocating memory\n");
                exit(0);
            }

            FILE *fp_file = fopen(fname_file, "rb");
            if (!fp_file) {
                printf("Error with file\n");
                exit(0);
            }

            fread(file_content, 1, size, fp_file); // load file content
            memcpy(file_content_write, joined, size_only_headers); // copy headers
            char *tfile = file_content_write + size_only_headers; // copy file content
            memcpy(tfile, file_content, size); // combine headers with file content

            crc_table[crc_value].pointer = (char*) calloc(size_with_headers, sizeof (char));
            crc_table[crc_value].size_bytes = size_with_headers;
            memcpy(crc_table[crc_value].pointer, file_content_write, size_with_headers);

            printf("Checksum value: 0x%x, %d\n", crc_value, crc_value);
            printf("\n"); //
            i++;

            fname[0] = '\0'; // set empty string
            fname_file[0] = '\0';
            free(file_content);
            free(file_content_write);
        }
    }

    (void) closedir(dp);

    return 0;
}

