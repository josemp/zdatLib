/*********************************************************************
 *
 * Filename:        timer.c
 * Project/module:  timer functions
 * Description:     handler for timeouts
 * Author:          I.A.Tidder, 12.01.94
 *  
 *********************************************************************/
#include <stdio.h>
#define __USE_XOPEN
#include <time.h>
#include <sys/time.h>
#include <unistd.h>
#include <stdlib.h>
#include <memory.h>
#include "zlTimer.h"

/*
**
** FUNCTIONAL DESCRIPTION:
**     StartTimer -- starts a timeout
**
** PARAMETERS (input/output):
**     Temporizador_t *, output, pointer to timer structure defined
**       by caller filled out by this routine
**     long, input, timeout - length of timeout in millisecs
**     if timeout is (-1) the timer never times out
** 
** RETURN VALUE:
**     none
**
** GLOBALS:
**     none
**
** DESIGN:
** 
*/

void StartTimer(Temporizador_t *timer, long timeout)
{
  struct timezone tz;
  gettimeofday(&timer->StartTimer,&tz);
  if (timeout==-1l)
	  timer->Timeout=-1l;
  else
          timer->Timeout = timeout*1000;
}

/*
**
** FUNCTIONAL DESCRIPTION:
**     timed_out -- says whether a timer has timed out
**
** PARAMETERS (input/output):
**     Temporizador_t *, input, pointer to timer structure 
**       initialised by StartTimer()
** 
** RETURN VALUE:
**     boolean - 1 iff timer has expired, otherwise 0
**
** GLOBALS:
**     none
**
** DESIGN:
** 
*/

int TimedOut(Temporizador_t *timer)
{ 
  struct timezone tz;
  struct timeval  tv;
  long diffu,diffs,difft;
  long time_left;

  if (timer->Timeout == -1L) return(0);
  time_left=TimeLeft(timer);
  if (time_left<=0) return(1);
  return(0);
  
  gettimeofday(&tv,&tz);

  diffu=tv.tv_usec-timer->StartTimer.tv_usec;
  diffs=tv.tv_sec-timer->StartTimer.tv_sec;
  difft=diffs*1000000l+diffu;
  if (difft<timer->Timeout)
     return(0);
  else
     return(1); 

} 

/*
**
** FUNCTIONAL DESCRIPTION:
**     time_left -- evaluate time left on a timer
**
** PARAMETERS (input/output):
**     Temporizador_t *, input, pointer to timer structure 
**       initialised by StartTimer()
** 
** RETURN VALUE:
**     long - time left in millisecs or (-1L) if indefinite
**
** GLOBALS:
**     none
**
** DESIGN:
** 
*/

long TimeLeft(Temporizador_t *timer)
{ 
  long  time_left;
  struct timezone tz;
  struct timeval  tv;
  long diffu,diffs,difft;

  if (timer->Timeout == -1L) return(-1L);
  gettimeofday(&tv,&tz);

  diffu=tv.tv_usec-timer->StartTimer.tv_usec;
  diffs=tv.tv_sec-timer->StartTimer.tv_sec;
  difft=diffs*1000000l+diffu;
  time_left=(long)(timer->Timeout-difft); 
  if (time_left<0L)    return(0L);
  return((long)(time_left/1000)); 
}


/* Obtiene la fecha y la hora actuales del sistema */
void timestamp(char *fecha,char *hora)
{
 struct tm *tiempos;
 time_t x;
 long y;

 x=time(&y);
 tiempos=localtime(&x);
 sprintf(hora,"%02d%02d%02d",  tiempos->tm_hour,
                               tiempos->tm_min,
                               tiempos->tm_sec);
 sprintf(fecha,"%04d%02d%02d", (tiempos->tm_year)+1900,
                               tiempos->tm_mon+1,
                               tiempos->tm_mday);
}

/* ------------------------------------------------------------------ */
/* --- PROYMAN           - Departamento Analisis y Programacion.  --- */
/* ---                   Sistema Operativo SCO                    --- */
/* --- ---------------------------------------------------------- --- */
/* --- timer.c                                                    --- */
/* --- ---------------------------------------------------------- --- */
/* ---       Libreria   Libreria Standard 01 libpr01.             --- */
/* ---       Fichero    Modulo Fuente con la Funcion para Obtener --- */
/* ---                  la Fecha y la Hora del Sistema.           --- */
/* ---                                                            --- */
/* --- ---------------------------------------------------------- --- */
/* --- Devuelve los siguientes valores:                           --- */
/* ---                                                            --- */
/* ---                  timer(0)    --> Segundos                  --- */
/* ---                  timer(1)    --> Minutos                   --- */
/* ---                  timer(2)    --> Hora                      --- */
/* ---                  timer(3)    --> Dia                       --- */
/* ---                  timer(4)    --> Mes                       --- */
/* ---                  timer(5)    --> A\o                       --- */
/* ---                  timer(6)    --> Dia semana                --- */
/* ---                  timer(7)    --> Dia a\o                   --- */
/* ---                                                            --- */
/* ---                                                            --- */
/* ------------------------------------------------------------------ */

#include <time.h>

int zlTimerDat(x)

int x;
{
   long      t;
   long      *pt;
   struct tm *st;

   t=time((long) 0);
   st=localtime(&t);

   switch (x)
      {
      case 0:
         return(st->tm_sec);                              /* segundos */
      case 1:
         return(st->tm_min);                               /* minutos */
      case 2:
         return(st->tm_hour);                                 /* hora */
      case 3:
         return(st->tm_mday);                                  /* dia */
      case 4:
         return(++st->tm_mon);                                 /* mes */
      case 5:
         return((st->tm_year)%100);                            /* a\o */
      case 6:
         return(st->tm_wday);                           /* dia semana */
      case 7:
         return(++st->tm_yday);                            /* dia a\o */
      default:
         return(-1);
      };
}
void zlTimerRestaDias(int numDias,int *dia,int *mes,int *ato)
{
time_t t;
struct tm *st;
t=time((time_t *) 0);
t=t-numDias*24l*60l*60l;
st=localtime(&t);
*dia=st->tm_mday;
*mes=st->tm_mon+1;
*ato=st->tm_year+1900;
}
void zlTimerRestaMes(int *dia,int *mes,int *ato)
{
time_t t;
struct tm *st;
t=time((time_t *) 0);
st=localtime(&t);
*dia=st->tm_mday;
st->tm_mday=15;
t=mktime(st);
t=t-30*24l*60l*60l;
st=localtime(&t);
*mes=st->tm_mon+1;
*ato=st->tm_year+1900;
}
long zlTimerCalculaSegundos(char *hora)
{
 int ho,mi,se;
 char aux[10];
sprintf(aux,"%.2s",hora);
ho=atoi(aux);
sprintf(aux,"%.2s",hora+2);
mi=atoi(aux);
sprintf(aux,"%.2s",hora+4);
se=atoi(aux);
return(ho*60*60+mi*60+se);
}

struct tm  *tmFecha(char *fecha,char *formatFecha, char *hora,char *formatHora,struct tm *ltm)
{
char fechaHora[200]; // supongo bastante
char format[200];
sprintf(fechaHora,"%s %s",fecha,hora);
sprintf(format,"%s %s",formatFecha,formatHora);
// Obligatorio inicializarla
memset(ltm, 0, sizeof(struct tm));
strptime(fechaHora, format, ltm);
return(ltm);
}
time_t timeFecha(char *fecha,char *formatFecha, char *hora,char *formatHora)
{
struct tm ltm;
tmFecha(fecha,formatFecha,hora,formatHora,&ltm);
return(mktime(&ltm));
}
