#include <stdio.h>
#include <stdlib.h>
#include <event2/event.h>
#include <event2/bufferevent.h>
#include <event2/buffer.h>
#include "zlEventMain.h"
#include "zlLog.h"
#include "zlEvtTimer.h"
#include "zlEvtSockSer.h"


void testCliente(struct event_base *base);
// Con esta función iniciamos un socket server, pero lo cerramos en 3 segundos

/*
static void write_to_file_cb(int severity, const char *msg)
{
    const char *s;
    switch (severity) {
        case _EVENT_LOG_DEBUG: s = "debug"; break;
        case _EVENT_LOG_MSG:   s = "msg";   break;
        case _EVENT_LOG_WARN:  s = "warn";  break;
        case _EVENT_LOG_ERR:   s = "error"; break;
        default:               s = "?";     break; // never reached 
    }
    printf("[%s] %s\n", s, msg);
}
*/
void escribeDiferido(void *tag)
{
	printf("diferido \n");
	//struct bufferevent *bev=tag;
	zlConexionItem_t *conexionItem=tag;
	zlServerSocketWriteCommand(0,conexionItem,"hola\n",5,NULL);
	
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
testVariosClientes(base);
return(0);
}

