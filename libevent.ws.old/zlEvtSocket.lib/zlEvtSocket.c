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

static void zlEvtSocketRead_cb(struct bufferevent *bev , void *tag);
static void zlEvtSocketEvent_cb(struct bufferevent *bev , short events, void *tag);

static void zlEvtSocketWrite_cb(struct bufferevent *bev, void *tag);
static void zlEvtSocketWriteClose_cb(struct bufferevent *bev, void *tag);

struct sockaddr *zlEvtSocketAddr ( struct sockaddr_in *adr ,char * ip, int puerto )
{
//  int socks ;
//int fd;
//  struct hostent * hostlocal ;
 // int ret ;
  unsigned long miip;
 // int lgadr = sizeof (*adr );
 memset (adr , 0, sizeof (*adr ));
 miip =inet_addr ( ip);            /*Direcc: Pasar formato puntos a long */
 adr ->sin_addr . s_addr= miip ;     /* /usr/include/netinet> in.h  in_f.h */
 adr ->sin_port = htons((uint16_t) puerto ); /*Num puerto  -->/etc/services */
 adr ->sin_family = AF_INET ;       /* Familia direccion --> dominio Internet */

  return ((struct sockaddr *)adr );
}

zlEvtSocket_t  *zlEvtSocketCreaCliente(
    struct event_base *base,
	char *ip,
	int puerto,
	zlEvtSocketList_t *lista,
	zlSocketConnect_i connect_i,
	zlSocketRead_i read_i,
	zlSocketClose_i close_i,
	zlSocketWrite_i write_i,
	short estadoEnable,
	void *tagConexion)
{
	struct bufferevent *evtBuffer;
	struct sockaddr_in sin ;
	int ret;
	LogW(9,"zlEvtSocketCrea\n");
	evtBuffer = bufferevent_socket_new (base , - 1, BEV_OPT_CLOSE_ON_FREE  );
    bufferevent_disable(evtBuffer , EV_READ|EV_WRITE);
    
    ret=bufferevent_socket_connect (evtBuffer , zlEvtSocketAddr (&sin , ip, puerto ), sizeof (sin ));
    if (ret<0)
	{
		LogW(9,"Socket cliente error\n");
		bufferevent_free(evtBuffer);
		return(NULL);
	}
	zlEvtSocket_t *evtSocket=malloc(sizeof(zlEvtSocket_t ));
	assert(evtSocket!=NULL);
	bzero(evtSocket,sizeof(zlEvtSocket_t));
	assert(read_i!=NULL);
	evtSocket->evtBuffer=evtBuffer;
	evtSocket->connect_i=connect_i;
	evtSocket->read_i=read_i;
	evtSocket->close_i=close_i;
	evtSocket->write_i=write_i;
	evtSocket->tagConexion=tagConexion;
	
	evtSocket->nItem = -1;
	evtSocket->nLog = -1;
	evtSocket->lista = lista;
	if (lista!=NULL)
		zlEvtSocketListAdd(lista,evtSocket);// anade nLog y nItem 
			 
		   bufferevent_setcb(evtBuffer , zlEvtSocketRead_cb, zlEvtSocketWrite_cb, 
                    zlEvtSocketEvent_cb, evtSocket);
	bufferevent_enable(evtBuffer , estadoEnable);	    
	return(evtSocket);
}
zlEvtSocket_t  *zlEvtSocketCreaServer(
    struct event_base *base,
	evutil_socket_t fd ,
	zlEvtSocketList_t *lista,	
	zlSocketRead_i read_i,
	zlSocketClose_i close_i,
	zlSocketWrite_i write_i,
	short estadoEnable,
	void *serverData,
	void *tagConexion)
{
	struct bufferevent *evtBuffer;

	
	LogW(9,"zlEvtSocketCrea\n");
	evtBuffer = bufferevent_socket_new (base , fd, BEV_OPT_CLOSE_ON_FREE  );
    bufferevent_disable(evtBuffer , EV_READ|EV_WRITE);
    

	zlEvtSocket_t *evtSocket=malloc(sizeof(zlEvtSocket_t ));
	assert(evtSocket!=NULL);
	bzero(evtSocket,sizeof(zlEvtSocket_t));
	assert(read_i!=NULL);
	evtSocket->evtBuffer=evtBuffer;
	evtSocket->nItem = -1;
	evtSocket->nLog = -1;
	evtSocket->lista = lista;
	if (lista!=NULL)
		zlEvtSocketListAdd(lista,evtSocket);// anade nLog y nItem 
	evtSocket->connect_i=NULL;
	evtSocket->read_i=read_i;
	evtSocket->close_i=close_i;
	evtSocket->write_i=write_i;
	evtSocket->tagConexion=tagConexion; // En teoria este es nulo, porque el recurso
	                                    // Se crea cuando llega la conexion
	evtSocket->serverData=serverData;

		   bufferevent_setcb(evtBuffer , zlEvtSocketRead_cb, zlEvtSocketWrite_cb, 
                    zlEvtSocketEvent_cb, evtSocket);
	bufferevent_enable(evtBuffer , estadoEnable);	    
	return(evtSocket);
}
static void zlEvtSocketRead_cb(struct bufferevent *bev , void  * tag)
{
	zlEvtSocket_t *socket= (zlEvtSocket_t *) tag;
	LgW(socket->nLog,9,"Llega read cb\n");fflush(stdout);
    	
  /* This callback is invoked when there is data to read on bev. */
  struct evbuffer *input = bufferevent_get_input(bev );
  socket->read_i(input,socket);
}
static void zlEvtSocketWrite_cb(struct bufferevent *bev, void *tag)
{
	zlEvtSocket_t *socket= (zlEvtSocket_t *) tag;
	if (socket->write_i!=NULL)
	  socket->write_i(socket);
}




