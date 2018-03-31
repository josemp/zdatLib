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
#include "multiSocketCliente.h"

/*
   
   dos clientes envían un hola y el servidor envía de fichero que hemos puesto en rest/resources
El cliente es el multiSocketCliente que se basa en una estructura multiSocketCliente_t para guardar cosas
La lectura la hace en un hilo 
   
   Utilizando los event write 
    bufferevent_enable(conexionItem->bev , EV_WRITE);
	bufferevent_disable(conexionItem->bev , EV_READ);
   
   Dos veces
   Cliente ---> Server    "Hola"
   Server  ---->Cliente   Envia fichero
   Server cierra la conexión, esto hace que la operativa sea ligeramente diferente del testCliente.c
   
   
   Lanzar con:  
               void testVariosClientes(struct event_base *base);
	Lanza el servidor con los correspondientes callback
    Manejador de SIGUSR1, para enterarnos cuando termina de leer el simpleClienteGetBuffer	
				sigusr1_cb
	En el "bindSocket_cb" hacemos la conexion y verificamos que es correcta
		fdSocket=multiSocketCliente(&c4_cliente[0],3500);
		fdSocket=multiSocketCliente(&c4_cliente[1],3500);
		ret=multiClienteWrite(&c4_cliente[0],c4_TEXTO);
		ret=multiClienteWrite(&c4_cliente[1],c4_TEXTO);
        
	En el "connectSocket_cb" no hacemos nada (antes haciamos el write, pero ahora no podemos
					
					
	Si todo ha ido bien, en el readSocket_cb leeremos el texto enviado por el cliente y enviaremos el echo al cliente pero con orden de cerrar 
				zlServerSocketWriteCommand(1,conexionItem,aux,lenMsg,NULL); 
	El cierre llega, pero antes que el "sigusr1_cb"
	En  "closeSocket_cb" no podemos hacer nada
	
	La lectura del cliente es bloqueante, por lo cual está esperado ha leer algo y una vez que termine lanzará SIGUSR1, puede lanzar el evento una o dos veces, puesto que si coinciden
	la señal segunda no llega
	Usamos la variable terminado que es entera (0-> no terminado el thread, 1-> terminado el thread 2-> chequeado )y supongo que es segura para threads
	
	Es de notar que En sigusr1_cb tenemos que chequear los dos clientes. Caso de terminar los dos no solamente liberamos el evento
		event_free(c4_sigusr1_event);
	sino también el servidor
       zlServerSocketFree(c4_server);
	Al quedarse libevent sin eventos, el programa saldrá solito.
	
   
   NOTA. La lectura se hace en un hilo y  señala con SIGUSR1 y he utilizado libevent para capturar la señal en sigusr1_cb
	NOTA. Realmente el unico callback que usamos es el de read. El resto es por motivo del test y chequear resultados




*/




static multiSocketCliente_t c4_cliente[2];
static long c4_lenEnviado;
zlServerSocket_t * c4_server;
struct event *c4_sigusr1_event; 
char c4_TEXTO[]="hola";

void c4_writeFile(char *file,struct zlConexionItem *conexionItem);
static void c4_sigusr1_cb(int sock, short which, void *arg) {
        /* Whoops: Calling event_active on the same event unconditionally
           from within its callback means that no other events might not get
           run! */
		   printf("llega c4_sigusr1 <%d><%d>\n",c4_cliente[0].terminado,c4_cliente[1].terminado);fflush(stdout);
		   int ret;
		   int ret1;
		   int ncli;
		   //ret=pthread_kill(c4_cliente[0].tid,0);// ret=0 es correcto, por lo que el thread todavía existe
		   
		   if (c4_cliente[0].terminado==1)
		   {
			   ncli=0;
			   printf("LLEGA SIGNAL SIGUSR1 de ncli=<%d> c4_TEXTO LEIDO <%ld> <%ld>\n",ncli,multiClienteGetlenRead(&c4_cliente[ncli]),17316L);fflush(stdout);
		     assert(multiClienteGetReadEstado(&c4_cliente[ncli])>0);
		     assert(17316L==multiClienteGetlenRead(&c4_cliente[ncli]));
			 c4_cliente[ncli].terminado=2;
		   }
			   ncli=0;
		   if (c4_cliente[1].terminado==1)
		   {
			   	ncli=1;
			   printf("LLEGA SIGNAL SIGUSR1 de ncli=<%d> c4_TEXTO LEIDO <%ld> <%ld>\n",ncli,multiClienteGetlenRead(&c4_cliente[ncli]),17316L);fflush(stdout);
		     assert(multiClienteGetReadEstado(&c4_cliente[ncli])>0);
		     assert(17316L==multiClienteGetlenRead(&c4_cliente[ncli]));
			 c4_cliente[ncli].terminado=2;
		   }
		if (c4_cliente[0].terminado==2 && c4_cliente[1].terminado==2)
		{
			event_free(c4_sigusr1_event);
			zlServerSocketFree(c4_server);
		}			
		   

		//assert(!strcmp(c4_TEXTO,simpleClienteGetBuffer()));
							//zlServerSocketFree(c4_server);
					//event_free(c4_sigusr1_event);
		//close(simpleClienteGetSockfd());
       // event_active(ev, EV_WRITE, 0);
}





