#define MAXBUF 1024
typedef struct {
int sockfd;
char buffer[MAXBUF];
int retEstado;
long lenLeido;
int terminado;

pthread_t tid;
} multiSocketCliente_t;

long multiClienteGetlenRead(multiSocketCliente_t *multi);
int multiClienteGetReadEstado(multiSocketCliente_t *multi);
int multiClienteGetSockfd(multiSocketCliente_t *multi);
char *multiClienteGetBuffer(multiSocketCliente_t *multi);
int multiSocketCliente(multiSocketCliente_t *multi,int puerto);

/* ---------------------------------------- */

void multiClienteReadHilo(multiSocketCliente_t *multi);
void *multiClienteRead(void *arg);
int  multiClienteWrite(multiSocketCliente_t *multi,char *buf);


void *multiClienteReadLargo(void *arg);
void multiClienteReadLargoHilo(multiSocketCliente_t *multi);




