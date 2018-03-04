/* Cabecera funciones timeout  isap060.c */
#include <stdio.h>
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
