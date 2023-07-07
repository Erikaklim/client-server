#include <stdlib.h>
#include <stdio.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <string.h>
#include <ctype.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 20001
#define PORT3 20003
#define BACKLOG 10  
#define BUFLEN 256
#define HOST "127.0.0.1"

int main(int argc, char const* argv[])
{

    int server_fd, new_socket, client_fd;
    struct sockaddr_in server_address, client_address;
    int optval = 1;
    char buffer[BUFLEN];
    char buffer2[BUFLEN];
    socklen_t addrlen = sizeof(server_address);

    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = inet_addr(HOST);
    server_address.sin_port = htons(PORT);

    if((server_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0){
        perror("failed to create a socket");
        exit(EXIT_FAILURE);
    }

    if(setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval))){
        perror("setsockopt failed");
        exit(EXIT_FAILURE);
    }

    if(bind(server_fd, (struct sockaddr*)&server_address, sizeof(server_address)) < 0){
        perror("bind failed");
        exit(EXIT_FAILURE);
    }


    if(listen(server_fd, BACKLOG) < 0){
        perror("listen failed");
        exit(EXIT_FAILURE);
    }

    if ((new_socket = accept(server_fd, (struct sockaddr*)&server_address, &addrlen)) < 0) {
        perror("accept failed");
        exit(EXIT_FAILURE);
    }


    if(recv(new_socket, buffer, BUFLEN, 0) < 0){
        perror("failed to receive");
        exit(EXIT_FAILURE);
    }

    printf("Received number %s from client...\n", buffer);

    close(new_socket);

    int value = 0;
    for (int i = 0; buffer[i] != '\0'; i++) {
        value = value * 10 + (buffer[i] - '0');
    }

    value *= 2;

    int i = 0;
    while (value != 0) {
        int digit = value % 10;
        buffer[i++] = digit + '0';
        value /= 10;
    }
    buffer[i] = '\0'; 
    
    int len = strlen(buffer);
    char *start = buffer;
    char *end = buffer + len - 1;
    
    while (start < end) {
        char temp = *start;
        *start++ = *end;
        *end-- = temp;
    }

    printf("Multiplying number by 2\n");
    
    client_address.sin_family = AF_INET;
    client_address.sin_addr.s_addr = inet_addr(HOST);
    client_address.sin_port = htons(PORT3);

    if((client_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0){
        perror("failed to create a socket");
        exit(EXIT_FAILURE);
    }

    if(connect(client_fd, (struct sockaddr*)&client_address, sizeof(client_address)) < 0){
        perror("Connection failed");
        exit(EXIT_FAILURE);
    }


    if(send(client_fd, buffer, BUFLEN, 0) < 0){
        perror("failed to send");
        exit(EXIT_FAILURE);
    }

    printf("Sent number %s to server 2 using port %d\n", buffer, PORT3);

}






