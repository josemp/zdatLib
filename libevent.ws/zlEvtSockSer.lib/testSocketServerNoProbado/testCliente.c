#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <memory.h>
#include <event2/event.h>
#include <event2/bufferevent.h>
#include <event2/buffer.h>
#include <signal.h>
#include <unistd.h>
#include <assert.h>
#include <pthread.h>
#include "zlEventMain.h"
#include "zlLog.h"
#include "zlEvtTimer.h"
#include "zlEvtSockSer.h"
#include "simpleSocketCliente.h"
/*
   Chequea una simple conexion Eco utilizando el simpleSocketCliente
   
   Cliente ---> Server    "Hola"
   Server  ---->Cliente   "hola"
   Cliente cierra la conexion
   
   
   Lanzar con:  
               void testCliente(struct event_base *base);
	Lanza el servidor con los correspondientes callback
    Manejador de SIGUSR1, para enterarnos cuando termina de leer el simpleClienteGetBuffer	
				sigusr1_cb
	En el "bindSocket_cb" hacemos la conexion y verificamos que es correcta
		int fdSocket=simpleSocketCliente(3500);

	En el "connectSocket_cb" el socketcliente envía el texto y lanzamos el thread de lectura  cliente (thread para que no interfiera)
					int ret=simpleClienteWrite(TEXTO);				
					assert(ret==strlen(TEXTO));
					simpleClienteReadHilo();
					
	Si todo ha ido bien, en el readSocket_cb leeremos el texto enviado por el cliente y enviaremos el echo al cliente 
				zlServerSocketWriteCommand(0,conexionItem,aux,lenMsg,NULL); 
	
	La lectura del cliente es bloqueante, por lo cual está esperado ha leer algo y una vez que termine lanzará SIGUSR1
	En sigusr1_cb si el texto leido es correcto, se cierra el socket cliente.
		close(simpleClienteGetSockfd());
	
	Ha de llegar entonces un close a la conexion del server y en "closeSocket_cb" cerramos todo
	Al quedarse libevent sin eventos, el programa saldrá solito.
	
   
   NOTA. La lectura se hace en un hilo y  señala con SIGUSR1 y he utilizado libevent para capturar la señal en sigusr1_cb





*/
	struct event *sigusr1_event; 
char TEXTO[]="hola";
static void sigusr1_cb(int sock, short which, void *arg) {
        /* Whoops: Calling event_active on the same event unconditionally
           from within its callback means that no other events might not get
           run! */
        printf("LLEGA SIGNAL SIGUSR1 TEXTO LEIDO <%s>\n",simpleClienteGetBuffer());fflush(stdout);
		
		close(simpleClienteGetSockfd());
       // event_active(ev, EV_WRITE, 0);
}


void funcionDiferidaWrite(void *tag)
{
zlServerSocket_t * server=(zlServerSocket_t *) tag;
printf(" ---- CIERRE CONTROLADO ------ testCierre\n");
zlServerSocketFree(server);
}


void bindSocket_cb(int estado,zlServerSocket_t *server)
{
	printf("LLEGA BIND ESTADO <%d>\n",estado);fflush(stdout);
	assert(estado==1);
	printf("que pasa con el asserrrrrrrrt\n");fflush(stdout);
	int fdSocket=simpleSocketCliente(3500);
    printf("Abierto el socket cliente\n");fflush(stdout);
    assert(fdSocket>=0);
}
void readSocket_cb(zlServerTipoEvento_e tipo,
                struct evbuffer *input,
				struct zlConexionItem *conexionItem)
{
   	char aux[300];
	printf("LLegan datos al servidor\n");fflush(stdout);
	if (tipo==ZL_CI_CIERRE){printf("cierre\n");fflush(stdout); return;}
	int lenMsg= (int) evbuffer_get_length(input);
	evbuffer_remove(input, aux,(size_t) lenMsg );// Leemos todo el buffer
	printf("llega <%.*s>\n",lenMsg,aux);fflush(stdout);
	zlServerSocketWriteCommand(0,conexionItem,aux,lenMsg,NULL); // hacemos eco
	//zlEventDeferedFunctionTime(2,0,escribeDiferido,conexionItem);
	
	printf("llegan cosas\n");
}
void connectSocket_cb(struct zlConexionItem *conexionItem)
				{
					
					conexionItem->conexionTag=NULL;
					printf("LLega connectSocket_cb\n");fflush(stdout);
					
					int ret=simpleClienteWrite(TEXTO);
					printf("simple Cliente envia <%s> retorna <%d>\n",TEXTO,ret);fflush(stdout);
					assert(ret==strlen(TEXTO));
					simpleClienteReadHilo();

				}
void closeSocket_cb(struct zlConexionItem *conexionItem)
				{
					// LLEGA ANTES DE CERRAR EL SOCKET Y DESCTRUIR CONEXIONITEM
					assert(!strcmp(TEXTO,simpleClienteGetBuffer()));
					printf("LLega closeSocket_cb\n");fflush(stdout);
					zlServerSocketFree(conexionItem->server);
					event_free(sigusr1_event);
					
				}
/*
void funcionDiferidaCliente(void *tag)
{
printf("Entrando en socket cliente\n");fflush(stdout);
socketTestClient(3500);

}
*/
/*
void * hiloCliente(void *arg)
{
//char * dato=(char *) arg; // el argumento puede ser cualquier cosa, pero luego la hemos de traducir
sleep(3);

simpleSocketCliente(3500);
return(NULL);
}
*/
// Con esta función iniciamos un socket server, pero lo cerramos en 3 segundos
void testCliente(struct event_base *base)
{
	
	 evsignal_add((sigusr1_event=evsignal_new(base, SIGUSR1, sigusr1_cb, NULL)), NULL);

	zlServerSocket_t * server = zlServerSocketInicia(3500, // puerto
                                              1,// timeout seg accept
											  1,// timeout seg close conexion
											  1,// registrar si o no
											  "msi", // prefijo log
											  9,
											  bindSocket_cb,
											  connectSocket_cb,
											  readSocket_cb, // interfaz lectura
											  NULL, // interfaz write
											  closeSocket_cb,
											  NULL );	//tag
zlServerSocketLanza(server, base);	
//zlDefered_t * z=
//zlEventDeferedFunctionTime(3,0,funcionDiferidaCliente, server);
//pthread_t id;

//pthread_create(&id ,NULL,hiloCliente,NULL );


	
}
