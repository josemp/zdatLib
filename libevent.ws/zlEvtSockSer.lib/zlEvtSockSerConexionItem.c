#include <event2/listener.h>
#include <event2/bufferevent.h>
#include <event2/buffer.h>

#include <arpa/inet.h>
#include <netinet/tcp.h>

#include <signal.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "zlLog.h"
#include "zlEvtSockSer.h"
// En C, una función estatica, simplemente limita su ambito al programa (no puede usarse en otros objetos y librerias)
static void zlServerSocketRead_cb(struct bufferevent *bev , void *ctx);//zlConexionItem_t *
static void zlServerSocketEvent_cb(struct bufferevent *bev , short events, void * ctx);	//zlConexionItem_t * 

static void zlServerSocketWrite_cb(struct bufferevent *bev, void *ptr);
static void zlServerSocketWriteClose_cb(struct bufferevent *bev, void *ptr);


// Usar como LgW(zlcolog(conexionItem),"hola amigos\n"); 
int zlclog(zlConexionItem_t *conexionItem)
{
    if (conexionItem->server->registrar==0) return(-1);
	if (conexionItem->server->prefijoLog==NULL) return(-1); // log normal
    if (strlen(conexionItem->server->prefijoLog)==0) return(-1);
	return(conexionItem->server->registroConexion.conexion[conexionItem->nItem].nLog);
}
void *zlConexionItemGetTag(zlConexionItem_t *conexionItem)
{
	return(conexionItem->server->tag);
}
void *zlConexionItemGetConexionTag(zlConexionItem_t *conexionItem)
{
	return(conexionItem->conexionTag);
}
void *zlConexionItemGetProgTag(zlConexionItem_t *conexionItem)
{
	return(conexionItem->progTag);
}
zlConexionItem_t *zlConexionItemCrea(zlServerSocket_t *server,
                                 struct bufferevent *bev)
{
	LogW(9,"zlConexionItemCrea registrar=<%p>\n",bev);
	zlConexionItem_t *conexionItem=malloc(sizeof(zlConexionItem_t ));
	memset(conexionItem,0,sizeof(zlConexionItem_t));
			conexionItem->nItem = -1;
			conexionItem->server=server;
			conexionItem->bev=bev;
			//conexionItem->closeOnWrite=0;
			
            if (server->registrar==1)
			 {
               zlRegistroConexionAdd(conexionItem);


 
			 }
		   bufferevent_setcb(bev , zlServerSocketRead_cb, zlServerSocketWrite_cb, 
                    zlServerSocketEvent_cb, conexionItem);
			bufferevent_enable(bev , EV_READ);
	return(conexionItem);
}


void zlConexionCommandClose(zlConexionItem_t *conexionItem)
{
	 zlServerSocket_t *serverMain=conexionItem->server;
        int nItem=conexionItem->nItem;  
      zlConexionItemFree(conexionItem);
      if (serverMain->registrar==1 && nItem>=0) // Está registrado  
	    zlRegistroConexionItemFree(&serverMain->registroConexion,nItem);

  LogW(9,"Command Close\n");
}


void zlConexionItemFree(zlConexionItem_t *conexionItem)
{
	bufferevent_free(conexionItem->bev);
	
	free(conexionItem);	
}


