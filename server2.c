#include <stdlib.h>
#include <stdio.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <string.h>
#include <ctype.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT2 20002
#define PORT3 20003
#define BACKLOG 10  
#define BUFLEN 256
#define HOST "127.0.0.1"

int main(int argc, char const* argv[])
{

    int server_fd2, new_socket2, client_fd;
    struct sockaddr_in server_address2, client_address;
    int optval = 1;
    char buffer[BUFLEN];
    socklen_t addrlen2 = sizeof(server_address2);

    server_address2.sin_family = AF_INET;
    server_address2.sin_addr.s_addr = inet_addr(HOST);
    server_address2.sin_port = htons(PORT3);

    if((server_fd2 = socket(AF_INET, SOCK_STREAM, 0)) < 0){
        perror("failed to create a socket");
        exit(EXIT_FAILURE);
    }

    if(setsockopt(server_fd2, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval))){
        perror("setsockopt failed");
        exit(EXIT_FAILURE);
    }

    if(bind(server_fd2, (struct sockaddr*)&server_address2, sizeof(server_address2)) < 0){
        perror("bind failed");
        exit(EXIT_FAILURE);
    }

    if(listen(server_fd2, BACKLOG) < 0){
        perror("listen failed");
        exit(EXIT_FAILURE);
    }

   
    if ((new_socket2 = accept(server_fd2, (struct sockaddr*)&server_address2, &addrlen2)) < 0) {
        perror("accept failed");
        exit(EXIT_FAILURE);
    }

    if(recv(new_socket2, buffer, BUFLEN, 0) < 0){
        perror("failed to receive");
        exit(EXIT_FAILURE);
    }

    printf("Received %s from server2\n", buffer);

    close(new_socket2);

    int value = 0;
    for (int i = 0; buffer[i] != '\0'; i++) {
        value = value * 10 + (buffer[i] - '0');
    }

    value += 1;

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

    printf("Adding one to the number...\n");

    client_address.sin_family = AF_INET;
    client_address.sin_addr.s_addr = inet_addr(HOST);
    client_address.sin_port = htons(PORT2);

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

    printf("Sent number %s back to client using port %d\n", buffer, PORT2);


}