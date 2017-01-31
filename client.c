/*
 * Modified version of Paul Griffiths, 1999 echo program by Samman Bikram
 * Thapa of Howard University. Modified for assignment 1 of Neworks and Web
 * Programming Class 2017 spring session taught by Dr. Li.
 *
 * Author: Samman Bikram Thapa (samman.thapa@bison.howard.edu)
 *
 * Modified TCP/IP echo client.
 *
 *
 */



#include "helper.h"           /*  Our own helper functions  */

/*  main()  */

int main(int argc, char *argv[]) {

    int       conn_s;                /*  connection socket         */
    short int port;                  /*  port number               */
    struct    sockaddr_in servaddr;  /*  socket address structure  */
    char      buffer[MAX_LINE];      /*  character buffer          */
    char     *szAddress;             /*  Holds remote IP address   */
    char     *szPort;                /*  Holds remote port         */
    char     *endptr;                /*  for strtol()              */


    /*  Get command line arguments  */

    if (argc == 3) {
        /*  Set the remote port  */
        port = strtol(argv[2], &endptr, 0);
        if ( *endptr ) {
            printf("SERVER: Invalid port supplied.\n");
            exit(EXIT_FAILURE);
        }
    }
    else {
        fprintf(stderr, "SERVER: Invalid arguments. Use the format <client> <server IP> <server Port> \n");
        exit(EXIT_FAILURE);
    }


    /*  Create the listening socket  */

    if ( (conn_s = socket(AF_INET, SOCK_STREAM, 0)) < 0 ) {
        fprintf(stderr, "SERVER: Error creating listening socket.\n");
        exit(EXIT_FAILURE);
    }


    /*  Set all bytes in socket address structure to
     *          zero, and fill in the relevant data members   */

    memset(&servaddr, 0, sizeof(servaddr));
    servaddr.sin_family      = AF_INET;
    servaddr.sin_port        = htons(port);


    /*  Set the remote IP address  */

    if ( inet_aton(argv[1], &servaddr.sin_addr) <= 0 ) {
        printf("SERVER: Invalid remote IP address.\n");
        exit(EXIT_FAILURE);
    }


    /*  connect() to the remote echo server  */

    if (connect(conn_s,(struct sockaddr *)&servaddr, sizeof(servaddr)) < 0 ) {
        printf("SERVER: Error calling connect()\n");
        exit(EXIT_FAILURE);
    }


    while (1)
    {
        /* Prompt user */

        printf("Enter s for message; t for file; q to exit): \n");
        fgets(buffer, MAX_LINE, stdin);

        if (strlen(buffer) == 2 && strncmp(buffer, "s", 1) == 0)
        {
            printf("Enter message in form: \n");
            // fgets also takes in the last \n
            fgets(buffer, MAX_LINE, stdin);

            int buffer_len = strlen(buffer);
            buffer[buffer_len + 1] = '\0';

            /* Send message to server in the form "CAP\nxxx\n" */
            char message[MAX_LINE + 10];
            strcpy(message, "CAP\n");
            strcat(message, buffer);

            message[buffer_len + 4] = '\0';

            send(conn_s, message, strlen(message), 0);

            /* receive message from server */
            int data_len = recv(conn_s, message, MAX_LINE + 10, 0);

            // because noise gets added during transmission
            message[data_len] = '\0';
            printf("Server Response: %s \n", message);
        }
        else if (strlen(buffer) == 2 && strncmp(buffer, "t", 1) == 0)
        {
            printf("Enter file name: \n");
            // fgets also takes in the last \n
            fgets(buffer, MAX_LINE, stdin);

            int buffer_len = strlen(buffer);
            buffer[buffer_len + 1] = '\0';

            /* Send message to server in the form "FILE\nxxx\n" */
            char message[MAX_LINE + 10];
            strcpy(message, "FILE\n");
            strcat(message, buffer);

            message[buffer_len + 5] = '\0';

            send(conn_s, message, strlen(message), 0);

            /* receive message from server */
            int data_len = recv(conn_s, message, MAX_LINE + 10, 0);

            // because noise gets added during transmission
            message[data_len] = '\0';
            printf("Server Response:\n%s\n", message);
        }
        else if (strlen(buffer) == 2 && strncmp(buffer, "q", 1) == 0)
        {
            return EXIT_SUCCESS;
        }
        else
        {
            /*  Send string to echo server, and retrieve response  */

            /* Writeline(conn_s, buffer, strlen(buffer)); */
            /* Readline(conn_s, buffer, MAX_LINE-1); */


            /*  Output echoed string  */
            /* printf("%d <- buffer size\n", strlen(buffer)); */

            /* printf("Echo response: %s\n", buffer); */
        }
    }

    return EXIT_SUCCESS;
}
