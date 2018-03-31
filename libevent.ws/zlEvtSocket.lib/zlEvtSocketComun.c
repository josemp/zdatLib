
#include <event2/event.h>
#include <event2/buffer.h>
#include <event2/bufferevent.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <errno.h>
#include <dirent.h>
#include <fcntl.h>

#include <string.h>
#include <zlEventMain.h>

#include "zlEvtTimer.h"
#include "zlStd.h"
#include "zlEvtServer.h"
#include "zlEvtSocketComun.h"


void zlEvtSocketTimersFree(zlEvtSocket_t *socket);

// CALLBACKS INTERNOS


void zlEvtSocketRead_cb (struct bufferevent *bev , void * ptr )
{
printf("zlEvtSocketRead_cb\n");
zlEvtSocket_t *socket=ptr;
  /* This callback is invoked when there is data to read on bev. */
  struct evbuffer *input = bufferevent_get_input(bev );
  socket->socket_cb(ZLEVT_SOCKET_DATOS,input,0,socket,socket->tag);
}

void zlEvtSocketWrite_cb(struct bufferevent *bev, void *ptr)
{
printf("zlEvtSocketWrite_cb\n");
        zlEvtSocket_t *socket=ptr;
        socket->socket_cb(ZLEVT_SOCKET_WRITE,NULL,0,socket,socket->tag);

}

void zlEvtSocketEvent_cb (struct bufferevent *bev , short events , void * ptr)
{
printf("zlEvtSocketEvent_cb\n");fflush(stdout);
   zlEvtSocket_t *socket=ptr;

zlEventDescBevEvent (events );

    if ( events & BEV_EVENT_CONNECTED ) 
{
printf ("conectamos\n");
                socket->socket_cb(ZLEVT_SOCKET_CONNECT,NULL,0,socket,socket->tag);
}

     if (events & ( BEV_EVENT_ERROR |BEV_EVENT_EOF )) {
   printf("por aqui no pasamos\n");
         if (socket->isConnected == zlTrue)
            socket->socket_cb(ZLEVT_SOCKET_FIN,NULL,0,socket,socket->tag);
         else
            socket->socket_cb(ZLEVT_SOCKET_RECHAZADO,NULL,0,socket,socket->tag);
         zlEvtSocketFree(socket);
         
    }
printf("ya no hay evento\n");
}
void  zlEvtSocketFree(zlEvtSocket_t *socket)
{
        printf("Liberando socketr\n");
        if (socket->server!=NULL)
             socket->server->numConexiones--;
        bufferevent_free(socket->buffer);
       // free(socket->socketParam);// lo hemos eliminado
        zlEvtSocketTimersFree(socket);
        free(socket);
}


// Conecta cliente

zlEvtSocket_t  * zlEvtSocketClienteConnect(
               struct event_base *base
             , char *ip
             , int puerto
             , zlEvtSocket_i socket_cb
             , void *tag)
{
printf("CONECTO CLIENTE");fflush(stdout);
zlEvtSocket_t *socket=malloc(sizeof(zlEvtSocket_t));
memset(socket,0,sizeof(zlEvtSocket_t));
  socket->base=base;
  struct sockaddr_in sin ;
  socket ->puerto = puerto;
  strcpy (socket -> ip, ip );
  socket->socket_cb=socket_cb;
  socket->tag=tag;
  socket->isConnected=zlFalse;

// bufferevent sin fd(-1)
  socket->buffer = bufferevent_socket_new (  base , - 1, BEV_OPT_CLOSE_ON_FREE  );
// callbacks

      bufferevent_setcb(
                 socket->buffer
               , zlEvtSocketRead_cb
               , zlEvtSocketWrite_cb
               , zlEvtSocketEvent_cb
               , socket);
//    evbuffer_add_printf(bufferevent_get_output(buffer), "GET %s\r\n", "!que pasa pues\n");
bufferevent_socket_connect (socket->buffer , zlEventSockaddr (&sin ,socket -> ip, socket ->puerto ), sizeof (sin ));
 bufferevent_enable ( socket->buffer, EV_READ |EV_WRITE );


  return (socket );
}



// SERVER CONNECT

