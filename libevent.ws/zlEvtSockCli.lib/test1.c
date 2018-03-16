#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <event2/event.h>
#include <event2/bufferevent.h>
#include <event2/buffer.h>
#include <zlEventMain.h>
#include "zlLog.h"
#include "zlEvtTimer.h"
//#include "zlEvtSockSer.h"
#include "zlSocketClient.h"
void eventoIniciaPrevio(int argc,char **argv)
{



}


char *worka;
long lenWorka;
void read_cb(clienteGenericTipoEvento_e tipo,struct evbuffer *input,struct clienteGeneric *cliente, void *tag)
{
int len;
char *buf ="01012014111709174100010627720     FEDJAF33\r\n"
           "1010111             90411867  143         10600000984                 \r\n"
"102000000000006700003\r\n"
"10500011000019000000\r\n"
"0199\r\n";
switch (tipo)
{
case CLIENTE_GENERIC_CONNECT:
   worka=malloc(1);
   worka[0]=0;
   lenWorka=1;
   printf("conexion <%d>\n",(int) strlen(buf));
clienteGenericWriteCommand(cliente,buf,strlen(buf));
   break;
case CLIENTE_GENERIC_FIN:
   printf("final\n");
   break;
case CLIENTE_GENERIC_DATOS:
	len=evbuffer_get_length(input);
	worka=realloc(worka,lenWorka+len);
	
	evbuffer_remove(input , worka+strlen(worka), len);
	lenWorka=lenWorka+len;
	worka[lenWorka-1]=0;

   	printf("datos<%d><%s>\n",len,worka);
   	printf("datos finales <%s>\n",worka+lenWorka-7);
        if (memcmp(worka+lenWorka-7,"0199\r\n",6)==0)
         {
	  clienteGenericFree(cliente);
         }
   	break;
default:
  break;
}



}
int eventoInicia(struct event_base *base,int argc,char **argv)
{
printf("---------------------------------------------------------------\n");	
printf("--------------------- TEST TEST -------------------------------\n");	
printf("---------------------------------------------------------------\n");	
clienteGeneric_t *cliente = clienteGenericInicia ("127.0.0.1" , 1826,read_cb);
clienteGenericLanza (cliente , base);
}

