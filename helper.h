/*
 *
 *   HELPER.H
 *     ========
 *  (c) Paul Griffiths, 1999
 *  Email: mail@paulgriffiths.net
 *
 *   Interface to socket helper functions.
 *
 *   Many of these functions are adapted from, inspired by, or
 *   otherwise shamelessly plagiarised from "Unix Network
 *  Programming", W Richard Stevens (Prentice Hall).
 */


#ifndef PG_SOCK_HELP
#define PG_SOCK_HELP


#include <unistd.h>             /*  for ssize_t data type  */
#include <sys/socket.h>
#include <errno.h>
#include <string.h>
#include <ctype.h>
#include <stdio.h>

#define LISTENQ        (1024)   /*  Backlog for listen()   */


/*  Function declarations  */

ssize_t Readline(int fd, void *vptr, size_t maxlen);
ssize_t Writeline(int fc, const void *vptr, size_t maxlen);
void Cap(char []);

void Cap(char string[]){
    int i;
    int x = strlen(string); // get the length of the whole string.
    if (isalpha(string[0]))
    {
        string[0] = toupper(string[0]);
    }
    for (i=1;i<x;i++){
        if (isalpha(string[i]) && string[i-1] == ' '){
            // only first letters of a word.
            string[i]= toupper(string[i]);
        }
    }
}



/*  Read a line from a socket  */

ssize_t Readline(int sockd, void *vptr, size_t maxlen) {
    ssize_t n, rc;
    char    c, *buffer;

    buffer = vptr;

    for ( n = 1; n < maxlen; n++ ) {

        if ( (rc = read(sockd, &c, 1)) == 1 ) {
            *buffer++ = c;
            if ( c == '\n' )
                break;
        }
        else if ( rc == 0 ) {
            if ( n == 1 )
                return 0;
            else
                break;
        }
        else {
            if ( errno == EINTR )
                continue;
            return -1;
        }
    }

    *buffer = 0;
    return n;
}


/*  Write a line to a socket  */

ssize_t Writeline(int sockd, const void *vptr, size_t n) {
    size_t      nleft;
    ssize_t     nwritten;
    const char *buffer;

    buffer = vptr;
    nleft  = n;

    while ( nleft > 0 ) {
        if ( (nwritten = write(sockd, buffer, nleft)) <= 0 ) {
            if ( errno == EINTR )
                nwritten = 0;
            else
                return -1;
        }
        nleft  -= nwritten;
        buffer += nwritten;
    }

    return n;
}



#endif  /*  PG_SOCK_HELP  */