static void zlEvtSocketWriteClose_cb(struct bufferevent *bev, void *tag)
{
		zlEvtSocket_t *socket= (zlEvtSocket_t *) tag;

	LogW(9,"Llega write cb\n");

zlEvtSocketCloseCommand(socket);
}



static void zlEvtSocketEvent_cb(struct bufferevent *bev , short events, void *tag)
{
	zlEvtSocket_t *socket= (zlEvtSocket_t *) tag;

    if ( events & BEV_EVENT_CONNECTED ) {
		// Por ahora no hagas nada
		LogW(9,"BEV_EVENT_CONNECTED\n");fflush(stdout);
		if (socket->connect_i!=NULL )socket->connect_i(socket);
        return; 
    }		
  if (events & (BEV_EVENT_EOF | BEV_EVENT_ERROR)) {
	  LgW(socket->nLog,9,"yEl evento es  BEV_EVENT_EOF | BEV_EVENT_ERROR\n");
	zlEvtSocketCloseCommand(socket); 

  }// evento de error
}
void zlEvtSocketCloseCommand(zlEvtSocket_t *socket)
{

	if (socket->close_i!=NULL )socket->close_i(socket);
		if(socket->lista!=NULL)
			zlEvtSocketListFreeItem(socket->lista,socket->nItem);
	  bufferevent_free(socket->evtBuffer);
      free(socket);

}
void zlEvtSocketWriteCommand(int closeOnWrite,zlEvtSocket_t *socket,
     char *buf,
     int len)
{

	if (buf!=NULL && len!=0) 
	 {
	   LogW(9,"ajo y agua<%d> <%.*s>\n",len,len,buf);fflush(stdout);
	   //LgW(socket->nLog,9,"pero bueno\n");fflush(stdout);
	   struct evbuffer *output = bufferevent_get_output(socket->evtBuffer );
	   LogW(9,"ajo y agua 1\n");fflush(stdout);
	   evbuffer_add(output,buf, (size_t) len);
	   LogW(9,"ajo y agua 1\n");fflush(stdout); 
	 }
	 
	if (closeOnWrite==1)
	{ // Si escribimos sin evento write, no hay forma humana de saber si hemos acabado 
      // la escritura si lo que queremos es cerrar, por lo tanto esto nos sirve
		  //conexionItem->closeOnWrite=1;
	   bufferevent_setcb(socket->evtBuffer , NULL, zlEvtSocketWriteClose_cb, 
                    zlEvtSocketEvent_cb,socket);

		  bufferevent_setwatermark(socket->evtBuffer, EV_WRITE, 0, 0);// que nos avise cuando ya está enviado ( 0 lower mark)
		  bufferevent_enable(socket->evtBuffer , EV_WRITE);
		  bufferevent_disable(socket->evtBuffer,EV_READ);
		return;
	}
	
}

