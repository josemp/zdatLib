

struct clienteGeneric1;
typedef enum {CLIENTE_GENERIC1_CONNECT,CLIENTE_GENERIC1_FIN,CLIENTE_GENERIC1_DATOS,CLIENTE_GENERIC1_TIMEOUT_R} clienteGeneric1TipoEvento_e;
typedef void (*clienteGeneric1ReadInterfaz)(clienteGeneric1TipoEvento_e tipo,struct evbuffer *input,struct clienteGeneric1 *cliente, void *tag);

typedef struct clienteGeneric1
{
struct event_base *base ;
char ip[ 100 ];
int puerto ;
struct bufferevent *buffer;
int closeOnWrite;//Finalizar al terminar el envio
clienteGeneric1ReadInterfaz read_i;
int tiempoRespuesta;
zlEvtTimer_t *timerRespuesta;
void *tag;
} clienteGeneric1_t ;


void clienteGeneric1WriteCommandCierre(clienteGeneric1_t *cliente,char *buf,int len,int cierraAlTerminar);
void clienteGeneric1WriteCommand(clienteGeneric1_t *cliente,char *buf,int len);
void clienteGeneric1Free(clienteGeneric1_t *cliente);
clienteGeneric1_t *clienteGeneric1Lanza ( struct event_base *base, char *ip , int puerto,clienteGeneric1ReadInterfaz read_i,int tiempoRespuesta ,void *cls);

