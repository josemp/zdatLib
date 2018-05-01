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
#include "zlEvtServer.h"
/*Los tipos del callback socket
typedef enum {
              ZLEVT_SOCKET_CONNECT
             ,ZLEVT_SOCKET_DATOS
             ,ZLEVT_SOCKET_FIN
             ,ZLEVT_SOCKET_TIMEOUT_R
              }  zlEvtSocketTipoEvento_e;
*/
//El tipo de callback
void socket_cb(
              zlEvtSocketTipoEvento_e tipo
             ,struct evbuffer *input
             , int numTimer
             ,struct zlEvtSocket_s *socket
             , void *tagProtocolo
             , void *tag)
{
    switch(tipo)
     {
              case ZLEVT_SOCKET_CONNECT:
                   printf("ZLEVT_SOCKET_CONNECT  \n");
                   char pepe[10];
                   strcpy(pepe,"hola\n");
zlEvtSocketWriteCommand(socket,pepe,strlen(pepe));
                   printf("pasa pues\n");fflush(stdout);
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
                   printf(" ZLEVT_SOCKET_TIMEOUT_R \n");
                   break;
              default:
                   printf("ZLEVT_SOCKET DEFAULT \n");
                   break;
     }

}

zlBool_t server_cb(
                   zlEvtServerTipoEvento_e tipo
                   , struct zlEvtServer_s *server
                   , zlBool_t estado
                   ,zlEvtServerSocketParam_t *socketParam // for connect
                   ,void *tag)
{
  printf("pasapues\n");
        switch(tipo){
         case SERVER_BASIC_BIND:
             printf("llega bind bind\n");

             break;
        case SERVER_BASIC_CONNECT:
             printf("llega connect\n");fflush(stdout);
/*
              zlEvtSocket_t  socket= *zlEvtServerSocketConnect(
                   server
                  ,socketParam
                  , NULL
                  );
*/
             return(zlTrue);
             break;
        case SERVER_BASIC_DISCONNECT:
             printf("llega disconnect\n");fflush(stdout);
             break;
        default:
             printf("llega ????\n");
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

zlEvtServer_t *server = zlEvtServerInicia (
          base 
         ,5000
         ,0
         ,server_cb
         ,socket_cb
         ,NULL );
if (server==NULL) printf("algo va mal\n");
return(0);
}