void c4_bindSocket_cb(int estado,zlServerSocket_t *server)
{
	int fdSocket;
	printf("LLEGA BIND ESTADO <%d>\n",estado);fflush(stdout);
	assert(estado==1);
	printf("que pasa con el asserrrrrrrrt\n");fflush(stdout);
	fdSocket=multiSocketCliente(&c4_cliente[0],3500);
    printf("Abierto el socket cliente\n");fflush(stdout);
    assert(fdSocket>=0);
	fdSocket=multiSocketCliente(&c4_cliente[1],3500);
    assert(fdSocket>=0);

	int ret;
	ret=multiClienteWrite(&c4_cliente[0],c4_TEXTO);
					printf("simple Cliente envia <%s> retorna <%d>\n",c4_TEXTO,ret);fflush(stdout);
					assert(ret==strlen(c4_TEXTO));
					ret=multiClienteWrite(&c4_cliente[1],c4_TEXTO);
					printf("simple Cliente envia <%s> retorna <%d>\n",c4_TEXTO,ret);fflush(stdout);
					assert(ret==strlen(c4_TEXTO));
					multiClienteReadLargoHilo(&c4_cliente[0]);
					multiClienteReadLargoHilo(&c4_cliente[1]);
}
void c4_writeSocket_cb(struct zlConexionItem *conexionItem)
{
	char buf[1001];
	
	FILE *in=(FILE *)conexionItem->progTag;	
	
	if (in==NULL)
	{
  	   
		in=fopen("file3000.txt","r");
		assert(in!=NULL);
		c4_lenEnviado=0;
		//conexionItem->progTag=in; ya lo hace el write command
	}
  	    
		
	if (fgets(buf,1000,in)!=NULL)
	{
		 
		zlServerSocketWriteCommand(0,conexionItem,buf,(int)strlen(buf),in); // hacemos eco
		c4_lenEnviado= c4_lenEnviado+ (long) strlen(buf);
	}
	else {
	   printf("Fin de envio de fichero\n");fflush(stdout);
	   buf[0]=3;
	   zlServerSocketWriteCommand(1,conexionItem,buf,1,NULL); /* primer parametro =1 significa que cierre al terminar */
	   c4_lenEnviado= c4_lenEnviado+1;
	   fclose(in);
	}
	
}
void c4_readSocket_cb(zlServerTipoEvento_e tipo,
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
	assert( strcmp(aux,c4_TEXTO)==0);
	conexionItem->progTag=NULL;
    bufferevent_enable(conexionItem->bev , EV_WRITE);
	bufferevent_disable(conexionItem->bev , EV_READ);
	//c4_writeFile("file3000.txt",conexionItem);
	//zlServerSocketWriteCommand(1,conexionItem,aux,lenMsg,NULL); // hacemos eco
	//zlEventDeferedFunctionTime(2,0,escribeDiferido,conexionItem);
	
	LgW(zlclog(conexionItem),9,"llegan cosas\n");
}
void c4_writeFile(char *file,struct zlConexionItem *conexionItem)
{
	char buf[1001];
	c4_lenEnviado=0;
	FILE *in=fopen(file,"r");
	assert(in!=NULL);
	while(fgets(buf,1000,in)!=NULL)
	{
		zlServerSocketWriteCommand(0,conexionItem,buf,(int)strlen(buf),NULL); // hacemos eco
		c4_lenEnviado= c4_lenEnviado+(long) strlen(buf);
	}
	printf("Fin de envio de fichero\n");fflush(stdout);
	buf[0]=3;
	zlServerSocketWriteCommand(1,conexionItem,buf,1,NULL); /* primer parametro =1 significa que cierre al terminar */
	c4_lenEnviado= c4_lenEnviado+1;
	fclose(in);
	
}
void c4_connectSocket_cb(struct zlConexionItem *conexionItem)
				{
					int ret;
					//bufferevent_disable(conexionItem->bev , EV_READ);
					conexionItem->conexionTag=NULL;
					printf("LLega c4_connectSocket_cb\n");fflush(stdout);
					
					

				}
void c4_closeSocket_cb(struct zlConexionItem *conexionItem)
				{
					printf("LLega closesocket_cb\n");fflush(stdout);
					// LLEGA ANTES DE CERRAR EL SOCKET Y DESCTRUIR CONEXIONITEM
					
					printf("LLega c4_closeSocket_cb\n");fflush(stdout);
					//zlServerSocketFree(c4_server);
					
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
void testVariosClientes(struct event_base *base)
{
	
	 evsignal_add((c4_sigusr1_event=evsignal_new(base, SIGUSR1, c4_sigusr1_cb, NULL)), NULL);

 c4_server = zlServerSocketInicia(3500, // puerto
                                              1,// timeout seg accept
											  1,// timeout seg close conexion
											  1,// registrar si o no
											  "msi",//prefijo log
											  9,
											  c4_bindSocket_cb,
											  c4_connectSocket_cb,
											  c4_readSocket_cb, // interfaz lectura
											  c4_writeSocket_cb, // interfaz write
											  c4_closeSocket_cb,
											  NULL );	//tag
zlServerSocketLanza(c4_server, base);	
//zlDefered_t * z=
//zlEventDeferedFunctionTime(3,0,funcionDiferidaCliente, server);
//pthread_t id;

//pthread_create(&id ,NULL,hiloCliente,NULL );


	
}
