/* multi-client.c
 * es un multi cliente en c
 * 
 *
 * SUCH DAMAGE.
*/

/*****************************************************************************/
/*** multi-client.c                                                       ***/
/***                                                                       ***/
/*****************************************************************************/

/**************************************************************************
*   This is a multi client socket reader.  It opens a socket, connects
*   to a server, reads the message, and closes.
**************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <resolv.h>
#include <memory.h>
#include <pthread.h>

#define PORT_TIME       13              /* "time" (not available on RedHat) */
#define PORT_FTP        21              /* FTP connection port */
#define SERVER_ADDR     "127.0.0.1"     /* localhost */
//#define MAXBUF          1024
#include "multiSocketCliente.h"
//static int sockfd;
pthread_t  tid;


long multiClienteGetlenRead(multiSocketCliente_t *multi)
{
	return(multi->lenLeido);
}
int multiClienteGetReadEstado(multiSocketCliente_t *multi)
{
	return(multi->retEstado);
}

int multiClienteGetSockfd(multiSocketCliente_t *multi)
{
	return(multi->sockfd);
}
char *multiClienteGetBuffer(multiSocketCliente_t *multi)
{
	return(multi->buffer);
}
int multiSocketCliente(multiSocketCliente_t *multi,int puerto)
{   

tid =pthread_self();

    struct sockaddr_in dest;
 
     printf("Open socket for streaming\n");fflush(stdout);
    /*---Open socket for streaming---*/
    if ( (multi->sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0 )
    {
        perror("Socket");
        return(-1);
    }
printf("Initialize server address/port struct\n");fflush(stdout);
    /*---Initialize server address/port struct---*/
    bzero(&dest, sizeof(dest));
    dest.sin_family = AF_INET;
    dest.sin_port = htons((uint16_t) puerto);
    if ( inet_aton(SERVER_ADDR,(struct in_addr *) &dest.sin_addr.s_addr) == 0 )
    {
        perror(SERVER_ADDR);
        return(-1);
    }
printf("Connect to server\n");fflush(stdout);

    /*---Connect to server---*/
    if ( connect(multi->sockfd, (struct sockaddr*)&dest, sizeof(dest)) != 0 )
    {
        perror("Connect ");
        return(-1);
    }
	printf("conectado ...... <%d>\n",multi->sockfd);fflush(stdout);
	return(multi->sockfd);
}

/* ---------------------------------------- */

void multiClienteReadHilo(multiSocketCliente_t *multi)
{
	//pthread_t id;
	printf("entrando a socketTestReadHILO\n");fflush(stdout);
     pthread_create(&multi->tid ,NULL,multiClienteRead,multi );
}
void *multiClienteRead(void *arg)
{	
   multiSocketCliente_t *multi= (multiSocketCliente_t *) arg;
   memset(multi->buffer,0,MAXBUF);
   printf("entrando a socketTestRead\n");fflush(stdout);
    /*---Get "Echo?"---*/
    bzero(multi->buffer, MAXBUF);
    recv(multi->sockfd, multi->buffer, sizeof(multi->buffer), 0);
    printf("LEIDO ------ <%s>\n", multi->buffer);fflush(stdout);
	pthread_kill(tid,SIGUSR1);
	return(NULL);
	
	
}
int  multiClienteWrite(multiSocketCliente_t *multi,char *buf)
{
	  printf("Escribiendo e socket <%d><%s>\n",multi->sockfd,buf);
     int ret=write(multi->sockfd,buf,strlen(buf));
	return(ret);
	
	
}


void *multiClienteReadLargo(void *arg)
{	
   multiSocketCliente_t *multi= (multiSocketCliente_t *) arg;
   multi->lenLeido=0;
   memset(multi->buffer,0,MAXBUF);
   printf("entrando a socketTestRead\n");fflush(stdout);
    /*---Get "Echo?"---*/
   
	while(1)
	{
	 bzero(multi->buffer, MAXBUF);	
    multi->retEstado=read(multi->sockfd, multi->buffer, sizeof(multi->buffer)-1);
	if (multi->retEstado<0)
		break;
	multi->lenLeido= multi->lenLeido + (long) multi->retEstado;
	if (multi->buffer[strlen(multi->buffer)-1]==3)
		break;
    }
	multi->terminado=1;
	pthread_kill(tid,SIGUSR1);
	return(NULL);
}
void multiClienteReadLargoHilo(multiSocketCliente_t *multi)
{

	printf("entrando a socketTestReadHILO\n");fflush(stdout);
     pthread_create(&multi->tid ,NULL,multiClienteReadLargo,multi );
}



