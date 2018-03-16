

struct clienteGeneric;
typedef enum {CLIENTE_GENERIC_CONNECT,CLIENTE_GENERIC_FIN,CLIENTE_GENERIC_DATOS} clienteGenericTipoEvento_e;
typedef void (*clienteGenericReadInterfaz)(clienteGenericTipoEvento_e tipo,struct evbuffer *input,struct clienteGeneric *cliente, void *tag);

typedef struct clienteGeneric
{
struct event_base *base ;
char ip[ 100 ];
int puerto ;
struct bufferevent *buffer;
int closeOnWrite;//Finalizar al terminar el envio
clienteGenericReadInterfaz read_i;
void *tag;
} clienteGeneric_t ;


void clienteGenericWriteCommandCierre(clienteGeneric_t *cliente,char *buf,int len,int cierraAlTerminar);
void clienteGenericWriteCommand(clienteGeneric_t *cliente,char *buf,int len);
void clienteGenericFree(clienteGeneric_t *cliente);
clienteGeneric_t *clienteGenericInicia ( char *ip , int puerto,clienteGenericReadInterfaz read_i );
void clienteGenericLanza (clienteGeneric_t * cliente ,struct event_base *base );

