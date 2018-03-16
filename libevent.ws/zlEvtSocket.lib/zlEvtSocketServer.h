struct zlEvtSocketServerData;
typedef void (*zlSocketBind_i) (int estado,
                    struct zlEvtSocketServerData *serverSocket);

typedef struct zlEvtSocketServerData
{
struct event_base *base;

struct timeval timeoutBind;

struct event *evTimeoutBind;
struct evconnlistener *listener; //listener de libevent
zlSocketBind_i bind_i;//callback de conexion
zlSocketConnect_i connect_i;
zlSocketRead_i read_i;
zlSocketClose_i close_i;
zlSocketWrite_i write_i;
int puerto;
zlEvtSocketList_t *lista;
short estadoEnable;
void * tag;
}zlEvtSocketServerData_t;

void zlEvtSocketServerFree(zlEvtSocketServerData_t  *serverData);

void zlEvtSocketServerData(zlEvtSocketServerData_t *serverData,
					int puerto,
					int timeoutBind,
					zlSocketBind_i bind_i,
					zlSocketConnect_i connect_i,
					zlSocketRead_i read_i,
					zlSocketClose_i close_i,
					zlSocketWrite_i write_i,
					zlEvtSocketList_t *lista,
					void * tag);
					  
void zlServerSocketLanza(zlEvtSocketServerData_t *serverData, struct event_base *base, short estadoEnable);					  
					  
					  
					  