static void zlServerSocketRead_cb(struct bufferevent *bev , void *ctx)//zlConexionItem_t * 
{
	LogW(9,"Llega read cb\n");fflush(stdout);
    zlConexionItem_t *conexionItem=ctx;
	//if (conexionItem->progTag!=NULL) return;// ya hay algo en proceso
	//if (conexionItem->closeOnWrite==1) return;//Orden del cliente de cerrar
    zlServerSocket_t *serverMain = conexionItem->server;
	
  /* This callback is invoked when there is data to read on bev. */
  struct evbuffer *input = bufferevent_get_input(bev );
  serverMain->read_i(ZL_CI_DATOS,input,conexionItem);
}
static void zlServerSocketWrite_cb(struct bufferevent *bev, void *ptr)
{
	zlConexionItem_t *conexionItem=ptr;
	zlServerSocket_t *serverMain =conexionItem->server;
	if (serverMain->write_i != NULL) serverMain->write_i(conexionItem);
}
static void zlServerSocketWriteClose_cb(struct bufferevent *bev, void *ptr)
{
	LogW(9,"Llega write cb\n");
	zlConexionItem_t *conexionItem=ptr;
	zlServerSocket_t *serverMain =conexionItem->server;
	if (serverMain->close_i!=NULL )serverMain->close_i(conexionItem);
      int nItem=conexionItem->nItem;  
      zlConexionItemFree(conexionItem);
      if (serverMain->registrar==1 && nItem>=0) // Está registrado  
	    zlRegistroConexionItemFree(&serverMain->registroConexion,nItem);
}
/*
void zlServerSocketWriteAllCommand(int closeOnWrite,zlServerSocket_t *server,
     char *buf,
     int len)
{
	 int i;
	 zlRegistroConexion_t *conexion=&server->registroConexion;
	for (i=0;i<conexion->numConexiones;i++)
	{
		if (conexion->item[i]!=NULL)
		{		
			zlServerSocketWriteCommand(closeOnWrite,conexion->item[i],
            buf,len,NULL);

		}
	}
	
}
*/
void zlServerSocketWriteCommand(int closeOnWrite,zlConexionItem_t *conexionItem,
     char *buf,
     int len, void *progTag)
{

	conexionItem->progTag=progTag;
	if (buf!=NULL && len!=0) 
	 {
	

	   struct evbuffer *output = bufferevent_get_output(conexionItem->bev );
	   evbuffer_add(output,buf, (size_t) len);
	 }
	 
	if (closeOnWrite==1)
	{ // Si escribimos sin evento write, no hay forma humana de saber si hemos acabado 
      // la escritura si lo que queremos es cerrar, por lo tanto esto nos sirve
		  //conexionItem->closeOnWrite=1;
		  bufferevent_setcb(conexionItem->bev , NULL, zlServerSocketWriteClose_cb, 
                    zlServerSocketEvent_cb, conexionItem); // No permitimos lectura
		  bufferevent_setwatermark(conexionItem->bev, EV_WRITE, 0, 0);// que nos avise cuando ya está enviado ( 0 lower mark)
		  bufferevent_enable(conexionItem->bev , EV_WRITE);
		return;
	}
	
}
static void zlServerSocketEvent_cb(struct bufferevent *bev , short events, void * ctx)//zlConexionItem_t * 
{
    LogW(9,"Llega evento\n");fflush(stdout);
	zlConexionItem_t *conexionItem=ctx;
    zlServerSocket_t *serverMain =conexionItem->server;
    //struct evbuffer *output = bufferevent_get_output(bev );
    //size_t remain = evbuffer_get_length (output);
  
  if (events & (BEV_EVENT_EOF | BEV_EVENT_ERROR)) {
	  LogW(9,"El evento es  BEV_EVENT_EOF | BEV_EVENT_ERROR\n");
	 
	  serverMain->close_i(conexionItem);
      int nItem=conexionItem->nItem;  
      zlConexionItemFree(conexionItem);
      if (serverMain->registrar==1 && nItem>=0) // Está registrado  
	    zlRegistroConexionItemFree(&serverMain->registroConexion,nItem);
	
	 
/*	 
	// porque proTag ha de ser !=NULL  ?
    //if (conexionItem->closeOnWrite==0 && conexionItem->progTag!=NULL)	
      serverMain->read_i(ZL_CI_CIERRE, NULL, conexionItem);
      int nItem=conexionItem->nItem;  
      zlConexionItemFree(conexionItem);
      if (serverMain->registrar==1 && nItem>=0) // Está registrado  
	    zlRegistroConexionItemFree(&serverMain->registroConexion,nItem);
*/		
  }// evento de error
}