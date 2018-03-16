#include <event2/listener.h>
#include <event2/bufferevent.h>
#include <event2/buffer.h>

#include <arpa/inet.h>
#include <netinet/tcp.h>

#include <signal.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include "zlLog.h"
#include "zlEvtSocket.h"
#include "zlEvtSocketServer.h"

static void zlEvtSocketAccept_cb(struct evconnlistener *listener ,
    evutil_socket_t fd , struct sockaddr *address , int socklen,
    void *tag );
static void zlEvtSocketTimeoutBind_cb(evutil_socket_t fd , short what, void *arg);
	
static void zlEvtSocketAccept_cb(struct evconnlistener *listener ,
    evutil_socket_t fd , struct sockaddr *address , int socklen,
    void *tag )
{
  zlEvtSocketServerData_t  *serverData =( zlEvtSocketServerData_t  *) tag ; 
   struct event_base *base = evconnlistener_get_base(listener );
    zlEvtSocket_t *evtSocket=zlEvtSocketCreaServer(
    base,
	fd ,
	serverData->lista,	
	serverData->read_i,
	serverData->close_i,
	serverData->write_i,
	serverData->estadoEnable,
	serverData,
	NULL);// El recurso de conexion habra de escribirse en el evento connect_i si procede
  if (serverData->connect_i!=NULL) serverData->connect_i(evtSocket); 
  
    
  //set_tcp_no_delay(fd);
  LogW(9,"accept conexion 2\n" );
  

  //bufferevent_enable(bev , EV_READ|EV_WRITE);
}

/* ---------------------------------------- */
/* Cerrar el server                         */
/* Solamente no se permiten mas conexiones  */
/* Las conexiones activas segirán           */
/* ---------------------------------------- */
void zlEvtSocketServerFree(zlEvtSocketServerData_t  *serverData)
{
	// borrar el evento timeout del bin
	if (serverData->evTimeoutBind) event_free(serverData->evTimeoutBind);
	evconnlistener_free(serverData->listener);
}	
static void zlEvtSocketTimeoutBind_cb(evutil_socket_t fd , short what, void *arg)
{
	LogW(9,"LLega el timeout bind\n");fflush(stdout);
  struct sockaddr_in sin ;
  zlEvtSocketServerData_t  *serverData = arg;
  /* Clear the sockaddr before using it, in case there are extra
   *          * platform-specific fields that can mess us up. */
  memset(&sin , 0 , sizeof (sin));
  /* This is an INET address */
  sin.sin_family = AF_INET;
  /* Listen on 0.0.0.0 */
  sin.sin_addr .s_addr = htonl(0 );
  /* Listen on the given puerto. */
  sin.sin_port = htons( (uint16_t)serverData->puerto );
  serverData->listener = evconnlistener_new_bind(serverData->base , 
                         zlEvtSocketAccept_cb, serverData,
                         LEV_OPT_CLOSE_ON_FREE |LEV_OPT_REUSEABLE, -1 ,
                         (struct sockaddr*)&sin , sizeof (sin));
  if (!serverData->listener) {
      LogW(9,"bind error\n");fflush(stdout);
	  if (serverData->timeoutBind.tv_sec==0)
	     {if (serverData->bind_i!=NULL) serverData->bind_i(0,serverData);}
	  else	  
          evtimer_add (serverData->evTimeoutBind, &serverData->timeoutBind);
  } else {
      LogW(9,"bind correcto <%ld>\n",serverData->puerto);fflush(stdout);
	   if (serverData->bind_i!=NULL) serverData->bind_i(1,serverData); 
      // Esperamos el accept, quizá s considerar el liberrar el timeout
  }
}


/* La mayoría de estos datos son para iniciar las conexiones con zlEvtSocket.c */
void zlEvtSocketServerData(zlEvtSocketServerData_t *serverData,
					int puerto,
					int timeoutBind,
					zlSocketBind_i bind_i,
					zlSocketConnect_i connect_i,
					zlSocketRead_i read_i,
					zlSocketClose_i close_i,
					zlSocketWrite_i write_i,
					zlEvtSocketList_t *lista,
					void * tag)
{
    struct timeval tv ;
  tv.tv_sec =timeoutBind;
  tv.tv_usec =0;
  serverData->timeoutBind = tv;
  serverData->bind_i =bind_i;  
  serverData->connect_i =connect_i;
  serverData->read_i =read_i;
  serverData->write_i =write_i;
  serverData->close_i =close_i;
  assert(puerto>0);
  serverData->puerto=puerto;
  serverData->tag=tag;
  serverData->lista=lista;
}

/* Aquí realmente lanzamos el servidor mediante un timeout */
void zlServerSocketLanza(zlEvtSocketServerData_t *serverData, struct event_base *base, short estadoEnable)
{ 
  struct timeval tv ; // el primero se lanza con timeout 0
  serverData->estadoEnable=estadoEnable;
  serverData->base=base;
   serverData->evTimeoutBind =evtimer_new(base, zlEvtSocketTimeoutBind_cb , serverData);
   tv.tv_sec=0;
   tv.tv_usec=0;
  evtimer_add(serverData->evTimeoutBind, &tv ); // no persistente

  return;
}
