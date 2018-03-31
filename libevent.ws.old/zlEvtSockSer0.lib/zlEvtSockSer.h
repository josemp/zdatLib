//struct zlConexionItem;
#include "zlEvtSockSerConexion.h" // registro de conexiones

#include "zlEvtSockSerConexionItem.h"

typedef void (*zlServerSocketBindInterfaz_i) (int estado,
                    struct zlServerSocket *serverSocket);

//Para registrar conexión si procede

typedef struct zlServerSocket
{
struct event_base *base;
void * tag;
struct timeval timeoutBind;
struct timeval timeoutCloseConexion;
struct event *evTimeoutBind;
struct evconnlistener *listener; //listener de libevent
zlServerSocketBindInterfaz_i bind_i;//callback de conexion
zlServerSocketConnectInterfaz_i connect_i;//callback de conexion
zlServerSocketReadInterfaz_i read_i; //callback de lectura
zlServerSocketCloseInterfaz_i close_i;// callback de cierre
zlServerSocketWriteInterfaz_i write_i;// callback de write
int registrar;
int puerto;
zlRegistroConexion_t registroConexion;
char prefijoLog[200];
int levelLog;
}zlServerSocket_t;


zlServerSocket_t * zlServerSocketInicia (int puerto ,int timeoutAccept,
          int timeoutCloseConexion,
		  int registrar,
		  char *prefijoLog,
		  int levelLog,
		  zlServerSocketBindInterfaz_i bind_i, 
		  zlServerSocketConnectInterfaz_i connect_i,
		  zlServerSocketReadInterfaz_i read_i,
		  zlServerSocketWriteInterfaz_i write_i,
		  zlServerSocketCloseInterfaz_i close_i,
          void * tag );
void zlServerSocketLanza(zlServerSocket_t *serverMain, struct event_base *base);

void zlServerSocketFree(zlServerSocket_t *server);


