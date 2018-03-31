
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
#include "zlEvtServerBasic.h"
#include "zlEvtServerBasicSocket.h"


void zlEvtServerBasicTimersFree(zlEvtServerBasicSocket_t *socket);




void zlEvtServerBasicSocketRead_cb (struct bufferevent *bev , void * ptr )
{
zlEvtServerBasicSocket_t *socket=ptr;
  /* This callback is invoked when there is data to read on bev. */
  struct evbuffer *input = bufferevent_get_input(bev );
  socket->socket_cb(SERVER_BASIC_SOCKET_DATOS,input,0,socket,socket->tag);
}

void zlEvtServerBasicSocketWrite_cb(struct bufferevent *bev, void *ptr)
{
        zlEvtServerBasicSocket_t *socket=ptr;
        socket->socket_cb(SERVER_BASIC_SOCKET_WRITE,NULL,0,socket,socket->tag);

}

void zlEvtServerBasicSocketEvent_cb (struct bufferevent *bev , short events , void * ptr)
{
   zlEvtServerBasicSocket_t *socket=ptr;
zlEventDescBevEvent (events );

     if (events & ( BEV_EVENT_ERROR |BEV_EVENT_EOF )) {
         socket->socket_cb(SERVER_BASIC_SOCKET_FIN,NULL,0,socket,socket->tag);
         zlEvtServerBasicSocketFree(socket);
         
    }
}
void  zlEvtServerBasicSocketFree(zlEvtServerBasicSocket_t *socket)
{
        bufferevent_free(socket->buffer);
        free(socket->socketParam);
        zlEvtServerBasicTimersFree(socket);
        free(socket);
}

zlEvtServerBasicSocket_t  *zlEvtServerBasicSocketConnect(
                  zlServerBasicSocketParam_t *socketParam
                  ,zlServerBasicSocket_i socket_cb
                  ,zlServerBasic_t *server
                  , void *tag
                  )
{

// inicia estructura
zlEvtServerBasicSocket_t *socket=malloc(sizeof(zlEvtServerBasicSocket_t));
memset(socket,0,sizeof(zlEvtServerBasicSocket_t));

// set datos
 socket->socketParam=socketParam;
 socket->base = server->base;
 socket->server = server;
socket->socket_cb=socket_cb;
socket->tag=tag;
 socket->buffer = bufferevent_socket_new(
      socket->base , socket->socketParam->fd, BEV_OPT_CLOSE_ON_FREE);
      bufferevent_setcb(
                 socket->buffer
               , zlEvtServerBasicSocketRead_cb
               , zlEvtServerBasicSocketWrite_cb
               , zlEvtServerBasicSocketEvent_cb
               , socket);
      //bufferevent_enable(bev , EV_READ);
 bufferevent_enable ( socket->buffer, EV_READ |EV_WRITE );
 socket->socket_cb(SERVER_BASIC_SOCKET_CONNECT,NULL,0,socket,socket->tag);

return(socket);

}



void zlEvtServerBasicSocketWriteCommandCierre(zlEvtServerBasicSocket_t *socket,char *buf,int len,int cierraAlTerminar)
{
        if (socket->buffer==NULL) return;//El socket ya se ha cerrado
        struct evbuffer *output = bufferevent_get_output(socket->buffer );
        evbuffer_add(output,buf, len);
        socket->closeOnWrite=cierraAlTerminar;


}
void zlEvtServerBasicSocketWriteCommand(zlEvtServerBasicSocket_t *socket,char *buf,int len)
{
        zlEvtServerBasicSocketWriteCommandCierre(socket,buf, len,0);
}


/* ------ Timer section ------ */

/*
#define SERVER_BASIC_NUM_TIMERS 10
zlEvtServerBasicTimer_t timer[SERVER_BASIC_NUM_TIMERS]; 
typedef struct
{
int numero;// iniciarlas con el número
int tiempo;
zlEvtTimer_t *timer;
struct zlEvtServerBasicSocket_s *socket;
} zlEvtServerBasicTimer_t;
*/

void zlEvtServerBasicTimersInicializa(zlEvtServerBasicSocket_t *socket)
{
 int i;
zlEvtServerBasicTimer_t *timer;
 for (i=0;i<SERVER_BASIC_NUM_TIMERS;i++)
  {
    timer=&socket->timer[i];
    memset(timer,0,sizeof(zlEvtServerBasicTimer_t));
    timer->numero=i;
    timer->socket=socket;
  }
}
void zlEvtServerBasicTimersFree(zlEvtServerBasicSocket_t *socket)
{
 int i;
zlEvtServerBasicTimer_t *timer;
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
void zlEvtServerBasicTimer_cb(void *cls)
{
zlEvtServerBasicTimer_t *timer = cls;
zlEvtServerBasicSocket_t *socket=timer->socket;
zlEvtTimerFree(timer->timer);
timer->timer=NULL;
socket->socket_cb(SERVER_BASIC_SOCKET_TIMEOUT,NULL,timer->numero,socket,socket->tag);

}

void zlEvtServerBasicTimerActiva(zlEvtServerBasicSocket_t *socket,int num,int tiempo,void *tag)
{
zlEvtServerBasicTimer_t *timer=&socket->timer[num];
timer->tag=tag;
timer->tiempo=tiempo;
timer->timer=zlEvtTimerNuevoActiva(
                     socket->base
                    ,tiempo,0
                    ,zlEvtServerBasicTimer_cb,timer);
}
