
// Declara la estructura porque la usamos antes de definirla
struct zlEvtServerBasicSocket_s;

//Los tipos del callback
typedef enum {
              SERVER_BASIC_SOCKET_CONNECT
             ,SERVER_BASIC_SOCKET_DATOS
             ,SERVER_BASIC_SOCKET_WRITE
             ,SERVER_BASIC_SOCKET_FIN
             ,SERVER_BASIC_SOCKET_TIMEOUT
              }  zlServerBasicSocketTipoEvento_e;

//El tipo de callback 

typedef void (*zlServerBasicSocket_i)(
              zlServerBasicSocketTipoEvento_e tipo
             ,struct evbuffer *input
             , int numTimer
             ,struct zlEvtServerBasicSocket_s *socket
             , void *tag);


// Timeouts

typedef struct
{
int numero;// iniciarlas con el número
int tiempo;
zlEvtTimer_t *timer;
struct zlEvtServerBasicSocket_s *socket;
void * tag;
} zlEvtServerBasicTimer_t;


// La estructura
#define SERVER_BASIC_NUM_TIMERS 10
typedef struct zlEvtServerBasicSocket_s {

zlServerBasicSocketParam_t *socketParam; // liberar
struct event_base *base;
struct bufferevent *buffer;
zlServerBasic_t *server;
zlServerBasicSocket_i socket_cb;
int closeOnWrite;
void *tag;
zlEvtServerBasicTimer_t timer[SERVER_BASIC_NUM_TIMERS]; // 10 timers para usar como queramos
} zlEvtServerBasicSocket_t;




void  zlEvtServerBasicSocketFree(zlEvtServerBasicSocket_t *socket);
zlEvtServerBasicSocket_t  *zlEvtServerBasicSocketConnect(
                  zlServerBasicSocketParam_t *socketParam
                  ,zlServerBasicSocket_i socket_cb
                  ,zlServerBasic_t *server
                  , void *tag
                  );



void zlEvtServerBasicTimerActiva(zlEvtServerBasicSocket_t *socket,int num,int tiempo,void *tag);

