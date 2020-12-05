//
// Created by Vahid on 11/24/2020.
//

#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>


#define PORT 8000

int main (void)
{
    int my_socket = 0;
    struct sockaddr_in address;
    int addrlen = sizeof(address);
    char buffer[1024] = {0};

    char *msg = "Hello from the Out Side!";

    if ((my_socket = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        printf("Socket creation error\n");
        return -1;
    }
    address.sin_family = AF_INET;
    address.sin_port = PORT;

    // Convert IPv4 address from text to binary form
    if(inet_pton(AF_INET, "127.0.0.1", &address.sin_addr)<=0)
    {
        printf("Invalid address\n");
        return -1;
    }

    printf("connecting to server ...\n");
    if (connect(my_socket, (struct sockaddr *)&address, sizeof(address)) < 0) {
        printf("Connection Failed \n");
        return -1;
    }


    send(my_socket, msg, strlen(msg), 0);
    printf("msg sent to server\n");
    read(my_socket, buffer, 1024);
    printf("%s\n", buffer);

    close(my_socket);

    return 0;
}