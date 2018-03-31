#include <stdio.h>
#include <stdlib.h>
#include <event2/event.h>
#include <event2/bufferevent.h>
#include <event2/buffer.h>
#include "zlEventMain.h"
#include "zlLog.h"
#include "zlEvtTimer.h"
#include "zlEvtSockSer.h"

void funcionDiferida(void *tag)
{
zlServerSocket_t * server=(zlServerSocket_t *) tag;
printf(" ---- CIERRE CONTROLADO ------ testCierre\n");
zlServerSocketFree(server);
}

// Con esta función iniciamos un socket server, pero lo cerramos en 3 segundos
void testCierre(struct event_base *base)
{
	zlServerSocket_t * server = zlServerSocketInicia(12345, // puerto
                                              1,// timeout seg accept
											  1,// timeout seg close conexion
											  0,// registrar si o no
											  "msi",
											  9,
											  NULL, // bind
											  NULL, // interfaz conexion
											  NULL, // interfaz read
											  NULL, // interfaz write
											  NULL, // interfaz close
											  NULL );	//tag
zlServerSocketLanza(server, base);	
//zlDefered_t * z=
//zlEventDeferedFunctionTime(3,0,funcionDiferida, server);
zlEvtTimerNuevoActivaLibera(base,3,0,funcionDiferida,server);
	
}
