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

static void zlServerSocketAccept_cb(struct evconnlistener *listener ,
    evutil_socket_t fd , struct sockaddr *address , int socklen,
    void *ctx );
static void serverMainTimeoutBind(evutil_socket_t fd , short what, void *arg);





static void zlServerSocketAccept_cb(struct evconnlistener *listener ,
    evutil_socket_t fd , struct sockaddr *address , int socklen,
    void *ctx )
{
  /* We got a new connection! Set up a bufferevent for it. */
  zlConexionItem_t *conexionItem;
    LogW(9,"accept conexion 1\n" );
  zlServerSocket_t *serverMain =ctx;
  struct event_base *base = evconnlistener_get_base(listener );
  struct bufferevent *bev = bufferevent_socket_new(
      base , fd, BEV_OPT_CLOSE_ON_FREE);
  conexionItem=zlConexionItemCrea(serverMain,bev);
  if (serverMain->connect_i) serverMain->connect_i(conexionItem); 
  
    
  //set_tcp_no_delay(fd);
  LogW(9,"accept conexion 2\n" );
  

  //bufferevent_enable(bev , EV_READ|EV_WRITE);
}



/* event timeout callback bind */
static void serverMainTimeoutBind(evutil_socket_t fd , short what, void *arg)
{
  struct sockaddr_in sin ;
  zlServerSocket_t *serverMain = arg;
  /* Clear the sockaddr before using it, in case there are extra
   *          * platform-specific fields that can mess us up. */
  memset(&sin , 0 , sizeof (sin));
  /* This is an INET address */
  sin.sin_family = AF_INET;
  /* Listen on 0.0.0.0 */
  sin.sin_addr .s_addr = htonl(0 );
  /* Listen on the given puerto. */
  sin.sin_port = htons( (uint16_t)serverMain->puerto );
  serverMain->listener = evconnlistener_new_bind(serverMain->base , zlServerSocketAccept_cb, serverMain,
      LEV_OPT_CLOSE_ON_FREE |LEV_OPT_REUSEABLE, -1 ,
      (struct sockaddr*)&sin , sizeof (sin));
  if (!serverMain->listener) {
      LogW(9,"bind error\n");
	  if (serverMain->timeoutBind.tv_sec==0)
	     {if (serverMain->bind_i) serverMain->bind_i(0,serverMain);}
	  else	  
          evtimer_add (serverMain->evTimeoutBind, &serverMain->timeoutBind);
  } else {
      LogW(9,"bind correcto <%ld>\n",serverMain->puerto);
	   if (serverMain->bind_i) serverMain->bind_i(1,serverMain); 
      // Esperamos el accept, quizá s considerar el liberrar el timeout
  }
}


zlServerSocket_t * zlServerSocketInicia (int puerto ,int timeoutAccept,
          int timeoutCloseConexion,int registrar,
		  char *prefijoLog,
		  int levelLog,
		  zlServerSocketBindInterfaz_i bind_i,
		  zlServerSocketConnectInterfaz_i connect_i,
		  zlServerSocketReadInterfaz_i read_i,
		  zlServerSocketWriteInterfaz_i write_i,
		  zlServerSocketCloseInterfaz_i close_i,
		  void * tag )
{
  struct timeval tv ;    
  //struct sockaddr_in sin ;
  if (puerto<= 0 || puerto>65535) {
    //puts("Invalid puerto");
    return(NULL);
  }
  zlServerSocket_t *serverMain ;
  serverMain=(zlServerSocket_t *) malloc (sizeof(zlServerSocket_t ));
  memset(serverMain ,0, sizeof(zlServerSocket_t ));

  tv.tv_sec =timeoutAccept;
  tv.tv_usec =0;
  serverMain->timeoutBind = tv;
  tv.tv_sec= timeoutCloseConexion; // para evitar que se pille el socket
  tv.tv_usec=0;
  serverMain->timeoutCloseConexion=tv;
  serverMain->tag =tag;
  serverMain->puerto =puerto;
  serverMain->bind_i =bind_i;  
  serverMain->connect_i =connect_i;
  serverMain->read_i =read_i;
  serverMain->write_i =write_i;
  serverMain->registrar = registrar;
  serverMain->close_i =close_i;
  serverMain->tag =tag;
  strcpy(serverMain->prefijoLog,prefijoLog);
  serverMain->levelLog=levelLog;
  return serverMain ;
}

void zlServerSocketLanza(zlServerSocket_t *serverMain, struct event_base *base)
{ 
 
serverMain->base =base;

  struct timeval tv ;
 
  serverMain->evTimeoutBind =evtimer_new(base, serverMainTimeoutBind , serverMain);
  tv.tv_sec =0;
  tv.tv_usec =0;
  evtimer_add(serverMain ->evTimeoutBind, &tv ); // no persistente

  return;
}
void zlServerSocketFree(zlServerSocket_t *server)
{
	// borrar el evento timeout del bin
	if (server->evTimeoutBind) event_free(server->evTimeoutBind);
	evconnlistener_free(server->listener);
	free(server);
}
