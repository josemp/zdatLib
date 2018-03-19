
typedef enum {zlrFileReadEstadoInicio,zlrFileReadEstadoData,zlrFileReadEstadoFinal,zlrFileReadEstadoCortado} zlrFileReadEstado_t;


typedef struct
{
FILE *in;
int len;//len del buffer de lectura
void *tag;
long numLinea;
int (*lectura)();
}zlrFileRead_t;

char *zlrFileReadEstadoGetDescri(zlrFileReadEstado_t estado);

zlrFileRead_t * zlrFileReadInicia(char *fichero,long len,int (*lectura)(),void *tag);

void zlrFileReadFree(zlrFileRead_t *file);

int zlrFileRead(zlrFileRead_t *file);
