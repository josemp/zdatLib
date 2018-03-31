# LIBRERIA SOCKETS BASADA EN LIBEVENT

Contenido

## Main con LIBEVENT

## Timers con LIBEVENT


## Cliente Socket

### Plantilla Cliente

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <memory.h>
#include <zlStd.h>
#include <zlEvt.h>

// Callback

void socket_cb(
              zlEvtSocketTipoEvento_e tipo // tipo de evento (ver el switch de abajo)
             ,struct evbuffer *input       // Solo válido para ZLEVT_SOCKET_DATOS
             , int numTimer                // Solo válido para ZLEVT_SOCKET_TIMEOUT
             ,zlEvtSocket_t *socket        // La estructura interna socket
             , void *tag                   // Una closure de usuario
             )
{
    switch(tipo)
     {
              case ZLEVT_SOCKET_RECHAZADO: // Solamente en cliente. No hay que hacer nada aqui respecto al socket
                   
                   break;
              case ZLEVT_SOCKET_CONNECT: // Valido en cliente y servidor
                   
                   break;
              case ZLEVT_SOCKET_DATOS: // Se reciben datos
                   
                   break;
              case ZLEVT_SOCKET_WRITE: // Se ha terminado de escribir
                   
                   break;
              case ZLEVT_SOCKET_FIN: // Se produce cuando el remoto cierra la conexion
                   
                   break;
              case ZLEVT_SOCKET_TIMEOUT: // Se produce cuando llega un timeout de los que hemos puesto.
                   
                   break;
              default: // Esta no se tiene que producir nunca
                   
                   break;
     }

}

//Main libevent

//Main libevent

void eventoIniciaPrevio(int argc,char **argv)
{
}


//Main libevent
int eventoInicia(struct event_base *base,int argc,char **argv)

{
 LogInit("test.log",9);
 LogW(9,"Esto es una prueba nivel 9\n");


// Conectamos cliente
zlEvtSocket_t  *cliente= zlEvtSocketClienteConnect(
               base
             , "127.0.0.1"
             , 5000
             ,socket_cb
             , NULL);

// En cuanto se cierre el socket, el programa muere mientras libevent no tenga eventos que mostrar

return(0);
}

