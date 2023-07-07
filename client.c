#include <stdlib.h>
#include <stdio.h>
#include <netinet/in.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 20001
#define PORT2 20002
#define BUFLEN 256
#define BACKLOG 10 
#define HOST "127.0.0.1"

int main(int argc, char const* argv[])
{
    int client_fd, client_fd2, new_socket;
    struct sockaddr_in server_address, server_address2;
    int optval = 1;
    char buffer[BUFLEN];
    char message[BUFLEN];
    socklen_t addrlen2 = sizeof(server_address2);

    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = inet_addr(HOST);
    server_address.sin_port = htons(PORT);

    if((client_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0){
        perror("failed to create a socket");
        exit(EXIT_FAILURE);
    }

    if(connect(client_fd, (struct sockaddr*)&server_address, sizeof(server_address)) < 0){
        perror("Connection failed");
        exit(EXIT_FAILURE);
    }

    printf("Enter number to send:");
    scanf("%s", message);
    
    if(send(client_fd, message, BUFLEN, 0) < 0){
        perror("failed to send");
        exit(EXIT_FAILURE);
    }

    printf("Number %s sent to server1 using port... %d\n", message, PORT);

    server_address2.sin_family = AF_INET;
    server_address2.sin_addr.s_addr = inet_addr(HOST);
    server_address2.sin_port = htons(PORT2);

    if((client_fd2 = socket(AF_INET, SOCK_STREAM, 0)) < 0){
        perror("failed to create a socket");
        exit(EXIT_FAILURE);
    }

    if(setsockopt(client_fd2, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval))){
        perror("setsockopt failed");
        exit(EXIT_FAILURE);
    }

    if(bind(client_fd2, (struct sockaddr*)&server_address2, sizeof(server_address2)) < 0){
        perror("bind failed");
        exit(EXIT_FAILURE);
    }

    if(listen(client_fd2, BACKLOG) < 0){
        perror("listen failed");
        exit(EXIT_FAILURE);
    }
   
    if ((new_socket = accept(client_fd2, (struct sockaddr*)&server_address2, &addrlen2)) < 0) {
        perror("accept failed");
        exit(EXIT_FAILURE);
    }

    if(recv(new_socket, buffer, BUFLEN, 0) < 0){
        perror("failed to receive");
        exit(EXIT_FAILURE);
    }

    printf("Received...%s\n", buffer);

    close(new_socket);

}
