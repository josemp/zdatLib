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
#include "zlEvtSocketServer.h"
static void connectServer1_cb(struct zlEvtSocket *socket)
{
	printf("llega conexion Server \n");fflush(stdout);
}
//Eco
static void readServer1_cb(struct evbuffer *input,
                    struct zlEvtSocket *socket)
					{
						char aux[101];
						printf("llega lectura\n");fflush(stdout);
						//int lenMsg= (int) evbuffer_get_length(input);
	                    int ret=evbuffer_remove(input, aux,(size_t) 100 );// Leemos 100 del buffer
						assert(ret>0);
						printf("Leidos <%d><%.*s>\n",ret,ret,aux);fflush(stdout);
						
						if (strncmp(aux,"finServer",9)==0)
						{
							//zlEvtSocketCloseCommand(socket);
							//Liberamos el servidor
							zlEvtSocketServerFree((zlEvtSocketServerData_t  *) socket->serverData);
							zlEvtSocketWriteCommand(0,socket,"cierrate",8);
							return;
                        }
						zlEvtSocketWriteCommand(0,socket,aux,ret);
					}
static void closeServer1_cb(struct zlEvtSocket *socket)
{
	printf("llega close\n");fflush(stdout);
}

static void writeServer1_cb(struct zlEvtSocket *socket)
{
	printf("llega write\n");fflush(stdout);
}

static void bindServer1_cb(int estado, zlEvtSocketServerData_t *serverData)
{
	printf("bind estado <%d>\n",estado);fflush(stdout);
}
zlEvtSocketServerData_t dataServer1;
zlEvtSocketList_t listaServer1;

void testServer1(struct event_base *base,int puerto)
{

listaServer1.numConexiones=0;
strcpy(listaServer1.prefijoLog,"server");
listaServer1.levelLog=9;
zlEvtSocketServerData(&dataServer1,
					puerto,
					3,// timeoutBind,
					bindServer1_cb,
					connectServer1_cb,
					readServer1_cb,
					closeServer1_cb,
					writeServer1_cb,
					&listaServer1,
					NULL);
LogW(9,"Lanzando el servidor\n");fflush(stdout);					
zlServerSocketLanza(&dataServer1, base, EV_READ|EV_WRITE);					  
	
	
}