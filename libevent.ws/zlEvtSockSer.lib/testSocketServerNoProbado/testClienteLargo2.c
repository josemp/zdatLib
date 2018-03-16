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
   Utilizando los event write 
    bufferevent_enable(conexionItem->bev , EV_WRITE);
	bufferevent_disable(conexionItem->bev , EV_READ);
   
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
		event_free(c3_sigusr1_event);
	

	Al quedarse libevent sin eventos, el programa saldrá solito.
	
   
   NOTA. La lectura se hace en un hilo y  señala con SIGUSR1 y he utilizado libevent para capturar la señal en sigusr1_cb
	NOTA. Realmente el unico callback que usamos es el de read. El resto es por motivo del test y chequear resultados




*/





static long c3_lenEnviado;
zlServerSocket_t * c3_server;
struct event *c3_sigusr1_event; 
char c3_TEXTO[]="hola";
FILE *in;
void c3_writeFile(char *file,struct zlConexionItem *conexionItem);
static void c3_sigusr1_cb(int sock, short which, void *arg) {
        /* Whoops: Calling event_active on the same event unconditionally
           from within its callback means that no other events might not get
           run! */
        printf("LLEGA SIGNAL SIGUSR1 c3_TEXTO LEIDO <%ld> <%ld>\n",simpleClienteGetlenRead(),c3_lenEnviado);fflush(stdout);
		assert(simpleClienteGetReadEstado()>0);
		assert(c3_lenEnviado==simpleClienteGetlenRead());
		//assert(!strcmp(c3_TEXTO,simpleClienteGetBuffer()));
							//zlServerSocketFree(c3_server);
					event_free(c3_sigusr1_event);
		//close(simpleClienteGetSockfd());
       // event_active(ev, EV_WRITE, 0);
}





void c3_bindSocket_cb(int estado,zlServerSocket_t *server)
{
	printf("LLEGA BIND ESTADO <%d>\n",estado);fflush(stdout);
	assert(estado==1);
	printf("que pasa con el asserrrrrrrrt\n");fflush(stdout);
	int fdSocket=simpleSocketCliente(3500);
    printf("Abierto el socket cliente\n");fflush(stdout);
    assert(fdSocket>=0);
}
void c3_writeSocket_cb(struct zlConexionItem *conexionItem)
{
	char buf[1001];
	
	FILE *in=(FILE *)conexionItem->progTag;	
	
	if (in==NULL)
	{
  	   
		in=fopen("file3000.txt","r");
		assert(in!=NULL);
		c3_lenEnviado=0;
		//conexionItem->progTag=in; ya lo hace el write command
	}
  	    
		
	if (fgets(buf,1000,in)!=NULL)
	{
		 
		zlServerSocketWriteCommand(0,conexionItem,buf,(int)strlen(buf),in); // hacemos eco
		c3_lenEnviado= c3_lenEnviado+ (long) strlen(buf);
	}
	else {
	   printf("Fin de envio de fichero\n");fflush(stdout);
	   buf[0]=3;
	   zlServerSocketWriteCommand(1,conexionItem,buf,1,NULL); /* primer parametro =1 significa que cierre al terminar */
	   c3_lenEnviado= c3_lenEnviado+1;
	   fclose(in);
	}
	
}
void c3_readSocket_cb(zlServerTipoEvento_e tipo,
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
	assert( strcmp(aux,c3_TEXTO)==0);
	conexionItem->progTag=NULL;
    bufferevent_enable(conexionItem->bev , EV_WRITE);
	bufferevent_disable(conexionItem->bev , EV_READ);
	//c3_writeFile("file3000.txt",conexionItem);
	//zlServerSocketWriteCommand(1,conexionItem,aux,lenMsg,NULL); // hacemos eco
	//zlEventDeferedFunctionTime(2,0,escribeDiferido,conexionItem);
	
	LgW(zlclog(conexionItem),9,"llegan cosas\n");
}
void c3_writeFile(char *file,struct zlConexionItem *conexionItem)
{
	char buf[1001];
	c3_lenEnviado=0;
	FILE *in=fopen(file,"r");
	assert(in!=NULL);
	while(fgets(buf,1000,in)!=NULL)
	{
		zlServerSocketWriteCommand(0,conexionItem,buf,(int)strlen(buf),NULL); // hacemos eco
		c3_lenEnviado= c3_lenEnviado+(long) strlen(buf);
	}
	printf("Fin de envio de fichero\n");fflush(stdout);
	buf[0]=3;
	zlServerSocketWriteCommand(1,conexionItem,buf,1,NULL); /* primer parametro =1 significa que cierre al terminar */
	c3_lenEnviado= c3_lenEnviado+1;
	fclose(in);
	
}
void c3_connectSocket_cb(struct zlConexionItem *conexionItem)
				{
					//bufferevent_disable(conexionItem->bev , EV_READ);
					conexionItem->conexionTag=NULL;
					printf("LLega c3_connectSocket_cb\n");fflush(stdout);
					
					int ret=simpleClienteWrite(c3_TEXTO);
					printf("simple Cliente envia <%s> retorna <%d>\n",c3_TEXTO,ret);fflush(stdout);
					assert(ret==strlen(c3_TEXTO));
					simpleClienteReadLargo1Hilo();

				}
void c3_closeSocket_cb(struct zlConexionItem *conexionItem)
				{
					printf("LLega closesocket_cb\n");fflush(stdout);
					// LLEGA ANTES DE CERRAR EL SOCKET Y DESCTRUIR CONEXIONITEM
					
					printf("LLega c3_closeSocket_cb\n");fflush(stdout);
					zlServerSocketFree(c3_server);
					
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
void testClienteLargo2(struct event_base *base)
{
	
	 evsignal_add((c3_sigusr1_event=evsignal_new(base, SIGUSR1, c3_sigusr1_cb, NULL)), NULL);

 c3_server = zlServerSocketInicia(3500, // puerto
                                              1,// timeout seg accept
											  1,// timeout seg close conexion
											  1,// registrar si o no
											  "msi",//prefijo log
											  9,
											  c3_bindSocket_cb,
											  c3_connectSocket_cb,
											  c3_readSocket_cb, // interfaz lectura
											  c3_writeSocket_cb, // interfaz write
											  c3_closeSocket_cb,
											  NULL );	//tag
zlServerSocketLanza(c3_server, base);	
//zlDefered_t * z=
//zlEventDeferedFunctionTime(3,0,funcionDiferidaCliente, server);
//pthread_t id;

//pthread_create(&id ,NULL,hiloCliente,NULL );


	
}
