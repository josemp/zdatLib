typedef enum {ZL_CI_DATOS,ZL_CI_CIERRE} zlServerTipoEvento_e;
typedef void (*zlServerSocketReadInterfaz_i)(
                    zlServerTipoEvento_e tipo,
					struct evbuffer *input,
                    struct zlConexionItem *conexionItem); 
					
typedef void (*zlServerSocketConnectInterfaz_i) (
                    struct zlConexionItem *conexionItem);
typedef void (*zlServerSocketCloseInterfaz_i) (
                    struct zlConexionItem *conexionItem);
typedef void (*zlServerSocketWriteInterfaz_i) (
                    struct zlConexionItem *conexionItem);
typedef struct zlConexionItem
{
	struct bufferevent *bev;
	int nItem;	
	
    
    
	struct zlServerSocket *server;
	//int closeOnWrite;
	void *conexionTag; // concerniente a la conexion
	void *progTag; // concerniente al proceso dentro de la conexion
	           
} zlConexionItem_t;

int zlclog(zlConexionItem_t *conexionItem);
void *zlConexionItemGetTag(zlConexionItem_t *conexionItem); //El tag del servicio socket

void *zlConexionItemGetConexionTag(zlConexionItem_t *conexionItem); // El tag de la conexion
void *zlConexionItemGetProgTag(zlConexionItem_t *conexionItem); // El tag del actual estado generalmente enviado por write
zlConexionItem_t *zlConexionItemCrea(struct zlServerSocket *server,struct bufferevent *bev);

void zlConexionItemFree(zlConexionItem_t *conexionItem);

void zlConexionCommandClose(zlConexionItem_t *conexionItem);

void zlServerSocketWriteCommand(int closeOnWrite,zlConexionItem_t *conexionItem,
     char *buf,
     int len, void *proTag);
