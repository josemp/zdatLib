/* simple-client.c
 * es un simple cliente en c
 * 
 *
 * SUCH DAMAGE.
*/

/*****************************************************************************/
/*** simple-client.c                                                       ***/
/***                                                                       ***/
/*****************************************************************************/

/**************************************************************************
*   This is a simple client socket reader.  It opens a socket, connects
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
#include "simpleSocketCliente.h"
#define PORT_TIME       13              /* "time" (not available on RedHat) */
#define PORT_FTP        21              /* FTP connection port */
#define SERVER_ADDR     "127.0.0.1"     /* localhost */
#define MAXBUF          1024
//static int sockfd;
pthread_t  tid;

static int sockfd;
static char buffer[MAXBUF+1];
static int retEstado;
static long lenLeido;
long simpleClienteGetlenRead()
{
	return(lenLeido);
}
int simpleClienteGetReadEstado()
{
	return(retEstado);
}

int simpleClienteGetSockfd()
{
	return(sockfd);
}
char *simpleClienteGetBuffer()
{
	return(buffer);
}
int simpleSocketCliente(int puerto)
{   

tid =pthread_self();

    struct sockaddr_in dest;
 
     printf("Open socket for streaming\n");fflush(stdout);
    /*---Open socket for streaming---*/
    if ( (sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0 )
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
    if ( connect(sockfd, (struct sockaddr*)&dest, sizeof(dest)) != 0 )
    {
        perror("Connect ");
        return(-1);
    }
	printf("conectado ...... \n");fflush(stdout);
	return(sockfd);
}

void *simpleClienteRead_x(void *arg)
{
	//int fd=*(int *) arg;
	simpleClienteRead();
	pthread_kill(tid,SIGUSR1);
	return(NULL);
}
void simpleClienteReadHilo()
{
	pthread_t id;
	printf("entrando a socketTestReadHILO\n");fflush(stdout);
     pthread_create(&id ,NULL,simpleClienteRead_x,NULL );
}
void simpleClienteRead()
{	
   memset(buffer,0,MAXBUF);
   printf("entrando a socketTestRead\n");fflush(stdout);
    /*---Get "Echo?"---*/
    bzero(buffer, MAXBUF);
    recv(sockfd, buffer, sizeof(buffer), 0);
    printf("LEIDO ------ <%s>\n", buffer);fflush(stdout);
	
	
}
int  simpleClienteWrite(char *buf)
{
     int ret=write(sockfd,buf,strlen(buf));
	return(ret);
	
	
}


void *simpleClienteReadLargo(void *arg)
{	
   lenLeido=0;
   memset(buffer,0,MAXBUF);
   printf("entrando a socketTestRead\n");fflush(stdout);
    /*---Get "Echo?"---*/
  
	while(1)
	{
		  bzero(buffer, MAXBUF);
    retEstado=read(sockfd, buffer, sizeof(buffer)-1);
	if (retEstado<0)
		break;
	//printf("Hilo lee <%.*s>\n",retEstado,buffer);fflush(stdout);
	lenLeido= lenLeido + (long) retEstado;
	if (buffer[strlen(buffer)-1]==3)
		break;
    }
	printf("Hilo Estado <%d>\n",retEstado);
	pthread_kill(tid,SIGUSR1);
	return(NULL);
}
void simpleClienteReadLargoHilo()
{
	pthread_t id;
	printf("entrando a socketTestReadHILO\n");fflush(stdout);
     pthread_create(&id ,NULL,simpleClienteReadLargo,NULL );
}

void *simpleClienteReadLargo1(void *arg)
{	
   
   lenLeido=0;
   
   printf("entrando a socketTestRead Largo 1\n");fflush(stdout);
    /*---Get "Echo?"---*/
   
	while(1)
	{
	  bzero(buffer, MAXBUF);
     retEstado=read(sockfd, buffer, MAXBUF-1);
	 if (retEstado<=0)
		break;
	//printf("Hilo lee <%.*s>\n",retEstado,buffer);fflush(stdout);
	 lenLeido= lenLeido + (long) retEstado;
	 if (buffer[strlen(buffer)-1]==3)
	 {
		printf("detectado ETX\n");fflush(stdout);
		break;
	 }
    }
	printf("Hilo Estado <%d>\n",retEstado);fflush(stdout);
	pthread_kill(tid,SIGUSR1);
	return(NULL);
}
void simpleClienteReadLargo1Hilo()
{
	pthread_t id;
	printf("entrando a socketTestReadLargo1HILO\n");fflush(stdout);
     pthread_create(&id ,NULL,simpleClienteReadLargo1,NULL );
}


