

typedef enum {SERVER_BASIC_BIND,SERVER_BASIC_CONNECT} serverBasicTipoEvento_e;

struct zlServerBasic_s;
struct zlServerBasicSocketParam_s;
typedef void (*zlServerBasic_i) (
                   serverBasicTipoEvento_e tipo
                   , struct zlServerBasic_s *serverBasic
                   , zlBool_t estado
                   , struct zlServerBasicSocketParam_s *socketParam
                   ,void *tag);


typedef struct zlServerBasic_s
{
struct event_base *base;
int puerto;
struct timeval timeoutBind;
//struct timeval timeoutCloseConexion;
struct event *evTimeoutBind;
struct evconnlistener *listener; //listener de libevent
zlServerBasic_i server_cb; //callback de lectura
void * tag;
}zlServerBasic_t;


typedef struct zlServerBasicSocketParam_s
{
struct evconnlistener *listener;
evutil_socket_t fd;
struct sockaddr *address;
int socklen;
}zlServerBasicSocketParam_t;


zlServerBasic_t *zlServerBasicInicia (
          struct event_base *base
         ,int puerto 
         ,int timeoutAccept
         ,zlServerBasic_i server_cb
         ,void * tag );


