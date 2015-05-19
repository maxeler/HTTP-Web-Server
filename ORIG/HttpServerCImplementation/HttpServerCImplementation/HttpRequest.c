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


int GET_Request(int new_fd, char *buffer_x)
{
    int MaxReqLineLength =1024;
    char *buffer= buffer_x;
    int NBytesToRead = 1;
    char c;
    int i;

    for(i=1; i<MaxReqLineLength; i++)
    {
        // read line
        // This function returns up to len bytes of data. If there are fewer than the requested number of bytes available, the function returns the number currently available.
        int a = recv(new_fd, &c , NBytesToRead, 0);
        *buffer=c;
        buffer++;
        //if (c=='\n')
        if (c=='X')
        {
            break; // exit for loop
        }

        // Return Value
        // These calls return the number of bytes received, or -1 if an error occurred. The return value will be 0 when the peer has performed an orderly shutdown.
        if (a<0)
        {
            exit(-1);
        }
        else if (a==0)
        {

        }
        else if(a==NBytesToRead)
        {
            printf("From server: %c\n",c);
        }
    }

    *buffer=0; // end of string
    return i;  // return from the function to main program
}

void Parse_Request(char *buffer)
{
    if(!strncmp(buffer, "GET ", 4))
    {
        char *startUriPos, *endUriPos;
        printf("input: GET request\n");
        buffer+=4;
        startUriPos=buffer;
        endUriPos=strchr(buffer,' '); // get position of the ending URI blank space: GET -url- HTTP/1.0
        printf("startPos: character=%c, %p, endPos: characher=%c, %p\n", *startUriPos, startUriPos, *endUriPos, endUriPos);
        int urlLength;
        char *url;
        urlLength=endUriPos-startUriPos;
        url = (char*) calloc(urlLength, sizeof(char));
        strncpy(url,buffer,urlLength);
        printf("URL: %s\n", url);

        // current crc value
        // calculate crc of URL and check if resource exist on the server
//        // crcCurrent=crc16(url);
//        if (crcTable[crcCurrent]!=0)
//        {
//            // get the resource in the RAM and send it back to a client
//            char *pntReq = crcTable[crcCurrent]; // get the pointer that points to the file in LMEM
//            //file length
//
//            send(conn, pntReq, number_of_bytes_to_send, 0);
//        }
//        else
//        {
//            printf("For the given CRC value there is no resource in the RAM");
//            exit(-3);
//        }

        buffer=endUriPos+1;
        if(!strncmp(buffer, "HTTP/1.", 7))
        {
            printf("HTTP part version 1: %s", buffer);
        }
        if(!strncmp(buffer, "HTTP/0.", 7))
        {
            printf("HTTP part version 0: %s", buffer);
        }

    };
}
