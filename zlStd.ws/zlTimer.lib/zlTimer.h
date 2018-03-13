/* Cabecera funciones timeout  isap060.c */
#include <stdio.h>
#define __USE_XOPEN
#include <time.h>
#include <sys/time.h>
#include <unistd.h>
#include <stdlib.h>

typedef struct 
{
long Timeout;
struct timeval StartTimer;
} Temporizador_t;

void StartTimer(Temporizador_t *timer, long timeout);
int TimedOut(Temporizador_t *timer);
long TimeLeft(Temporizador_t *timer);
long zlTimerCalculaSegundos(char *hora);

/**
  @brief retorna una estructura de tiempo, a partir de cadenas de fecha y hora

  @param fecha (i)      : Cadena con la fecha. ej:  "20180203"
  @param formatFecha (i):  el formato.   ej: "%Y%m%d"
  @param hora (i)       :  Cadena con la fecha. ej:  "20180203"
  @param formatHora (i) :  el formato.   ej: "%Y%m%d"
  @param ltm (io)       :  La estructura que retorna (tiene que estar inicializada)
  @returns   :  Puntero a una estructura de tiempo  (struct tm). Estructura que contiene una fecha y hora del calendario desglosadas en sus componentes. 

  Example:
  \verbatim
    struct tm ltm;

    tmFecha("20180203","%Y%m%d","12:05:08","%H:%M:%S",&ltm);

  \endverbatim

  */

struct tm  *tmFecha(char *fecha,char *formatFecha, char *hora,char *formatHora,struct tm *ltm);

/**
  @brief Calcula los segundos de una Fecha/Hora desde 1900 (time_t)

  @param fecha (i): Cadena con la fecha. ej:  "20180203"
  @param formatFecha (i):  el formato.   ej: "%Y%m%d"
  @param hora (i):  Cadena con la fecha. ej:  "20180203"
  @param formatHora (i):  el formato.   ej: "%Y%m%d"
  @param ltm (io):  La estructura que retorna (tiene que estar inicializada)
  @returns       :  (tiempo en segundos calculados desde 1900)

  Example:
  \verbatim
     char buf[22];
     strcpy(buf," quita espacios ");
     Conf1LimpiaBuffer(buf,7);
     printf("buf despues [%s]\n",buf);
   \endverbatim

  Resultado:
  \Verbatin
   buf despues [quitaespacios ]
  \endverbatim

  */

time_t timeFecha(char *fecha,char *formatFecha, char *hora,char *formatHora);



