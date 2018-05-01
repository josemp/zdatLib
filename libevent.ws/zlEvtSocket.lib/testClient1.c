#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <memory.h>
#include <dirent.h>
#include <event2/event.h>
#include <event2/bufferevent.h>
#include <event2/buffer.h>
#include <signal.h>
#include <unistd.h>
#include <assert.h>
#include <pthread.h>
#include "zlEventMain.h"
#include "zlStd.h"
#include "zlListas.h"
#include "zlEvtTimer.h"
#include "zlEvtSocketComun.h"
#include "zlEvtClient.h"
#include "zlEvtServer.h"

zlBool_t client_cb(zlEvtClientTipoEvento_e tipo
                , struct zlEvtClient_s *cliente
                ,void *tag)
{
//	zlEvtClient_t *cliente = (zlEvtClient_t *) tag;
	switch (tipo)
	{
		case CLIENTE_BASIC_DISCONNECT:
		printf("Cliente desconectado\n");
		break;
		default:
		break;
	}
}
void socket_cb(
              zlEvtSocketTipoEvento_e tipo
             ,struct evbuffer *input
             , int numTimer
            // ,struct zlEvtSocket_s *socket
             ,zlEvtSocket_t *socket
             , void *tagProtocolo
             , void *tag)
{
  printf("pasa pues\n");fflush(stdout);
   switch(tipo)
     {
              case ZLEVT_SOCKET_RECHAZADO:
                   printf("ZLEVT_SOCKET_RECHAZADO  \n");
                   break;
              case ZLEVT_SOCKET_CONNECT:
                   printf("ZLEVT_SOCKET_CONNECT  \n");
                   break;
              case ZLEVT_SOCKET_DATOS:
                   printf(" ZLEVT_SOCKET_DATOS  \n");
                   break;
              case ZLEVT_SOCKET_WRITE:
                   printf("ZLEVT_SOCKET_WRITE  \n");
                   break;
              case ZLEVT_SOCKET_FIN:
                   printf(" ZLEVT_SOCKET_FIN  \n");
                   break;
              case ZLEVT_SOCKET_TIMEOUT:
                   printf(" ZLEVT_SOCKET_TIMEOUT \n");
                   break;
              default:
                   printf("ZLEVT_SOCKET DEFAULT \n");
                   break;
     }

}

void eventoIniciaPrevio(int argc,char **argv)
{
}

int eventoInicia(struct event_base *base,int argc,char **argv)

{
printf("---------------------------------------------------------------\n");
printf("--------------------- TEST TEST -------------------------------\n");
printf("---------------------------------------------------------------\n");
        LogInit("test.log",9);
        LogW(9,"Esto es una prueba nivel 9\n");
//testCierre(base);
//testCliente(base);
//testClienteCloseServer(base);
//testClienteLargo1(base);
printf("Conectando ....................\n");
zlEvtClient_t *clienteSocket=zlEvtClientInicia (
          base
         , "127.0.0.1"
         , 3010
         , client_cb
         , socket_cb
         , NULL );
zlEvtClientCommandConnect(clienteSocket);
printf("nos vamos\n");
return(0);
}


