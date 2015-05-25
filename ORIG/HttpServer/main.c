// http://beej.us/net2/html/clientserver.html

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

#include "HttpRequest.h"
#include "initCodeCImplementation.h"

//#define PORT "10080"  // the port server will listen on

#define BACKLOG 10    // how many pending connections queue will hold

struct Element {
    char* pointer;
    int sizeBytes;
};

void sigchld_handler(int s) {
    while (waitpid(-1, NULL, WNOHANG) > 0);
}

// get sockaddr, IPv4 or IPv6:

void *get_in_addr(struct sockaddr *sa) {
    if (sa->sa_family == AF_INET) {
        return &(((struct sockaddr_in*) sa)->sin_addr);
    }

    return &(((struct sockaddr_in6*) sa)->sin6_addr);
}

int main(int argc, char * argv[]) {
        
    if (argc != 2) {
        printf("Usage: %s <port>", argv[0]);
        exit(1);
    }

    int sockfd, new_fd; // listen on sock_fd, new connection on new_fd
    struct addrinfo hints, *servinfo, *p;
    struct sockaddr_storage their_addr; // connector's address information
    socklen_t sin_size;
    struct sigaction sa;
    int yes = 1;
    char s[INET6_ADDRSTRLEN];
    int rv;

    //char PORT[] = argv[1];

    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE; // use current IP

    if ((rv = getaddrinfo(NULL, argv[1], &hints, &servinfo)) != 0) { // PORT
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
        return 1;
    }

    // loop through all the results and bind to the first we can
    for (p = servinfo; p != NULL; p = p->ai_next) {
        if ((sockfd = socket(p->ai_family, p->ai_socktype,
                p->ai_protocol)) == -1) {
            perror("server: socket");
            continue;
        }

        if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &yes,
                sizeof (int)) == -1) {
            perror("setsockopt");
            exit(1);
        }

        if (bind(sockfd, p->ai_addr, p->ai_addrlen) == -1) {
            close(sockfd);
            perror("server: bind");
            continue;
        }

        break;
    }

    if (p == NULL) {
        fprintf(stderr, "server: failed to bind\n");
        return 2;
    }

    freeaddrinfo(servinfo); // free structure

    if (listen(sockfd, BACKLOG) == -1) {
        perror("listen");
        exit(1);
    }

    sa.sa_handler = sigchld_handler; // gather all dead processes
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = SA_RESTART;
    if (sigaction(SIGCHLD, &sa, NULL) == -1) {
        perror("sigaction");
        exit(1);
    }

    // initialize RAM
    printf("Loading hosted files into RAM\n");
    int NelTable = 65536;
    struct Element crcTable[NelTable];


    initCode(crcTable);
    printf("Loading hosted files into RAM completed\n");


    printf("Server: waiting for connections...\n");

    while (1) // accept()
    {
        sin_size = sizeof their_addr;
        new_fd = accept(sockfd, (struct sockaddr *) &their_addr, &sin_size);
        if (new_fd == -1) {
            perror("Accept");
            continue;
        }

        inet_ntop(their_addr.ss_family,
                get_in_addr((struct sockaddr *) &their_addr),
                s, sizeof s);
        printf("Server: received connection from %s\n", s);

        if (!fork()) // this is the child process
        {
            close(sockfd); // close listener inside child

            // USER CODE
            // call function to process input data

            int k = 0;
            char buffer_x[1024] = {0};
            GET_Request(new_fd, buffer_x);

            printf("Buffer: %s\n", buffer_x);
            printf("Run Parse_Request()");
            Parse_Request(buffer_x, crcTable, new_fd);

            close(new_fd);
            printf("Close child process\n");

            exit(0); // exit child process
        }
        close(new_fd); // parent
    }

    return 0;
}
