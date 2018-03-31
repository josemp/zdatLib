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
#include "zlEvtSockSer.h"
void bindSocket_cb(int estado,zlServerSocket_t *server)
{
  printf("bind\n");
}

void connectSocket_cb(struct zlConexionItem *conexionItem)
{
  printf("connect\n");
}

void writeSocket_cb(struct zlConexionItem *conexionItem)
{
}
void readSocket_cb(zlServerTipoEvento_e tipo,
                struct evbuffer *input,
                struct zlConexionItem *conexionItem)
{
        char aux[300];
        memset(aux,0,300);
        printf("LLegan datos al servidor\n");fflush(stdout);
//        if (tipo==ZL_CI_CIERRE){printf("cierre\n");fflush(stdout); return;}
        int lenMsg= (int) evbuffer_get_length(input);
        evbuffer_remove(input, aux,(size_t) lenMsg );// Leemos todo el buffer
        printf("llega <%.*s> \n",lenMsg,aux);fflush(stdout);
        conexionItem->progTag=NULL;
//    bufferevent_enable(conexionItem->bev , EV_WRITE);
//        bufferevent_disable(conexionItem->bev , EV_READ);

}
void closeSocket_cb(struct zlConexionItem *conexionItem)
                                {
                                }

void eventoIniciaPrevio(int argc,char **argv)
{



}


int eventoInicia(struct event_base *base,int argc,char **argv)
{
printf("---------------------------------------------------------------\n");
printf("--------------------- TEST TEST -------------------------------\n");
printf("---------------------------------------------------------------\n");
        LogInit(NULL,9);
        LogW(9,"Esto es una prueba nivel 9\n");
//testCierre(base);
//testCliente(base);
//testClienteCloseServer(base);
//testClienteLargo1(base);
zlServerSocket_t *server = zlServerSocketInicia(
           5000 // puerto
           ,1   // timeout seg accept 
           ,1   // timeout seg accept 
           ,0   // registrar si o no
           ,"msi" //prefijo log
           ,9
           ,bindSocket_cb
           ,connectSocket_cb
           ,readSocket_cb // interfaz lectura
           ,writeSocket_cb // interfaz write
           ,closeSocket_cb
           ,NULL );       //tag
if (server==NULL) printf("algo va mal\n");
zlServerSocketLanza(server, base);
return(0);
}







