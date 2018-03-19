typedef enum {zlrBufReadEstadoInicio,zlrBufReadEstadoData,zlrBufReadEstadoFinal,zlrBufReadEstadoCortado} zlrBufReadEstado_t;

typedef struct
{
char *buffer; /* El buffer no es guardado por la libreria*/
char separador[11];
long len;//len del buffer
long numLinea;
void *tag;
int (*lectura)();
}zlrBufRead_t;
char *zlrBufReadEstadoGetDescri(zlrBufReadEstado_t estado);
zlrBufRead_t * zlrBufReadInicia(char *buf,long len,char *separador,int (*lectura)(),void *tag);
void zlrBufReadFree(zlrBufRead_t *buffer);
int zlrBufRead(zlrBufRead_t *buffer);
