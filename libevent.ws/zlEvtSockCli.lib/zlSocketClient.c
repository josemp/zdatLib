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
#include "zlSocketClient.h"
/*
struct clienteGeneric;

typedef enum {CLIENTE_GENERIC_CONNECT,CLIENTE_GENERIC_FIN,CLIENTE_GENERIC_DATOS} clienteGenericTipoEvento_e;
typedef void (*clienteGenericReadInterfaz)(clienteGenericTipoEvento_e tipo,struct evbuffer *input,struct clienteGeneric *cliente, void *tag);

typedef struct clienteGeneric
{
struct event_base *base ;
char ip[ 100 ];
int puerto ;
struct bufferevent *buffer;
int closeOnWrite;//Finalizar al terminar el envío
clienteGenericReadInterfaz read_i;
void *tag;
} clienteGeneric_t ;
*/
void clienteGenericRead_cb (struct bufferevent *bev , void * ptr )
{
    clienteGeneric_t *cliente=ptr;
  /* This callback is invoked when there is data to read on bev. */
  struct evbuffer *input = bufferevent_get_input(bev );
  cliente->read_i(CLIENTE_GENERIC_DATOS,input,cliente,cliente->tag);
}

void clienteGenericWrite_cb(struct bufferevent *bev, void *ptr)
{
	clienteGeneric_t *cliente = ptr;
	if (cliente->closeOnWrite==1)
		clienteGenericFree(cliente);
		
}
void clienteGenericEvent_cb (struct bufferevent *bev , short events , void * ptr)
{
    clienteGeneric_t *cliente = ptr;
zlEventDescBevEvent (events );

    if ( events & BEV_EVENT_CONNECTED ) {
		cliente->read_i(CLIENTE_GENERIC_CONNECT,NULL,cliente,cliente->tag);
        return; 
    } else if (events & ( BEV_EVENT_ERROR |BEV_EVENT_EOF )) {
         cliente->read_i(CLIENTE_GENERIC_FIN,NULL,cliente,cliente->tag);
         clienteGenericFree(cliente);
        
    }
}

void clienteGenericWriteCommandCierre(clienteGeneric_t *cliente,char *buf,int len,int cierraAlTerminar)
{
	if (cliente->buffer==NULL) return;//El socket ya se ha cerrado
	struct evbuffer *output = bufferevent_get_output(cliente->buffer );
	evbuffer_add(output,buf, len);
	cliente->closeOnWrite=cierraAlTerminar;

	
}
void clienteGenericWriteCommand(clienteGeneric_t *cliente,char *buf,int len)
{
	clienteGenericWriteCommandCierre(cliente,buf, len,0);
}
void clienteGenericFree(clienteGeneric_t *cliente)
{	
	bufferevent_free(cliente->buffer);
	free(cliente);
}
clienteGeneric_t *clienteGenericInicia ( char *ip , int puerto,clienteGenericReadInterfaz read_i )
{
clienteGeneric_t *cliente ;
  cliente =(clienteGeneric_t *) malloc (sizeof (clienteGeneric_t ));
  memset (cliente , 0, sizeof (clienteGeneric_t ));     
  cliente ->puerto = puerto;
  strcpy (cliente -> ip, ip );
  cliente->closeOnWrite=0;
  cliente->read_i=read_i;
  return (cliente );
}
void clienteGenericLanza (clienteGeneric_t * cliente ,struct event_base *base )
{
struct sockaddr_in sin ;
cliente ->base = base;

// bufferevent sin fd(-1)
struct bufferevent *buffer = bufferevent_socket_new ( cliente-> base , - 1, BEV_OPT_CLOSE_ON_FREE  );
cliente->buffer=buffer;
// callbacks

    bufferevent_setcb ( buffer, clienteGenericRead_cb , clienteGenericWrite_cb , clienteGenericEvent_cb, cliente );
//    evbuffer_add_printf(bufferevent_get_output(buffer), "GET %s\r\n", "!que pasa pues\n");
bufferevent_socket_connect (buffer , zlEventSockaddr (&sin ,cliente -> ip, cliente ->puerto ), sizeof (sin ));
    bufferevent_enable ( buffer, EV_READ |EV_WRITE );


}


/*
main ()
{
clienteGeneric_t *cliente = clienteGenericInicia ("127.0.0.1" , 12345);
struct event_base *base =event_base_new ();
clienteGenericLanza (cliente , base);
event_base_dispatch (base );



}

*/
