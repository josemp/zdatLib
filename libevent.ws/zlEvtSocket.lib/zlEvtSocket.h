struct zlEvtSocket;
#include "zlEvtSocketList.h"

typedef void (*zlSocketConnect_i) (
                    struct zlEvtSocket *socket);
typedef void (*zlSocketRead_i)(                    
					struct evbuffer *input,
                    struct zlEvtSocket *socket); 
typedef void (*zlSocketClose_i) (
                    struct zlEvtSocket *socket);
typedef void (*zlSocketWrite_i) (
                    struct zlEvtSocket *socket);


					
typedef struct zlEvtSocket
{
//struct event_base *base ; struct event_base *bufferevent_get_base(struct bufferevent *bev);
struct bufferevent *evtBuffer;
zlSocketConnect_i connect_i;
zlSocketRead_i read_i;
zlSocketClose_i close_i;
zlSocketWrite_i write_i;
void *tagConexion;
void *serverData;// Data del servidor caso que haya
int nLog;
int nItem;
zlEvtSocketList_t *lista;
} zlEvtSocket_t;

zlEvtSocket_t *zlEvtSocketCreaCliente(
    struct event_base *base,
	char *ip,
	int puerto,	
	zlEvtSocketList_t *lista,
	zlSocketConnect_i connect_i,
	zlSocketRead_i read_i,
	zlSocketClose_i close_i,
	zlSocketWrite_i write_i,
	short estadoEnable,
	void *tagConexion);

zlEvtSocket_t  *zlEvtSocketCreaServer(
    struct event_base *base,
	evutil_socket_t fd ,
	zlEvtSocketList_t *lista,	
	zlSocketRead_i read_i,
	zlSocketClose_i close_i,
	zlSocketWrite_i write_i,
	short estadoEnable,
	void *serverData,
	void *tagConexion);
	
void zlEvtSocketCloseCommand(zlEvtSocket_t *socket);

void zlEvtSocketWriteCommand(int closeOnWrite,zlEvtSocket_t *socket,
     char *buf,
     int len);
	 
	 
	 


