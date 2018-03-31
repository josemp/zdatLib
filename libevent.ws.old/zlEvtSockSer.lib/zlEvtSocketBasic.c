#include <event2/event.h>
#include <event2/buffer.h>
#include <event2/bufferevent.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <errno.h>
#include <fcntl.h>

#include <string.h>
#include <zlEventMain.h>
#include <zlEvtTimer.h>
#include "zlEvtSocketBasic.h"



void zlEvtServerBasicSocketTimerRespuesta_cb(void *cls)
{
    zlEvtServerBasicSocket_t *cliente=cls;
    zlEvtTimerFree(cliente->timerRespuesta);
    cliente->timerRespuesta=NULL;
    cliente->read_i(CLIENTE_GENERIC1_TIMEOUT_R,NULL,cliente,cliente->tag);

}



void zlEvtServerBasicSocketRead_cb (struct bufferevent *bev , void * ptr )
{
zlEvtServerBasicSocket_t *socket=ptr;
  /* This callback is invoked when there is data to read on bev. */
  struct evbuffer *input = bufferevent_get_input(bev );
  cliente->read_i(CLIENTE_GENERIC1_DATOS,input,socket,socket->tag);
}

void zlEvtServerBasicSocketWrite_cb(struct bufferevent *bev, void *ptr)
{
        zlEvtServerBasicSocket_t *socket=ptr;
        if (socket->closeOnWrite==1)
                zlEvtServerBasicSocketFree(socket);

}

void zlEvtServerBasicSocketEvent_cb (struct bufferevent *bev , short events , void * ptr)
{
   zlEvtServerBasicSocket_t *socket=ptr;
zlEventDescBevEvent (events );

     if (events & ( BEV_EVENT_ERROR |BEV_EVENT_EOF )) {
         if (socket->timerRespuesta!=NULL)
            zlEvtTimerFree(socket->timerRespuesta);
         cliente->read_i(CLIENTE_GENERIC1_FIN,NULL,cliente,cliente->tag);
         zlEvtServerBasicSocketFree(cliente);

    }
}

commandConnect(zlEvtServerBasicSocket_t *socket)
{
 struct bufferevent *bev = bufferevent_socket_new(
      socket->base , socket->fd, BEV_OPT_CLOSE_ON_FREE);
      bufferevent_setcb(
                 bev 
               , zlEvtServerBasicSocketRead_cb
               , zlEvtServerBasicSocketWrite_cb
               , zlEvtServerBasicSocketEvent_cb
               , socket);
      bufferevent_enable(bev , EV_READ);


}

