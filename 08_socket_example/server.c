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
    int server_fd, client_fd;
    struct sockaddr_in address;
    int addrlen = sizeof(address);

    char buffer[1024] = {0};

    char *msg = "Hello from the Other Side!";


    // handling error of the socket creation
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0)
    {
        printf("socket failed\n");
        exit(-1);
    }

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = PORT;

    // time to bind!
    if (bind(server_fd, (struct sockaddr*)&address, sizeof(address)) < 0)
    {
        printf("bind failed\n");
        exit(-1);
    }

    if (listen(server_fd, 5) < 0)
    {
        printf("listen failed!\n");
        exit(-1);
    }

    printf("waiting for a client ...\n");
    if ((client_fd = accept(server_fd, (struct sockaddr*)&address, &addrlen)) < 0)
    {
        printf("accept failed!\n");
        exit(-1);
    }
    printf("a client connected.\n");
    read(client_fd, buffer, 1024);
    printf("%s\n", buffer);
    send(client_fd, msg, strlen(msg), 0);
    printf("msg sent to client\n");

    close(client_fd);
    close(server_fd);



    return 0;
}