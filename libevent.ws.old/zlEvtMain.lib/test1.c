#include <stdio.h>
#include <stdlib.h>
#include <event2/event.h>
#include <event2/bufferevent.h>
#include <event2/buffer.h>
#include "zlEventMain.h"
#include <zlLog.h>
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
	
	return(0);
}

