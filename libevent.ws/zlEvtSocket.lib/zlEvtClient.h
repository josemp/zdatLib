
// No usado
typedef enum {CLIENTE_BASIC_DISCONNECT} zlEvtClientTipoEvento_e;

struct zlEvtClient_s;
struct zlEvtClientSocketParam_s;
typedef zlBool_t (*zlEvtClient_i) (
                   zlEvtClientTipoEvento_e tipo
                   , struct zlEvtClient_s *cliente
                   ,void *tag);


typedef struct zlEvtClient_s
{
  struct event_base *base;
  char ip[100];
  int puerto;
  int timeoutConnect;

  zlEvtClient_i client_cb; //callback de eventos cliente
  zlEvtSocket_i socket_cb; // callback de  eventos socket

  int numConexiones;// numero de conexiones. Corresponde con el canal del socket
  zlListap_t *conexiones;
  void * tagProtocolo;
  void * tag;
}zlEvtClient_t;



zlEvtClient_t *zlEvtClientInicia (
          struct event_base *base
         , char *ip
         ,int puerto
         ,zlEvtClient_i client_cb
         ,zlEvtSocket_i socket_cb
         ,void * tag );

void zlEvtClientDisconectConexion(zlEvtClient_t *client,long canal);

zlEvtSocket_t *zlEvtClientCommandConnect(zlEvtClient_t *client);

