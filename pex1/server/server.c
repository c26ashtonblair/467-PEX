// Wget2 server.c
// Name:
// Submission Date:

#include <stdio.h>
#include <netdb.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <unistd.h> // read(), write(), close() - optional if you user send/recv

#define MAX 80
#define SA struct sockaddr

// Check for errors
void errorCheck(char *message){
    perror(message);
    exit(1);
}

int main(int argc, char *argv[])
{
    int sockfd, connfd, len, port, status, n;
    struct sockaddr_in servaddr, cli;
    char fileName[MAX];


    if (argc != 2){
        fprintf(stderr, "Invalid Input: ./server portnum\n");
    }
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if(sockfd < 0){
        errorCheck("Couldn't Open Socket");
    }
    printf("Socket successfully created..\n");


    bzero((char*) &servaddr, sizeof(servaddr));
    port = atoi(argv[1]);
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(port);
    servaddr.sin_addr.s_addr = INADDR_ANY;
    if(bind(sockfd, (struct sockaddr *) &servaddr, sizeof(servaddr)) < 0){
        errorCheck("ERROR Binding");
        
    }
    printf("Socket successfully binded..\n");

    if (listen(sockfd, 5) < 0){
        errorCheck("Listening Fail");
    }
    printf("Server listening..\n");
    len = sizeof(cli);

    connfd = accept(sockfd, (struct sockaddr *) &cli, &len);
    
    if(connfd < 0){
        errorCheck("Accept ERROR");
    }
    printf("Server accept the client..\n");

    n = read(connfd, fileName, sizeof(fileName));
    FILE *fileToSend = fopen(fileName, "r");
    
    if(fileToSend == NULL){
        errorCheck("Invalid File Name:");
    }    
    printf("Client requested file: %s\n", fileName);
    char temp[MAX]; //CHATGPT informed me that I needed to create a new var to deconflict
    while((n = fread(temp, sizeof(char), MAX, fileToSend)) > 0){
        write(connfd, temp, n);
    }

    if(n < 0){
        errorCheck("ERROR sending file");
    }

    fclose(fileToSend);
    close(sockfd);
    close(connfd);
    printf("Closed %s\n", fileName);
    printf("Exiting\n");


    return 0;
}