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
#include "zlEvtTimer.h"
#include "zlEvtServerBasic.h"
#include "zlEvtServerBasicSocket.h"
/*Los tipos del callback socket
typedef enum {
              SERVER_BASIC_SOCKET_CONNECT
             ,SERVER_BASIC_SOCKET_DATOS
             ,SERVER_BASIC_SOCKET_FIN
             ,SERVER_BASIC_SOCKET_TIMEOUT_R
              }  zlServerBasicSocketTipoEvento_e;
*/
//El tipo de callback
void socket_cb(
              zlServerBasicSocketTipoEvento_e tipo
             ,struct evbuffer *input
             , int numTimer
             ,struct zlEvtServerBasicSocket_s *socket
             , void *tag)
{
    switch(tipo)
     {
              case SERVER_BASIC_SOCKET_CONNECT:
                   printf("SERVER_BASIC_SOCKET_CONNECT  \n");
                   break;
              case SERVER_BASIC_SOCKET_DATOS:
                   printf(" SERVER_BASIC_SOCKET_DATOS  \n");
                   break;
              case SERVER_BASIC_SOCKET_WRITE:
                   printf("SERVER_BASIC_SOCKET_WRITE  \n");
                   break;
              case SERVER_BASIC_SOCKET_FIN:
                   printf(" SERVER_BASIC_SOCKET_FIN  \n");
                   break;
              case SERVER_BASIC_SOCKET_TIMEOUT:
                   printf(" SERVER_BASIC_SOCKET_TIMEOUT_R \n");
                   break;
              default:
                   printf("SERVER_BASIC_SOCKET DEFAULT \n");
                   break;
     }

}

void serverBasic_cb(
                   serverBasicTipoEvento_e tipo
                   , struct zlServerBasic_s *serverBasic
                   , zlBool_t estado
                   ,zlServerBasicSocketParam_t *socketParam // for connect
                   ,void *tag)
{
  printf("pasapues\n");
        switch(tipo){
         case SERVER_BASIC_BIND:
             printf("llega bind bind\n");

             break;
        case SERVER_BASIC_CONNECT:
             printf("llega connect\n");fflush(stdout);
             zlEvtServerBasicSocket_t  *socket = zlEvtServerBasicSocketConnect(
                   socketParam
                  ,socket_cb
                  ,serverBasic
                  , NULL
                  );

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

zlServerBasic_t *server = zlServerBasicInicia (
          base 
         ,5000
         ,0
         ,serverBasic_cb
         ,NULL );
if (server==NULL) printf("algo va mal\n");
return(0);
}


