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
#include "zlEvtTimer.h"
#include "zlStd.h"
#include "zlEvtServer.h"
#include "zlEvtSocketComun.h"

static void zlEvtServerTimeoutBind(evutil_socket_t fd , short what, void *arg);
zlBool_t zlEvtServerBind(zlEvtServer_t *server);

static void zlEvtServerAccept_cb(struct evconnlistener *listener ,
    evutil_socket_t fd , struct sockaddr *address , int socklen,
    void *ctx )
{
  /* We got a new connection! Set up a bufferevent for it. */
    printf("accept conexion 1\n" );
    zlEvtServer_t *server =ctx;
//  struct event_base *base = evconnlistener_get_base(listener );
//  struct bufferevent *bev = bufferevent_socket_new(
//      base , fd, BEV_OPT_CLOSE_ON_FREE);
//  conexionItem=zlConexionItemCrea(serverMain,bev);
//  if (serverMain->connect_i) serverMain->connect_i(conexionItem);


// La estructura
printf("estructura socket param\n");fflush(stdout);
zlEvtServerSocketParam_t *socketParam;// t-q-l
socketParam=malloc(sizeof(zlEvtServerSocketParam_t));
memset(socketParam,0,sizeof(zlEvtServerSocketParam_t));
socketParam->fd=fd;
socketParam->address=address;
socketParam->socklen=socklen;
socketParam->listener=listener;

printf("fin estructura socket param\n");fflush(stdout);
     server->server_cb(SERVER_BASIC_CONNECT,server,zlTrue,socketParam,server->tag);
printf("fin  llamada connect\n");fflush(stdout);

  //set_tcp_no_delay(fd);
  LogW(9,"accept conexion 2\n" );


  //bufferevent_enable(bev , EV_READ|EV_WRITE);
}
/*
commandConnect(zlEvtSocket_t *socket)
{
 struct bufferevent *bev = bufferevent_socket_new(
      socket->base , socket->fd, BEV_OPT_CLOSE_ON_FREE);
      bufferevent_setcb(bev , zlEvtServerSocketRead_cb, zlEvtServerSocketWrite_cb,
                    zlEvtServerSocketEvent_cb, conexionItem);
      bufferevent_enable(bev , EV_READ);



}
*/
zlEvtServer_t *zlEvtServerInicia (
          struct event_base *base
         ,int puerto 
         ,int timeoutAccept
         ,zlEvtServer_i server_cb
         ,void * tag )
{
  struct timeval tv ;
  //struct sockaddr_in sin ;
  if (puerto<= 0 || puerto>65535) {
    //puts("Invalid puerto");
    return(NULL);
  }
  zlEvtServer_t *server;
  server=(zlEvtServer_t *) malloc (sizeof(zlEvtServer_t ));
  memset(server ,0, sizeof(zlEvtServer_t ));

  tv.tv_sec =timeoutAccept;
  tv.tv_usec =0;
  server->timeoutBind = tv;
  server->tag =tag;
  server->puerto =puerto;
  server->server_cb =server_cb;
  server->tag =tag;
  server->base =base;

  server->evTimeoutBind =evtimer_new(base, zlEvtServerTimeoutBind , server);

  if (zlEvtServerBind(server)==zlFalse)
    {
      free(server);
      return(NULL);
    }
  
  return server ;
}
zlBool_t zlEvtServerBind(zlEvtServer_t *server)
{
  struct sockaddr_in sin ;
  /* Clear the sockaddr before using it, in case there are extra
   *          * platform-specific fields that can mess us up. */
  memset(&sin , 0 , sizeof (sin));
  /* This is an INET address */
  sin.sin_family = AF_INET;
  /* Listen on 0.0.0.0 */
  sin.sin_addr .s_addr = htonl(0 );
  /* Listen on the given puerto. */
  printf("puerto <%d>\n",server->puerto);
  sin.sin_port = htons( (uint16_t)server->puerto );

  server->listener = evconnlistener_new_bind(server->base , zlEvtServerAccept_cb, server,
      LEV_OPT_CLOSE_ON_FREE |LEV_OPT_REUSEABLE, -1 ,
      (struct sockaddr*)&sin , sizeof (sin));



  if (!server->listener) {
      LogW(9,"bind error\n");
      if (server->timeoutBind.tv_sec<=0)
           return(zlFalse);
      server->server_cb(SERVER_BASIC_BIND,server,zlFalse,NULL,server->tag);
      evtimer_add (server->evTimeoutBind, &server->timeoutBind);
      return(zlTrue); // No ha habido bind, pero lo vamos a reintentar
  } else {
      LogW(9,"bind correcto <%ld>\n",server->puerto);
       server->server_cb(SERVER_BASIC_BIND,server,zlTrue,NULL,server->tag);
      // Esperamos el accept, quizá s considerar el liberrar el timeout
      return(zlTrue);
  }

}
/* event timeout callback bind */
static void zlEvtServerTimeoutBind(evutil_socket_t fd , short what, void *arg)
{
  zlEvtServer_t *server = arg;
  zlEvtServerBind(server);
}
