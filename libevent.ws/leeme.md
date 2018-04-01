# LIBRERIA SOCKETS BASADA EN LIBEVENT

Contenido

## Main con LIBEVENT

## Timers con LIBEVENT


## Cliente Socket

### Plantilla Cliente
```
/*
                   TEST CLIENT SOCKET zlEvt
                   
                   Compilar :
                   
fuentes: testClient.c
outherDirInclude: /usr/local/zdat/include
outherDirLib: /usr/local/zdat/lib
outherLibs: -lzlEvt -lzlStd -levent
                   
    cc -I/usr/local/zdat/include   -c -o testClient.o testClient.c
    cc -L/usr/local/zdat/lib  testClient.o -lzlEvt -lzlStd -levent -o testClient
    
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <memory.h>
#include <zlStd.h>
#include <zlEvt.h>

// Callback socket conexion

void socket_cb(
              zlEvtSocketTipoEvento_e tipo // tipo de evento (ver el switch de abajo)
             ,struct evbuffer *input       // Solo válido para ZLEVT_SOCKET_DATOS
             , int numTimer                // Solo válido para ZLEVT_SOCKET_TIMEOUT
             ,zlEvtSocket_t *socket        // La estructura interna socket
             , void *tag                   // Una closure de usuario
             )
{
    char *bufRead;
    int   lenLeido;

    switch(tipo)
     {
              case ZLEVT_SOCKET_RECHAZADO: // Solamente en cliente. No hay que hacer nada aqui respecto al socket
                   
                   break;
              case ZLEVT_SOCKET_CONNECT: // Valido en cliente y servidor
              
                   // Enviamos un texto, y preparamos la lectura;
                   bufRead=malloc(1);
                   bufRead[0]=0;
                   
                   char buf[]="Comando";
                   zlEvtSocketWriteCommand(socket,buf,strlen(buf)); // Enviamos un comando

                   break;
                   
              case ZLEVT_SOCKET_DATOS: // Se reciben datos
              
                   // Lee los datos en bufRead
                   lenLeido=evbuffer_get_length(input);
                   bufRead=realloc(bufRead,strlen(bufRead)+lenLeido+1);                   
                   evbuffer_remove(input , bufRead+strlen(bufRead),lenLeido);
                   bufRead[strlen(bufRead)]=0;
                   
                   // Chequea la respuesta
                   
                   if (bufRead[strlen(bufRead)-1]='\r')
                       zlEvtSocketFree(socket); // No pasa por ZLEVT_SOCKET_FIN
                   
                   break;
                   
              case ZLEVT_SOCKET_WRITE: // Se ha terminado de escribir
              
                   zlEvtSocketTimerActiva(socket,0,10,NULL); //Activamos un timer esperando respuesta
                   
                   break;
                   
              case ZLEVT_SOCKET_FIN: // Se produce cuando el remoto cierra la conexion
              
                   free(bufRead);
                   
                   break;
                   
              case ZLEVT_SOCKET_TIMEOUT: // Se produce cuando llega un timeout de los que hemos puesto.
                   
                   if (numTimer==0) // No puede ser otro. Se ha acabado el tiempo de respuesta
                    {
                      free(bufRead);
                      zlEvtSocketFree(socket); // No pasa por ZLEVT_SOCKET_FIN
                    }
                      
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
```
## Server Socket

### Plantilla servidor
```
/*
                   TEST SERVER SOCKET zlEvt
                   
                   Compilar :
                   
fuentes: testServer.c
outherDirInclude: /usr/local/zdat/include
outherDirLib: /usr/local/zdat/lib
outherLibs: -lzlEvt -lzlStd -levent
                   
    cc -I/usr/local/zdat/include   -c -o testClient.o testClient.c
    cc -L/usr/local/zdat/lib  testClient.o -lzlEvt -lzlStd -levent -o testClient
    
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <memory.h>
#include <zlStd.h>
#include <zlEvt.h>

// Callback server

void server_cb(
                   zlEvtServerTipoEvento_e tipo
                   , struct zlEvtServer_s *server
                   , zlBool_t estado
                   ,zlEvtServerSocketParam_t *socketParam // for connect
                   ,void *tag)
{
  
        switch(tipo){
         case SERVER_BASIC_BIND:  // Simple notificación
  

             break;
        case SERVER_BASIC_CONNECT:
  
             zlEvtSocket_t  *socket = zlEvtSocketServerConnect(
                   server->base
                  ,socketParam
                  ,socket_cb
                  ,server
                  , NULL
                  );

             
             break;
             
        default:
             
             break;
        }
}



// Callback socket conexion

void socket_cb(
              zlEvtSocketTipoEvento_e tipo // tipo de evento (ver el switch de abajo)
             ,struct evbuffer *input       // Solo válido para ZLEVT_SOCKET_DATOS
             , int numTimer                // Solo válido para ZLEVT_SOCKET_TIMEOUT
             ,zlEvtSocket_t *socket        // La estructura interna socket
             , void *tag                   // Una closure de usuario
             )
{
    char *bufRead;
    int   lenLeido;

    switch(tipo)
     {
              case ZLEVT_SOCKET_RECHAZADO: // Solamente en cliente. No hay que hacer nada aqui respecto al socket
                   
                   break;
              case ZLEVT_SOCKET_CONNECT: // Valido en cliente y servidor
              
                   // Enviamos un texto, y preparamos la lectura;
                   bufRead=malloc(1);
                   bufRead[0]=0;
                   
                   char buf[]="Comando";
                   zlEvtSocketWriteCommand(socket,buf,strlen(buf)); // Enviamos un comando

                   break;
                   
              case ZLEVT_SOCKET_DATOS: // Se reciben datos
              
                   // Lee los datos en bufRead
                   lenLeido=evbuffer_get_length(input);
                   bufRead=realloc(bufRead,strlen(bufRead)+lenLeido+1);                   
                   evbuffer_remove(input , bufRead+strlen(bufRead),lenLeido);
                   bufRead[strlen(bufRead)]=0;
                   
                   // Chequea la respuesta
                   
                   if (bufRead[strlen(bufRead)-1]='\r')
                       zlEvtSocketFree(socket); // No pasa por ZLEVT_SOCKET_FIN
                   
                   break;
                   
              case ZLEVT_SOCKET_WRITE: // Se ha terminado de escribir
              
                   zlEvtSocketTimerActiva(socket,0,10,NULL); //Activamos un timer esperando respuesta
                   
                   break;
                   
              case ZLEVT_SOCKET_FIN: // Se produce cuando el remoto cierra la conexion
              
                   free(bufRead);
                   
                   break;
                   
              case ZLEVT_SOCKET_TIMEOUT: // Se produce cuando llega un timeout de los que hemos puesto.
                   
                   if (numTimer==0) // No puede ser otro. Se ha acabado el tiempo de respuesta
                    {
                      free(bufRead);
                      zlEvtSocketFree(socket); // No pasa por ZLEVT_SOCKET_FIN
                    }
                      
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
```