zlEvtSocket_t  * zlEvtSocketServerConnect(
                   struct event_base *base
                  ,zlEvtServerSocketParam_t *socketParam
                  ,zlEvtSocket_i socket_cb
                  ,zlEvtServer_t *server
                  , void *tag
                  )
{

// inicia estructura
zlEvtSocket_t *socket=malloc(sizeof(zlEvtSocket_t));
memset(socket,0,sizeof(zlEvtSocket_t));

// set datos
// socket->socketParam=socketParam;
 socket->base = base;
 socket->server = server;
socket->socket_cb=socket_cb;
socket->tag=tag;
socket->fd=socketParam->fd;
socket->isConnected=zlTrue;

 socket->buffer = bufferevent_socket_new(
      socket->base , socketParam->fd, BEV_OPT_CLOSE_ON_FREE);
 bufferevent_enable ( socket->buffer, EV_READ |EV_WRITE );
      bufferevent_setcb(
                 socket->buffer
               , zlEvtSocketRead_cb
               , zlEvtSocketWrite_cb
               , zlEvtSocketEvent_cb
               , socket);
//      bufferevent_enable(socket->buffer , EV_READ);
 socket->socket_cb(ZLEVT_SOCKET_CONNECT,NULL,0,socket,socket->tag);
 socket->canal=server->numConexiones;
 server->numConexiones=server->numConexiones+1;
 free(socketParam);
return(socket);

}



void zlEvtSocketWriteCommandCierre(zlEvtSocket_t *socket,char *buf,int len,int cierraAlTerminar)
{
        if (socket->buffer==NULL) return;//El socket ya se ha cerrado
        struct evbuffer *output = bufferevent_get_output(socket->buffer );
        printf("Enviando <%.*s>\n",len,buf);
        evbuffer_add(output,buf, len);
        socket->closeOnWrite=cierraAlTerminar;


}
void zlEvtSocketWriteCommand(zlEvtSocket_t *socket,char *buf,int len)
{
        zlEvtSocketWriteCommandCierre(socket,buf, len,0);
}


/* ------ Timer section ------ */

/*
#define SERVER_BASIC_NUM_TIMERS 10
zlEvtSocketTimer_t timer[SERVER_BASIC_NUM_TIMERS]; 
typedef struct
{
int numero;// iniciarlas con el número
int tiempo;
zlEvtTimer_t *timer;
struct zlEvtSocket_s *socket;
} zlEvtSocketTimer_t;
*/

void zlEvtSocketTimersInicializa(zlEvtSocket_t *socket)
{
 int i;
zlEvtSocketTimer_t *timer;
 for (i=0;i<SERVER_BASIC_NUM_TIMERS;i++)
  {
    timer=&socket->timer[i];
    memset(timer,0,sizeof(zlEvtSocketTimer_t));
    timer->numero=i;
    timer->socket=socket;
  }
}
void zlEvtSocketTimersFree(zlEvtSocket_t *socket)
{
 int i;
zlEvtSocketTimer_t *timer;
 for (i=0;i<SERVER_BASIC_NUM_TIMERS;i++)
  {
    timer=&socket->timer[i];
    if (timer->timer!=NULL)
     {
      zlEvtTimerFree(timer->timer);
      timer->timer=NULL;
     }
  }
}
void zlEvtSocketTimer_cb(void *cls)
{

LogW(9,"llega timer\n");
zlEvtSocketTimer_t *timer = cls;
LogW(9,"llega timer1\n");
zlEvtSocket_t *socket=timer->socket;
LogW(9,"llega timer2\n");
zlEvtTimerFree(timer->timer);
LogW(9,"llega timer3\n");
timer->timer=NULL;
LogW(9,"llega timer4\n");
socket->socket_cb(ZLEVT_SOCKET_TIMEOUT,NULL,timer->numero,socket,socket->tag);
LogW(9,"llega timer5\n");

}

void zlEvtSocketTimerActiva(zlEvtSocket_t *socket,int num,int tiempo,void *tag)
{
zlEvtSocketTimer_t *timer=&socket->timer[num];
timer->tag=tag;
timer->tiempo=tiempo;
timer->socket=socket;
LogW(9,"Activando timer\n");
timer->timer=zlEvtTimerNuevoActiva(
                     socket->base
                    ,tiempo,0
                    ,zlEvtSocketTimer_cb,timer);
}
