#include<stdio.h>
#include<stdlib.h>
#include <unistd.h>

#include <sys/types.h>
#include <sys/socket.h>
#include<netinet/in.h>
#include<string.h>
#include <arpa/inet.h>

#include <errno.h>

int main()
{
    // take a tcp socket
    int server_socket, client_socket;
    struct sockaddr_in server;
    struct sockaddr_in client;
    short int port;
    int len;
    char mesg[] = "Hello World!";
    int sent_data;

    port = 10000;

    // try connecting a socket with default AF_INET, SOCK_STREAM for TCP
    if ((server_socket = socket(AF_INET, SOCK_STREAM, 0)) == -1)
    {
        perror("socket: ");
        exit(-1);
    }

    // used in the skeleton code
    //memset(&server, 0, sizeof(server));
    server.sin_family = AF_INET;
    server.sin_port = htons(port);
    // server binds to any interface in the machine
    server.sin_addr.s_addr = INADDR_ANY;
    // make everything else 0 in the server address server
    bzero(&server.sin_zero, 8);

    len = sizeof(struct sockaddr_in);

    // bind the socket to a port
    if ((bind(server_socket, (struct sockaddr *)&server, len)) == -1)
    {
        perror("bind");
        exit(-1);
    }

   // listen 
   if ((listen(server_socket, 5)) == -1)
   {
       perror("listen");
       exit(-1);
   }

   
    // wait for clients
    while (1) 
    {
        if (client_socket = accept(server_socket, (struct sockaddr *)&client,
                    &len) == -1)
        {
            perror("Accept");
            exit(-1);
        }

        sent_data = send(client_socket, mesg, strlen(mesg), 0);

        printf("Sent %d bytes to client: %s\n", sent_data, 
                inet_ntoa(client.sin_addr));

        close(client_socket);

    }


    // send out hello world after client connects
    return 0;
}









