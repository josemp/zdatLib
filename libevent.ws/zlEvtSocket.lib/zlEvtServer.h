

typedef enum {SERVER_BASIC_BIND,SERVER_BASIC_CONNECT,SERVER_BASIC_DISCONNECT} zlEvtServerTipoEvento_e;

struct zlEvtServer_s;
struct zlEvtServerSocketParam_s;

// El retorno bool_t no está usado
typedef zlBool_t (*zlEvtServer_i) (
                   zlEvtServerTipoEvento_e tipo
                   , struct zlEvtServer_s *server
                   , zlBool_t estado // para el bind, false-> bind no correcto (hay reintentos) true si hay bind 
                   , struct zlEvtServerSocketParam_s *socketParam
                   ,void *tag);


typedef struct zlEvtServer_s
{
  struct event_base *base;
  int puerto;

  struct timeval timeoutBind;
  struct event *evTimeoutBind;
  struct evconnlistener *listener; //listener de libevent

  zlEvtServer_i server_cb; //callback de eventos server
  zlEvtSocket_i socket_cb; // callback de  eventos socket

//  int numConexiones;// numero de conexiones. Corresponde con el canal del socket. El numero de conexiones esta en la lista conexiones
  zlBool_t  isBinding;
  zlListap_t *conexiones;
  void * tag;
}zlEvtServer_t;


/*
typedef struct zlEvtServerSocketParam_s
{
struct evconnlistener *listener;
evutil_socket_t fd;
struct sockaddr *address;
int socklen;
}zlEvtServerSocketParam_t;
*/

zlEvtServer_t *zlEvtServerInicia (
          struct event_base *base
         ,int puerto 
         ,int timeoutAccept
         ,zlEvtServer_i server_cb
         ,zlEvtSocket_i socket_cb
         ,void * tag );

zlEvtSocket_t  *zlEvtServerSocketConnect(zlEvtServer_t *server
                  ,zlEvtServerSocketParam_t *socketParam
                  , long canal
                  , void *tag
                  );
long zlEvtSocketServerNumConexiones(zlEvtServer_t *server);
