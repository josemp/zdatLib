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

void testServer1(struct event_base *base,int puerto);
void testCliente1(struct event_base *base,char *ip,int puerto);
//haz eco
static void connectSocket_cb(struct zlEvtSocket *socket)
{
	printf("llega conexion\n");fflush(stdout);
}
static void readSocket_cb(struct evbuffer *input,
                    struct zlEvtSocket *socket)
					{
						char aux[101];
						printf("llega lectura\n");fflush(stdout);
						//int lenMsg= (int) evbuffer_get_length(input);
	                    int ret=evbuffer_remove(input, aux,(size_t) 100 );// Leemos 100 del buffer
						assert(ret>0);
						printf("Leidos <%d><%.*s>\n",ret,ret,aux);fflush(stdout);
						zlEvtSocketWriteCommand(0,socket,aux,ret);
						if (strncmp(aux,"fin",3)==0)
							zlEvtSocketCloseCommand(socket);
     
					}
static void closeSocket_cb(struct zlEvtSocket *socket)
{
	printf("llega close\n");fflush(stdout);
}
static void writeSocket_cb(struct zlEvtSocket *socket)
{
	printf("llega write\n");fflush(stdout);
}
void lanzaCliente1Diferido(void *tag)
{

testCliente1((struct event_base *) tag,"127.0.0.1",5555);
}

zlEvtSocketList_t lista;

int main(int argc , char **argv)
{
lista.numConexiones=0;
strcpy(lista.prefijoLog,"file");
lista.levelLog=9;
LogInit(NULL,9);
LgW(-1,9,"Esto es una prueba nivel 9\n");
printf("--------------- TEST1 -------------------\n");fflush(stdout);
printf("un cliente al puerto 5555\n");
	 struct event_base *base=event_base_new();
zlEvtSocket_t *misocket= zlEvtSocketCreaCliente(
    base,
	"127.0.0.1",
	5555,
	&lista,
	connectSocket_cb,
	readSocket_cb,
	closeSocket_cb,
	writeSocket_cb,
	EV_READ|EV_WRITE,
	NULL);
if (misocket==NULL)
{
	printf("Error lanzando cliente\n");fflush(stdout);
}	
else
	printf("Correcto lanzando cliente\n");fflush(stdout);

printf("Despachando\n");fflush(stdout);
	 event_base_dispatch(base );	
printf("--------------- FIN TEST1 -------------------\n");fflush(stdout);


printf("--------------- TEST2 -------------------\n");fflush(stdout);	
printf("server al 5555\n");fflush(stdout);
testServer1(base,5555);

printf("Despachar de nuevo \n");fflush(stdout);
zlEvtTimerNuevoActivaLibera(base,1,0,lanzaCliente1Diferido,base);
event_base_dispatch(base );

	 printf("--------------- FIN TEST2 -------------------\n");fflush(stdout);
printf("\n--------------- FIN TODOS LOS TEST -------------------\n");fflush(stdout);
	return(0);
}
