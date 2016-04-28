/*
***AUTORE: Dario Platania 'dariopl91@gmail.com', Marzo 2016
gcc -g -Wall -DTRACE -o server server.c errlib.c sockwrap.c
*/

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <ctype.h>
#include <stdio.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/stat.h>
#include <rpc/xdr.h>

#include <string.h>
#include <time.h>
#include <unistd.h>

#include "errlib.h"
#include "sockwrap.h"

#ifdef TRACE
#define trace(x) x
#else
#define trace(x)
#endif

char * prog_name;

uint16_t		  tport_n; //= 1500;
struct        sockaddr_in	servaddr;
struct        sockaddr_in	clientaddr;
int 			    listenfd;
int           ac;
socklen_t     addrlen;
char          lettura[255];
char          sendbuff[1025];
char          file[50];
struct        stat sstr;
int           i=0;
int           j=0;
int           size1;
uint32_t      size,timestamp; //dimensione file
FILE          *fp;
int           byte_ricevuti;
int           i;
int           nchildren = 0;
int           childpid = 0;
char          msg[200];

int main(int argc, char **argv)
{

  prog_name = argv[0];

  if(argc!=3)
    err_quit("wrong number of parameters e.g. ./server 'port' 'child_number'", prog_name);
    tport_n=atoi(argv[1]);
    nchildren = atoi(argv[2]);

    /* create socket */
  	listenfd = Socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    trace (err_msg("(%s) socket created",prog_name));

    /* specify address to bind to */
    memset(&servaddr, 0, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(tport_n);
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    clientaddr.sin_family = AF_INET;

    /*BIND*/
    Bind(listenfd, (struct sockaddr*) &servaddr, sizeof(servaddr));
    trace ( err_msg("(%s) listening on %s:%u", prog_name, inet_ntoa(servaddr.sin_addr), ntohs(servaddr.sin_port)) );

    /*LISTEN*/
    Listen(listenfd, 10); //LISTENQ NUMERO DI RICHIESTE PENDENTI
    for(i = 0; i < nchildren; i++){

      if((childpid=fork())<0)
      err_msg("fork() failed");

        else if (childpid > 0)
        {
        /* processo padre */
        }

        else {
        /* processo figlio */
            while(1) {
            trace( err_msg ("(%s) waiting for connections ...", prog_name) );
            addrlen = sizeof(struct sockaddr_in);
            ac = Accept(listenfd, (struct sockaddr*) &clientaddr, &addrlen);
            trace ( err_msg("(%s) - new connection from client %s:%u", prog_name, inet_ntoa(clientaddr.sin_addr), ntohs(clientaddr.sin_port)));
            Recv(ac, msg, sizeof(msg), 0);
            //break;
            }
        }
    }
    while(1){}
return 0;
}
