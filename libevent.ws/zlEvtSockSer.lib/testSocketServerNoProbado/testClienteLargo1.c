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
   Chequea una simple conexion Eco utilizando un envío de fichero que hemos puesto en rest/resources
   
   Cliente ---> Server    "Hola"
   Server  ---->Cliente   Envia fichero
   Server cierra la conexión, esto hace que la operativa sea ligeramente diferente del testCliente.c
   
   
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
					simpleClienteReadLargoHilo();
					
	Si todo ha ido bien, en el readSocket_cb leeremos el texto enviado por el cliente y enviaremos el echo al cliente pero con orden de cerrar 
				zlServerSocketWriteCommand(1,conexionItem,aux,lenMsg,NULL); 
	El cierre llega, pero antes que el "sigusr1_cb"
	Ha de llegar entonces un close a la conexion del server y en "closeSocket_cb" cerramos todo menos el evento signal
	
	La lectura del cliente es bloqueante, por lo cual está esperado ha leer algo y una vez que termine lanzará SIGUSR1
	En sigusr1_cb simplemete tenemos que ver si el texto leido es correcto. Liberamos el evento signal
		event_free(c2_sigusr1_event);
	

	Al quedarse libevent sin eventos, el programa saldrá solito.
	
   
   NOTA. La lectura se hace en un hilo y  señala con SIGUSR1 y he utilizado libevent para capturar la señal en sigusr1_cb
	NOTA. Realmente el unico callback que usamos es el de read. El resto es por motivo del test y chequear resultados




*/





static long c2_lenEnviado;
zlServerSocket_t * c2_server;
struct event *c2_sigusr1_event; 
char c2_TEXTO[]="hola";
void c2_writeFile(char *file,struct zlConexionItem *conexionItem);
static void c2_sigusr1_cb(int sock, short which, void *arg) {
        /* Whoops: Calling event_active on the same event unconditionally
           from within its callback means that no other events might not get
           run! */
        printf("LLEGA SIGNAL SIGUSR1 c2_TEXTO LEIDO <%ld> <%ld>\n",simpleClienteGetlenRead(),c2_lenEnviado);fflush(stdout);
		assert(simpleClienteGetReadEstado()>0);
		assert(c2_lenEnviado==simpleClienteGetlenRead());
		//assert(!strcmp(c2_TEXTO,simpleClienteGetBuffer()));
							//zlServerSocketFree(c2_server);
					event_free(c2_sigusr1_event);
		//close(simpleClienteGetSockfd());
       // event_active(ev, EV_WRITE, 0);
}


void c2_funcionDiferidaWrite(void *tag)
{
zlServerSocket_t * server=(zlServerSocket_t *) tag;
printf(" ---- CIERRE CONTROLADO ------ testCierre\n");
zlServerSocketFree(server);
}


void c2_bindSocket_cb(int estado,zlServerSocket_t *server)
{
	printf("LLEGA BIND ESTADO <%d>\n",estado);fflush(stdout);
	assert(estado==1);
	printf("que pasa con el asserrrrrrrrt\n");fflush(stdout);
	int fdSocket=simpleSocketCliente(3500);
    printf("Abierto el socket cliente\n");fflush(stdout);
    assert(fdSocket>=0);
}
void c2_writeSocket_cb(struct zlConexionItem *conexionItem)
{
	printf("Ha llegado un write socket ESTO NO TENÍA QUE PASAR\n");fflush(stdout);
	assert(1==0);
}
void c2_readSocket_cb(zlServerTipoEvento_e tipo,
                struct evbuffer *input,
				struct zlConexionItem *conexionItem)
{
   	char aux[300];
	memset(aux,0,300);
	printf("LLegan datos al servidor\n");fflush(stdout);
	if (tipo==ZL_CI_CIERRE){printf("cierre\n");fflush(stdout); return;}
	int lenMsg= (int) evbuffer_get_length(input);
	evbuffer_remove(input, aux,(size_t) lenMsg );// Leemos todo el buffer
	printf("llega <%.*s> \n",lenMsg,aux);fflush(stdout);
	assert( strcmp(aux,c2_TEXTO)==0);
	c2_writeFile("file3000.txt",conexionItem);
	//zlServerSocketWriteCommand(1,conexionItem,aux,lenMsg,NULL); // hacemos eco
	//zlEventDeferedFunctionTime(2,0,escribeDiferido,conexionItem);
	
	printf("llegan cosas\n");
}
void c2_writeFile(char *file,struct zlConexionItem *conexionItem)
{
	char buf[1001];
	c2_lenEnviado=0;
	FILE *in=fopen(file,"r");
	assert(in!=NULL);
	while(fgets(buf,1000,in)!=NULL)
	{
		zlServerSocketWriteCommand(0,conexionItem,buf,(int)strlen(buf),NULL); // hacemos eco
		c2_lenEnviado= c2_lenEnviado+(long)strlen(buf);
	}
	LgW(0,9,"Fin de envio de fichero\n");fflush(stdout);
	buf[0]=3;
	zlServerSocketWriteCommand(1,conexionItem,buf,1,NULL); /* primer parametro =1 significa que cierre al terminar */
	c2_lenEnviado= c2_lenEnviado+1;
	fclose(in);
	
}
void c2_connectSocket_cb(struct zlConexionItem *conexionItem)
				{
					//bufferevent_disable(conexionItem->bev , EV_READ);
					conexionItem->conexionTag=NULL;
					printf("LLega c2_connectSocket_cb\n");fflush(stdout);
					
					int ret=simpleClienteWrite(c2_TEXTO);
					printf("simple Cliente envia <%s> retorna <%d>\n",c2_TEXTO,ret);fflush(stdout);
					assert(ret==strlen(c2_TEXTO));
					simpleClienteReadLargoHilo();

				}
void c2_closeSocket_cb(struct zlConexionItem *conexionItem)
				{
					printf("LLega closesocket_cb\n");fflush(stdout);
					// LLEGA ANTES DE CERRAR EL SOCKET Y DESCTRUIR CONEXIONITEM
					
					printf("LLega c2_closeSocket_cb\n");fflush(stdout);
					zlServerSocketFree(c2_server);
					
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
void testClienteLargo1(struct event_base *base)
{
	
	 evsignal_add((c2_sigusr1_event=evsignal_new(base, SIGUSR1, c2_sigusr1_cb, NULL)), NULL);

 c2_server = zlServerSocketInicia(3500, // puerto
                                              1,// timeout seg accept
											  1,// timeout seg close conexion
											  1,// registrar si o no
											  "msi",// prefijo log
											  9,
											  c2_bindSocket_cb,
											  c2_connectSocket_cb,
											  c2_readSocket_cb, // interfaz lectura
											  c2_writeSocket_cb, // interfaz write
											  c2_closeSocket_cb,
											  NULL );	//tag
zlServerSocketLanza(c2_server, base);	
//zlDefered_t * z=
//zlEventDeferedFunctionTime(3,0,funcionDiferidaCliente, server);
//pthread_t id;

//pthread_create(&id ,NULL,hiloCliente,NULL );


	
}
