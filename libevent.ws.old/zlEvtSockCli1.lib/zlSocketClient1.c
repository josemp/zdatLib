/* basado en stdbuffer.c */
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
#include "zlSocketClient1.h"
/*
struct clienteGeneric1;

typedef enum {CLIENTE_GENERIC1_CONNECT,CLIENTE_GENERIC1_FIN,CLIENTE_GENERIC1_DATOS} clienteGeneric1TipoEvento_e;
typedef void (*clienteGeneric1ReadInterfaz)(clienteGeneric1TipoEvento_e tipo,struct evbuffer *input,struct clienteGeneric1 *cliente, void *tag);

typedef struct clienteGeneric1
{
struct event_base *base ;
char ip[ 100 ];
int puerto ;
struct bufferevent *buffer;
int closeOnWrite;//Finalizar al terminar el envío
clienteGeneric1ReadInterfaz read_i;
void *tag;
} clienteGeneric1_t ;
*/
void clienteGeneric1TimerRespuesta_cb(void *cls)
{
    clienteGeneric1_t *cliente=cls;
    zlEvtTimerFree(cliente->timerRespuesta);
    cliente->timerRespuesta=NULL;
    cliente->read_i(CLIENTE_GENERIC1_TIMEOUT_R,NULL,cliente,cliente->tag);

}
void clienteGeneric1Read_cb (struct bufferevent *bev , void * ptr )
{
    clienteGeneric1_t *cliente=ptr;
  /* This callback is invoked when there is data to read on bev. */
  struct evbuffer *input = bufferevent_get_input(bev );
  cliente->read_i(CLIENTE_GENERIC1_DATOS,input,cliente,cliente->tag);
}

void clienteGeneric1Write_cb(struct bufferevent *bev, void *ptr)
{
        printf("write-------- callback ----->\n");
	clienteGeneric1_t *cliente = ptr;
	if (cliente->closeOnWrite==1)
		clienteGeneric1Free(cliente);
		
}
void clienteGeneric1Event_cb (struct bufferevent *bev , short events , void * ptr)
{
    clienteGeneric1_t *cliente = ptr;
zlEventDescBevEvent (events );

    if ( events & BEV_EVENT_CONNECTED ) {
		cliente->read_i(CLIENTE_GENERIC1_CONNECT,NULL,cliente,cliente->tag);
        if (cliente->tiempoRespuesta >0)
        {
          cliente->timerRespuesta=zlEvtTimerNuevoActiva(
                     cliente->base
                    ,cliente->tiempoRespuesta,0
                    ,clienteGeneric1TimerRespuesta_cb,cliente);
        }
        return; 
    } else if (events & ( BEV_EVENT_ERROR |BEV_EVENT_EOF )) {
         if (cliente->timerRespuesta!=NULL)
            zlEvtTimerFree(cliente->timerRespuesta); 
         cliente->read_i(CLIENTE_GENERIC1_FIN,NULL,cliente,cliente->tag);
         clienteGeneric1Free(cliente);
        
    }
}

void clienteGeneric1WriteCommandCierre(clienteGeneric1_t *cliente,char *buf,int len,int cierraAlTerminar)
{
	if (cliente->buffer==NULL) return;//El socket ya se ha cerrado
	struct evbuffer *output = bufferevent_get_output(cliente->buffer );
	evbuffer_add(output,buf, len);
	cliente->closeOnWrite=cierraAlTerminar;

	
}
void clienteGeneric1WriteCommand(clienteGeneric1_t *cliente,char *buf,int len)
{
        printf("-----------write command\n");
	clienteGeneric1WriteCommandCierre(cliente,buf, len,0);
}
void clienteGeneric1Free(clienteGeneric1_t *cliente)
{	
	bufferevent_free(cliente->buffer);
	free(cliente);
}
/* Tiempo respuesta en segundos. 
   tiempoRespuesta=-1 -> tiempo infinito
*/
clienteGeneric1_t *clienteGeneric1Lanza ( struct event_base *base, char *ip , int puerto,clienteGeneric1ReadInterfaz read_i,int tiempoRespuesta ,void *cls)
{
  clienteGeneric1_t *cliente ;
  struct sockaddr_in sin ;
  cliente =(clienteGeneric1_t *) malloc (sizeof (clienteGeneric1_t ));
  memset (cliente , 0, sizeof (clienteGeneric1_t ));     
  cliente ->puerto = puerto;
  strcpy (cliente -> ip, ip );
  cliente->closeOnWrite=0;
  cliente->read_i=read_i;
  cliente ->base = base;
  cliente->tag=cls;
  cliente->tiempoRespuesta=tiempoRespuesta;

// bufferevent sin fd(-1)
struct bufferevent *buffer = bufferevent_socket_new ( cliente-> base , - 1, BEV_OPT_CLOSE_ON_FREE  );
cliente->buffer=buffer;
// callbacks

    bufferevent_setcb ( buffer, clienteGeneric1Read_cb , clienteGeneric1Write_cb , clienteGeneric1Event_cb, cliente );
//    evbuffer_add_printf(bufferevent_get_output(buffer), "GET %s\r\n", "!que pasa pues\n");
bufferevent_socket_connect (buffer , zlEventSockaddr (&sin ,cliente -> ip, cliente ->puerto ), sizeof (sin ));
    bufferevent_enable ( buffer, EV_READ |EV_WRITE );


  return (cliente );
}


/*
main ()
{
clienteGeneric1_t *cliente = clienteGeneric1Inicia ("127.0.0.1" , 12345);
struct event_base *base =event_base_new ();
clienteGeneric1Lanza (cliente , base);
event_base_dispatch (base );



}

*/
