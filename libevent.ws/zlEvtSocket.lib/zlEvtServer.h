

typedef enum {SERVER_BASIC_BIND,SERVER_BASIC_CONNECT} zlEvtServerTipoEvento_e;

struct zlEvtServer_s;
struct zlEvtServerSocketParam_s;
typedef void (*zlEvtServer_i) (
                   zlEvtServerTipoEvento_e tipo
                   , struct zlEvtServer_s *server
                   , zlBool_t estado
                   , struct zlEvtServerSocketParam_s *socketParam
                   ,void *tag);


typedef struct zlEvtServer_s
{
struct event_base *base;
int puerto;
struct timeval timeoutBind;
//struct timeval timeoutCloseConexion;
struct event *evTimeoutBind;
struct evconnlistener *listener; //listener de libevent
zlEvtServer_i server_cb; //callback de lectura
int numConexiones;// numero de conexiones. Corresponde con el canal del socket
void * tag;
}zlEvtServer_t;


typedef struct zlEvtServerSocketParam_s
{
struct evconnlistener *listener;
evutil_socket_t fd;
struct sockaddr *address;
int socklen;
}zlEvtServerSocketParam_t;


zlEvtServer_t *zlEvtServerInicia (
          struct event_base *base
         ,int puerto 
         ,int timeoutAccept
         ,zlEvtServer_i server_cb
         ,void * tag );


