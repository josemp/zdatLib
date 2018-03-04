
#define MAX_PATH_LOG  300


typedef struct {
   char Activo;
   char LogFileName[MAX_PATH_LOG+1];   /* Nombre del fichero Log       */
   FILE *LogFd;                      /* variable FILE del Log        */
   int  LogLevel;                    /* Nivel Maximo asociado al Log */
   long MaxLen;                      /* Maxima longitud del log */
   short Pasa;                       /* veces que ha pasado la maxima longitud */ 
   char Programa[MAX_PATH_LOG+1];
}zlLog_t;

//extern zlLog_t *zlLog;
