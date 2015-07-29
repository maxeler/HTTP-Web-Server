#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <sys/wait.h>
#include <signal.h>
#include "init.h"

int get_request(int new_fd, char *buffer_x)
{
    char *buffer = buffer_x;
    int num_bytes_to_read = 1;
    char c;

    while (1) {
        // read line
        // This function returns up to len bytes of data. If there are fewer than the requested number of bytes available, the function returns the number currently available.
        int a = recv(new_fd, &c, num_bytes_to_read, 0);
        *buffer = c;
        buffer++;
        if (c == '\n') {
            break; // exit for loop
        }

        // Return Value
        // These calls return the number of bytes received, or -1 if an error occurred. The return value will be 0 when the peer has performed an orderly shutdown.
        if (a < 0) {
            exit(-1);
        } else if (a == 0) {

        } else if (a == num_bytes_to_read) {
            printf("From client: %c\n", c);
        }

    }

    *buffer = 0; // end of string
    return 0; // return from the function to main program
}

void parse_request(char *buffer, struct Element *crc_table, int new_fd, char* path, unsigned int *crc_page_not_found)
{
    if (!strncmp(buffer, "GET ", 4)) {
        char *start_uri_pos, *end_uri_pos;
        printf("input: GET request\n");
        buffer += 4;
        start_uri_pos = buffer;
        end_uri_pos = strchr(buffer, ' '); // get position of the ending URI blank space: GET -url- HTTP/1.0
        printf("startPos: character=%c, %p, endPos: character=%c, %p\n", *start_uri_pos, start_uri_pos, *end_uri_pos, end_uri_pos);
        int url1_length;
        char *url1;
        url1_length = end_uri_pos - start_uri_pos - 1;
        url1 = (char*) calloc(url1_length, sizeof (char));
        strncpy(url1, buffer + 1, url1_length);
        printf("URL: %s\n", url1);

        int url_length = url1_length;
        char *url = (char *) calloc(url_length + 1, sizeof (char));
        strcat(url, url1);
        printf("PATH, URL: %s, %s\n", path, url1);
        printf("combined: %s, urlLength=%d\n", url, url_length);
        // exit(0);

        unsigned int crc_value = calculate_crc16(url, url_length);
        printf("crc_value of URL: %d\n", crc_value);
        printf("crc_table[cc_value=%d].pointer=%d\n", crc_value, crc_table[crc_value].pointer);
        printf("crc_table[crc_value=%d].sizeBytes=%d\n", crc_value, crc_table[crc_value].size_bytes);
        
        // check if resource exist on the server
        if (crc_table[crc_value].pointer != 0) {
            // get the resource in the RAM and send it back to a client
            printf("The resource '%s' EXISTS in the RAM\n", url);
            // ssize_t send(int sockfd, const void *buf, size_t len, int flags);
            int bytes_sent = send(new_fd, crc_table[crc_value].pointer, crc_table[crc_value].size_bytes, 0);
            printf("bytesSent: %d\n", bytes_sent);
        } else {
            printf("The resource '%s' does not exist in the RAM\nSending default page to the client\n", url);
            int bytes_sent = send(new_fd, crc_table[*crc_page_not_found].pointer, crc_table[*crc_page_not_found].size_bytes, 0);
            exit(-3);
        }

    };
}
