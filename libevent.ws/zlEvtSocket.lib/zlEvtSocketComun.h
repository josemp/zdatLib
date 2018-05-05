
// Declara la estructura porque la usamos antes de definirla
struct zlEvtSocket_s;

//Los tipos del callback
typedef enum {
              ZLEVT_SOCKET_CONNECT
             ,ZLEVT_SOCKET_DATOS
             ,ZLEVT_SOCKET_WRITE
             ,ZLEVT_SOCKET_FIN
             ,ZLEVT_SOCKET_TIMEOUT
             ,ZLEVT_SOCKET_RECHAZADO
              }  zlEvtSocketTipoEvento_e;

//El tipo de callback 

typedef void (*zlEvtSocket_i)(
              zlEvtSocketTipoEvento_e tipo
             ,struct evbuffer *input
             , int numTimer
             ,struct zlEvtSocket_s *socket
             , void *tagProtocolo
             , void *tag);

// Timeouts

typedef struct
{
int numero;// iniciarlas con el número
int tiempo;
zlEvtTimer_t *timer;
struct zlEvtSocket_s *socket;
void * tag;
} zlEvtSocketTimer_t;


// La estructura

#define SERVER_BASIC_NUM_TIMERS 10

typedef struct zlEvtSocket_s {
long canal; // solo para server, es el numConexiones de la conexion
char ip[ 100 ];
int puerto ;
evutil_socket_t fd;
//zlServerBasicSocketParam_t *socketParam; // liberar
struct event_base *base;
struct bufferevent *buffer;
void *serverOrClient; // datos del manejador de sockets cliente o server
void (*cancel_cb)();
zlEvtSocket_i socket_cb;
int closeOnWrite;
zlBool_t isConnected;
void *tag;
void *tagProtocolo; // Este tag lo creo para que sea creado en una conexion y liberado al final de la conexion. Solo datos de protocolo.
zlEvtSocketTimer_t timer[SERVER_BASIC_NUM_TIMERS]; // 10 timers para usar como queramos
} zlEvtSocket_t;


typedef struct zlEvtServerSocketParam_s
{
struct evconnlistener *listener;
evutil_socket_t fd;
struct sockaddr *address;
int socklen;
}zlEvtServerSocketParam_t;


void  zlEvtSocketFree(zlEvtSocket_t *socket);
zlEvtSocket_t  *zlEvtSocketServerConnect(
                  struct event_base *base
                  ,zlEvtServerSocketParam_t *socketParam
                  ,zlEvtSocket_i socket_cb
                  ,void *server
                  ,void (*cancel_cb)()
                  , long canal
                  , void *tag
                  );

zlEvtSocket_t  * zlEvtSocketClienteConnect(
               struct event_base *base
             , char *ip
             , int puerto
             , zlEvtSocket_i socket_cb
             , void *client
             , void (*cancel_cb)()
             , void *tag);

zlEvtSocket_t  * zlEvtSocketClienteConnectBasic(
               struct event_base *base
             , char *ip
             , int puerto
             , zlEvtSocket_i socket_cb
             , void *tag);

void zlEvtSocketTimerActiva(zlEvtSocket_t *socket,int num,int tiempo,void *tag);

void zlEvtSocketWriteCommand(zlEvtSocket_t *socket,char *buf,int len);


struct sockaddr *zlEventSockaddr ( struct sockaddr_in *adr ,char * ip, int puerto );
