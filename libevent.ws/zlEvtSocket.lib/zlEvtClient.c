#include <event2/listener.h>
#include <event2/bufferevent.h>
#include <event2/buffer.h>
#include <dirent.h>
#include <arpa/inet.h>
#include <netinet/tcp.h>

#include <signal.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "zlStd.h"
#include <zlListas.h>
#include "zlEvtTimer.h"
#include "zlEvtSocketComun.h"
#include "zlEvtClient.h"


//Callback de desconexion informado por socketComun : ver cancel_cb

void zlEvtClientDisconectConexion(zlEvtClient_t *client,long canal)
{
zlListapFreeItem(client->conexiones ,  canal);
client->client_cb(CLIENTE_BASIC_DISCONNECT,client,client->tag);
}






zlEvtClient_t *zlEvtClientInicia (
          struct event_base *base
         , char *ip
         ,int puerto 
         ,zlEvtClient_i client_cb
         ,zlEvtSocket_i socket_cb
         ,void * tag )
{
  struct timeval tv ;
  //struct sockaddr_in sin ;
  if (puerto<= 0 || puerto>65535) {
    //puts("Invalid puerto");
    return(NULL);
  }
  zlEvtClient_t *client;
  client=(zlEvtClient_t *) malloc (sizeof(zlEvtClient_t ));
  memset(client ,0, sizeof(zlEvtClient_t ));
  client->conexiones=zlListapCrea("clientConexiones",NULL);
  strcpy(client->ip,ip);
  client->tag =tag;
  client->puerto =puerto;
  client->client_cb =client_cb;
  client->socket_cb = socket_cb;
  client->tag =tag;
  client->base =base;

  return client ;
}

zlEvtSocket_t *zlEvtClientCommandConnect(zlEvtClient_t *client)
{
zlEvtSocket_t  *socket= zlEvtSocketClienteConnect(
               client->base
             , client->ip
             , client->puerto
             , client->socket_cb
             , client
             , zlEvtClientDisconectConexion
             , client->tag);
 socket->canal=zlListapUpdateOrAppend(client->conexiones,socket);
 return(socket);
}



long zlEvtSocketClientNumConexiones(zlEvtClient_t *client)
{
 return(client->conexiones->numActivos);


}

