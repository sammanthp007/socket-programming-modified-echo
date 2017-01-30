/*
 * Modified version of Paul Griffiths, 1999 echo program by Samman Bikram
 * Thapa of Howard University. Modified for assignment 1 of Neworks and Web
 * Programming Class 2017 spring session taught by Dr. Li.
 *
 * Author: Samman Bikram Thapa (samman.thapa@bison.howard.edu)
 *
 * Modified TCP/IP echo server.
 *
 *
 */


#include "helper.h"           /*  our own helper functions  */

int main(int argc, char *argv[]) {
    int       list_s;                /*  listening socket          */
    int       conn_s;                /*  connection socket         */
    short int port;                  /*  port number               */
    struct    sockaddr_in servaddr;  /*  socket address structure  */
    char      buffer[MAX_LINE];      /*  character buffer          */
    char     *endptr;                /*  for strtol()              */
    struct sockaddr_in clientaddr;  // client address structure
    int sockaddr_len = sizeof(struct sockaddr_in);  // size for bind and accept
    int data_len;         // length of message from recv is stored here

    /*  Get port number from the command line, and
     *          set to default port if no arguments were supplied  */

    if ( argc == 2 ) {
        port = strtol(argv[1], &endptr, 0);
        if ( *endptr ) {
            fprintf(stderr, "ECHOSERV: Invalid port number.\n");
            exit(EXIT_FAILURE);
        }
    }
    else if ( argc < 2 ) {
        port = ECHO_PORT;
    }
    else {
        fprintf(stderr, "ECHOSERV: Invalid arguments.\n");
        exit(EXIT_FAILURE);
    }


    /*  Create the listening socket  */

    if ( (list_s = socket(AF_INET, SOCK_STREAM, 0)) < 0 ) {
        fprintf(stderr,"ECHOSERV: Error creating listening socket.\n");
        exit(EXIT_FAILURE);
    }


    /*  Set all bytes in socket address structure to
     *          zero, and fill in the relevant data members   */

    memset(&servaddr, 0, sizeof(servaddr));
    servaddr.sin_family      = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port        = htons(port);


    /*  Bind our socket addresss to the listening socket */

    if ( bind(list_s, (struct sockaddr *) &servaddr, sizeof(servaddr)) < 0 ) {
        fprintf(stderr, "ECHOSERV: Error calling bind()\n");
        exit(EXIT_FAILURE);
    }

    /* if bind succeeds then listen using the listening socket*/
    if ( listen(list_s, LISTENQ) < 0 ) {
        fprintf(stderr, "ECHOSERV: Error calling listen()\n");
        exit(EXIT_FAILURE);
    }

    /*  Enter an infinite loop to respond
     *          to client requests and echo input  */

    while ( 1 ) {

        /*  Wait for a connection, then accept() it  */
        if ((conn_s = accept(list_s,(struct sockaddr *)&clientaddr,&sockaddr_len)) < 0) {
            fprintf(stderr, "ECHOSERV: Error calling accept()\n");
            exit(EXIT_FAILURE);
        }

        printf("New Client connected from port %d and IP %s\n", ntohs(clientaddr.sin_port), inet_ntoa(clientaddr.sin_addr));

        data_len = 1;

        // data_len is 0 when orderly shutdown happens
        while (data_len)
        {
            /*  Retrieve an input line from the connected socket */
            data_len = recv(conn_s, buffer, MAX_LINE, 0);

            // Because noise can get in during transmission
            buffer[data_len] = '\0';

            /* Capitalize and send message */
            if (strncmp(buffer, "CAP", 3) == 0)
            {
                // get the content
                char content[data_len - 4];
                memcpy (content, &buffer[4], data_len - 5);
                content[data_len - 5] = '\0';

                // capitalize the content
                Cap(content);

                // create a message to send
                char msg[data_len];
                char size_of_content[10];
                int char_in_content = strlen(content);
                sprintf(msg, "%d", char_in_content);
                strcat(msg, "\n");
                strcat(msg, content);

                // send message
                send(conn_s, msg, strlen(msg), 0);
            }

            /* Read the content of the file and send content */
            if (strncmp(buffer, "FILE", 4) == 0)
            {
                // get the content
                char content[data_len - 5];
                memcpy (content, &buffer[5], data_len - 6);
                content[data_len - 6] = '\0';

                // look if there is a file called content in the directory
                FILE * fpointer;
                if ((fpointer = fopen(content, "r")) == NULL)
                {
                    char return_val[] = "9\nNOT FOUND"; 
                    send(conn_s, return_val, strlen(return_val), 0);
                }
                else
                {
                    // get the content of the file
                    char *filecontent = ReadFile(content);
                    if (filecontent)
                    {
                        // create and modify msg
                        char msg[strlen(filecontent)];
                        filecontent[strlen(filecontent) - 1] = '\0';
                        int char_in_file = strlen(filecontent);
                        sprintf(msg, "%d", char_in_file);
                        strcat(msg, "\n");
                        strcat(msg, filecontent);

                        // send message to client
                        send(conn_s, msg, strlen(msg), 0);
                        free(filecontent);
                    }
                }
            }

        }
        /*  Close the connected socket after client disconnects */

        printf("Client Disconnected \n");

        if ( close(conn_s) < 0 ) {
            fprintf(stderr, "ECHOSERV: Error calling close()\n");
            exit(EXIT_FAILURE);
        }

    }
}

