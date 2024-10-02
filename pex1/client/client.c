// Wget2 client.c
// Name:
// Submission Date:

#include <arpa/inet.h> // inet_addr()
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h> // bzero()
#include <sys/socket.h>
#include <unistd.h> // read(), write(), close()
#define MAX 80
#define SA struct sockaddr

void errorCheck(char *message){
    perror(message);
    exit(1);
}


// main function
int main(int argc, char *argv[])
{
    int sockfd, port, n;
    struct sockaddr_in servaddr, cli;
    struct hostent *server;
    char *filename, *addr;

    if(argc != 4){
        fprintf(stderr, "Invalid Input: ./client hostname port filename\n");
        exit(0);
    }

    port = atoi(argv[2]);
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    filename = argv[3];
    if(sockfd < 0){
        errorCheck("Socket opening ERROR");
    }
    printf("Socket successfully created..\n");

    server = gethostbyname(argv[1]);
    if(server == NULL){
        fprintf(stderr, "No such host, exiting..");
        exit(0);
    }


    servaddr.sin_family = AF_INET;

    servaddr.sin_port = htons(port);

    if((connect(sockfd, (struct sockaddr*)&servaddr, sizeof(servaddr))) < 0){
        errorCheck("Connection ERROR");
    }
    n = send(sockfd, filename, sizeof(filename), 0);

    if(n < 0){
        errorCheck("ERROR sending file");
    }
    printf("Requesting file %s\n", filename);

    

    FILE* fileToWrite = fopen(filename, "w");
    
    char temp[MAX];
    while((n = read(sockfd, temp, MAX)) > 0){
        fwrite(temp, sizeof(char), n, fileToWrite);
    }
    if(n < 0){
        errorCheck("ERROR receiving file");
    }
    printf("File received: %s\n", filename);

    

    fclose(fileToWrite);
    close(sockfd);



    return 0;
}