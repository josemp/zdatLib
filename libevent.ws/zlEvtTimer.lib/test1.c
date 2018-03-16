#include <stdio.h>
#include <stdlib.h>
#include <event2/event.h>
#include <event2/bufferevent.h>
#include <event2/buffer.h>
//#include "zlEventMain.h"
#include <assert.h>
#include "zlLog.h"
#include "zlEvtTimer.h"
#include <time.h>
struct event_base *base;
int estado;
int contador;

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
void funcionDiferida2(void *tag)
{
zlEvtTimer_t *zlTimer=tag;

printf("FuncionDiferida2 desactivando timer \n");
zlEvtTimerDesactiva(zlTimer);
pintahora();

}
void funcionDiferida3(void *tag)
{
zlEvtTimer_t *zlTimer=tag;
contador++;
estado=1;
pintahora();
    printf("contador <%d>\n",contador);
if (contador <3)
{
    printf("FuncionDiferida3 activa timer <%d>\n",contador);
	zlEvtTimerActiva(zlTimer,0,0);
	return;
	}
printf("FuncionDiferida3 desactiva timer \n");
zlEvtTimerDesactiva(zlTimer);
estado=0;
}


int main(int argc,char **argv)
{
	zlEvtTimer_t *zlTimer;
	LogInit(NULL,9);
	base = event_base_new ();
	
	printf("-------------- TEST 1 ----------------------\n");
	printf("lanzar un evento que tarde un segundo y se libere el solito\n");
	estado=0;
	pintahora();
	zlEvtTimerNuevoActivaLibera(base,1,0,funcionDiferida1,"... Test 1 ...");
	event_base_dispatch(base );
	assert(estado==1);
	
	printf("-------------- TEST 2 ----------------------\n");
	printf("lanzar un evento inmediato y se libere el solito\n");
	estado=0;
	pintahora();
	zlEvtTimerFunction(base,funcionDiferida1, "... Test 2 ...");
	event_base_dispatch(base );
	assert(estado==1);
	
		printf("-------------- TEST 3 ----------------------\n"); 
	printf("lanzar un evento en 2 segundos y desactivarla con un evento en un segudo \n");
	estado=0;
	pintahora();
	zlTimer=zlEvtTimerNuevoActiva(base,2,0,funcionDiferida1,"... Test 3 ...");
	zlEvtTimerNuevoActivaLibera(base,1,0,funcionDiferida2,zlTimer);
	event_base_dispatch(base );
	assert(estado==0);


	printf("-------------- TEST 4 ----------------------\n");
	printf("lanzar un evento y relanzarlo dos veces y luego borrarlo \n");fflush(stdout);
	estado=0;
	pintahora();
	contador=0;
	zlTimer=zlEvtTimerNuevoActiva(base,0,0,funcionDiferida3,zlEventSelfPuntero());
	//zlEvtTimerNuevoActiva(base,1,0,funcionDiferida3,1,zlTimer);
	event_base_dispatch(base );
	assert(estado==0);

	return(0);
}
