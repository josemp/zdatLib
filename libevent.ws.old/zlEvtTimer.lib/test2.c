#include <stdio.h>
#include <stdlib.h>
#include <event2/event.h>
#include <event2/bufferevent.h>
#include <event2/buffer.h>
#include "zlEventMain.h"
#include "zlLog.h"
#include "zlEvtTimer.h"
#include <time.h>
struct event_base *baseEvt;
int estado=0;
void pintahora() {
    time_t epoch_time;
    struct tm *tm_p;
    epoch_time = time( NULL );
    tm_p = localtime( &epoch_time );
    printf("The time is %.2d:%.2d:%.2d\n", 
        tm_p->tm_hour, tm_p->tm_min, tm_p->tm_sec );
    return ;
    }

void funcionDiferida1(void *tag)
{
char *nombre=(char*) tag;
printf("FuncionDiferida1 realizada <%s>\n",nombre);
pintahora();
estado=1;
}
void eventoIniciaPrevio(int argc,char **argv)
{
	printf("-------------- TEST ----------------------\n");
	LogInit("file.log",5);
	LogW(9,"Esto es una prueba nivel 9\n");
	LogW(5,"Esto es una prueba nivel 5\n");
    LogW(1,"Esto es una prueba nivel 5\n");
}
int eventoInicia(struct event_base *base,int argc,char **argv)
{
	 zlEvtTimerNuevoActivaLibera(base,1,0,funcionDiferida1,"... Test 1 ...");
	return(0);
}

