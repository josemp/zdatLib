

typedef struct
{
  struct zlConexionItem *item; // si es nulo, es que no está activo
  int nLog;
} zlConexion_t;
typedef struct
{
   int numConexiones;
   //struct zlConexionItem **item;
   zlConexion_t *conexion; 
} zlRegistroConexion_t;

int zlRegistroConexionAdd(struct zlConexionItem *conexionItem);
void zlRegistroConexionFree(zlRegistroConexion_t *registroConexion);
void zlRegistroConexionItemFree(zlRegistroConexion_t *registroConexion,
                                int nItem);
