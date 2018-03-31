#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <memory.h>
#include <event2/event.h>
#include <event2/bufferevent.h>
//#include "zlEventMain.h"
#include <assert.h>
#include "zlEvtTimer.h"
#include "zlLog.h"

/*
  Estados
  -------  
  None                           : no existe, no hay recursos
  Evento noPendiente/Desactivado : recursos creados, y asignado a event_base : zlEvtTimerTimerNuevo(...)
                                   no hace nada
  Evento pendiente/Activo        : esperando el momento de lanzarse          : zlEvtTimerTimerActiva(...) ha de estar inicializado
  Evento lanzado                 : llegó la hora y se activo el callback     : zlEvtTimer_i(void *tag)  por defecto pasa a pendiente
  
  Comandos
  --------
  Inicializar evento  : Crea los recursos y asigna a event_base   : zlEvtTimerTimerNuevo(...)    
                        Estado desactivado
  Activar evento      : El evento pasa a activo                   : zlEvtTimerTimerActiva(...) ha de estar inicializado
                                                                    zlEvtTimerTimerNuevoActiva(...)
  Desactivar evento   : El evento Activo pasa a inactivo          : zlEvtTimerTimerDesactiva(...)
  Liberar evento      : El evento se desactiva y desinicializa
                           liberando todos los recursos           : zlEvtTimerFree(...)
  Evento
  ------
  Lanzar evento       : El loop lanza el callback      
  
  Flujo
  ---------
  Comando/evento     Estado Inicial      Estado Final
  --------------     -------------       --------------
  Inicializar        none                Desactivado
  Activar            Desactivado         activo
  Activar            Activo              Activo
  Activar            Lanzado             Activo
  Lanzar(Evento)     Lanzado             Desactivado (por defecto)
  Desactivar         Desactivado         Desactivado
  Desactivar         Activo              Desactivado
  Desactivar         Lanzado             Desactivado
  Liberar            Desactivado         None
  Liberar            Activo              None
  Liberar            Lanzado             None
  
 */

// Este puntero pasado como parámetro al timer, toma el propio evento como argumento
// no hay otra manera, copiada de la funcion event_self_cbarg() de libevent de futuras versiones
// pero que no existe en la versión que estoy utilizando.

static int zlEventSelfPunteroi;
void *zlEventSelfPuntero()
{
	return((void *)&zlEventSelfPunteroi);
}

 /* Callback interno para manejar el evento */
static void zlEvtTimerInterno_cb(evutil_socket_t fd, short what, void *arg)
{
	LogW(9,"Entrando en callbackInterno normal\n");
  zlEvtTimer_t *defered = arg;
  defered->function(defered->tag);
  
//  event_base_loopexit(base, NULL);
}

/* Este callback libera el zlEvtTimer */
/* jamás se debe de usar si se va a usar el comando desactivar */
/* porque no tenemos forma de saber si se ha lanzado ya */
/* por lo tanto es util solo si es un evento seguro de lanzarse */

/* Callback interno para manejar el evento liberando todo*/


static void zlEvtTimerInternoFree_cb(evutil_socket_t fd, short what, void *arg)
{
	LogW(9,"Entrando en callbackInterno Liberador\n");
	
  zlEvtTimer_t *defered = arg;
  defered->function(defered->tag);
  zlEvtTimerFree(defered);
//  event_base_loopexit(base, NULL);
}

void zlEvtTimerFree(zlEvtTimer_t *defered)
{
	event_free(defered->evtimeout);
    free(defered);
	
}

// Creo que hay que separar los nombres de los que liberan y no liberan en vez del parámetro libera
// Por defecto que no libere
/* si como argumento pasamos el zlEventSelfPuntero(), el argumento leido en el callback es el zlEvtTimer */
zlEvtTimer_t *zlEvtTimerNuevo(struct event_base *base,zlEvtTimer_i function,void *arg)
{
	int libera=0;
	zlEvtTimer_t *defered=malloc(sizeof(zlEvtTimer_t));
	assert(defered!=NULL);
	if ((int *)arg== (int *) zlEventSelfPuntero())
		arg=defered;
	defered->function=function;
	defered->tag=arg;
	if (libera==0)
	  defered->evtimeout = evtimer_new(base, zlEvtTimerInterno_cb, defered);	
    else
 	  defered->evtimeout = evtimer_new(base, zlEvtTimerInternoFree_cb, defered);	
	return(defered);
}
// Creo que hay que separar los nombres de los que liberan y no liberan en vez del parámetro libera
// Por defecto que no libere
/* si como argumento pasamos el zlEventSelfPuntero(), el argumento leido en el callback es el zlEvtTimer */
zlEvtTimer_t *zlEvtTimerNuevoLibera(struct event_base *base,zlEvtTimer_i function,void *arg)
{
	int libera=1;
	zlEvtTimer_t *defered=malloc(sizeof(zlEvtTimer_t));
	assert(defered!=NULL);
	if ((int *)arg== (int *) zlEventSelfPuntero())
		arg=defered;
	defered->function=function;
	defered->tag=arg;
	if (libera==0)
	  defered->evtimeout = evtimer_new(base, zlEvtTimerInterno_cb, defered);	
    else
 	  defered->evtimeout = evtimer_new(base, zlEvtTimerInternoFree_cb, defered);	
	return(defered);
}
// No es persistente el timer
void zlEvtTimerActiva(zlEvtTimer_t *defered,int sec,int usec)
{
	struct timeval tv;
	tv.tv_sec=sec;
	tv.tv_usec=usec;
	LogW(9,"Activando \n");fflush(stdout);
	assert(evtimer_add(defered->evtimeout, &tv)==0);	
	LogW(9,"Activado \n");fflush(stdout);
}
zlEvtTimer_t *zlEvtTimerNuevoActiva(struct event_base *base,int sec,int usec,zlEvtTimer_i function,void *arg)
{
	zlEvtTimer_t *defered=zlEvtTimerNuevo(base,function,arg);
	zlEvtTimerActiva(defered,sec,usec);
	return(defered);
}
zlEvtTimer_t *zlEvtTimerNuevoActivaLibera(struct event_base *base,int sec,int usec,zlEvtTimer_i function,void *arg)
{
	zlEvtTimer_t *defered=zlEvtTimerNuevoLibera(base,function,arg);
	zlEvtTimerActiva(defered,sec,usec);
	return(defered);
}
void zlEvtTimerDesactiva(zlEvtTimer_t *defered)
{
	assert(evtimer_del(defered->evtimeout)==0);
}

/* Función rapida que libera y con 0 de timer */
zlEvtTimer_t * zlEvtTimerFunction(struct event_base *base,zlEvtTimer_i funcion, void *arg)
{
	return(zlEvtTimerNuevoActivaLibera(base,0,0,funcion,arg));
}

// Convierte (double) segundos en sec y usec
void segundosToSecAndUsec(double segundos,int *sec,long *usec)
{
//int parteEntera=(int) segundos;
double parteDecimal=segundos - (int) segundos;
double usecd = parteDecimal*1000000;
*sec=(int) segundos;
*usec= (long) usecd;
}

