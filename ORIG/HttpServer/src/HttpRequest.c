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

#include "../include/init.h"

int GET_Request(int new_fd, char *buffer_x) {
    char *buffer = buffer_x;
    int NBytesToRead = 1;
    char c;

    while (1) {
        // read line
        // This function returns up to len bytes of data. If there are fewer than the requested number of bytes available, the function returns the number currently available.
        int a = recv(new_fd, &c, NBytesToRead, 0);
        *buffer = c;
        buffer++;
        if (c == '\n')
            //if (c == 'X') 
        {
            break; // exit for loop
        }

        // Return Value
        // These calls return the number of bytes received, or -1 if an error occurred. The return value will be 0 when the peer has performed an orderly shutdown.
        if (a < 0) {
            exit(-1);
        } else if (a == 0) {

        } else if (a == NBytesToRead) {
            printf("From client: %c\n", c);
        }

    }

    *buffer = 0; // end of string
    return 0; // return from the function to main program
}

void Parse_Request(char *buffer, struct Element *crcTable, int new_fd) {
    if (!strncmp(buffer, "GET ", 4)) {
        char *startUriPos, *endUriPos;
        printf("input: GET request\n");
        buffer += 4;
        startUriPos = buffer;
        endUriPos = strchr(buffer, ' '); // get position of the ending URI blank space: GET -url- HTTP/1.0
        printf("startPos: character=%c, %p, endPos: character=%c, %p\n", *startUriPos, startUriPos, *endUriPos, endUriPos);
        int urlLength;
        char *url;
        urlLength = endUriPos - startUriPos;
        url = (char*) calloc(urlLength, sizeof (char));
        strncpy(url, buffer, urlLength);
        printf("URL: %s\n", url);

        unsigned int crcValue = exampleOfUseCRC16(url, urlLength);
        printf("crcValue of URL: %d\n", crcValue);
        printf("crcTable[crcValue=%d].pointer=%d\n", crcValue, crcTable[crcValue].pointer);
        printf("crcTable[crcValue=%d].sizeBytes=%d\n", crcValue, crcTable[crcValue].sizeBytes);

        // check if resource exist on the server
        if (crcTable[crcValue].pointer != 0) {
            // get the resource in the RAM and send it back to a client
            printf("The resource '%s' EXISTS in the RAM\n", url);
            // ssize_t send(int sockfd, const void *buf, size_t len, int flags);
            int bytesSent = send(new_fd, crcTable[crcValue].pointer, crcTable[crcValue].sizeBytes, 0);
            printf("bytesSent: %d\n", bytesSent);
        } else {
            int crcPageNotFound = 45118;
            printf("The resource '%s' does not exist in the RAM\nSending default page to the client\n", url);
            int bytesSent = send(new_fd, crcTable[crcPageNotFound].pointer, crcTable[crcPageNotFound].sizeBytes, 0);
            exit(-3);
        }

    };
}
