#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <memory.h>
#include <event2/event.h>
#include <event2/bufferevent.h>
#include <event2/buffer.h>
#include <assert.h>
#include "zlLog.h"
#include "zlEvtTimer.h"
#include "zlEvtSocket.h"


static int contadorCliente1;
static void connectCliente1_cb(struct zlEvtSocket *socket)
{
	printf("llega conexion\n");fflush(stdout);
	char aux[100];
	contadorCliente1++;
	sprintf(aux,"hola%03d",contadorCliente1);
	LogW(9,"Cliente envia <%s>\n",aux);fflush(stdout);
	zlEvtSocketWriteCommand(0,socket,aux,(int)strlen(aux));
}
static void readCliente1_cb(struct evbuffer *input,
                    struct zlEvtSocket *socket)
					{
						char aux[101];
						char auxLectura[101];
						printf("llega lectura\n");fflush(stdout);
						//int lenMsg= (int) evbuffer_get_length(input);
	                    int ret=evbuffer_remove(input, auxLectura,(size_t) 100 );// Leemos 100 del buffer
						
						assert(ret>0);
						auxLectura[ret]=0;
						
						printf("Cliente recibe <%s>\n",auxLectura);fflush(stdout);
						sprintf(aux,"hola%03d",contadorCliente1);
						printf("Recibido <%s> Esperado <%s>\n",auxLectura,aux);
						if (strcmp(auxLectura,"cierrate")==0)
						{
							zlEvtSocketCloseCommand(socket);return;
						}
						assert(strcmp(aux,auxLectura)==0);
						if (contadorCliente1==3) {
							sprintf(aux,"finServer",contadorCliente1);
						    LogW(9,"Cliente envia <%s>\n",aux);fflush(stdout);
						    zlEvtSocketWriteCommand(0,socket,aux,(int)strlen(aux));
							//zlEvtSocketCloseCommand(socket);return;
							return;
							}
					    contadorCliente1++;
						sprintf(aux,"hola%03d",contadorCliente1);
						LogW(9,"Cliente envia <%s>\n",aux);fflush(stdout);
						zlEvtSocketWriteCommand(0,socket,aux,(int)strlen(aux));
						     
					}
static void closeCliente1_cb(struct zlEvtSocket *socket)
{
	printf("llega close\n");fflush(stdout);
}
static void writeCliente1_cb(struct zlEvtSocket *socket)
{
	printf("llega write\n");fflush(stdout);
}
zlEvtSocketList_t listaCliente1;
void testCliente1(struct event_base *base,char *ip,int puerto)
{
printf("Lanzando cliente 1 puerto <%d>\n",puerto);	
listaCliente1.numConexiones=0;
strcpy(listaCliente1.prefijoLog,"file");
listaCliente1.levelLog=9;	
zlEvtSocket_t *misocket= zlEvtSocketCreaCliente(
    base,
	ip,
	puerto,
	&listaCliente1,
	connectCliente1_cb,
	readCliente1_cb,
	closeCliente1_cb,
	writeCliente1_cb,
	EV_READ|EV_WRITE,
	NULL);
if (misocket==NULL)
{
	printf("Error lanzando cliente\n");fflush(stdout);
}	
else
	printf("Correcto lanzando cliente1\n");fflush(stdout);

	
}