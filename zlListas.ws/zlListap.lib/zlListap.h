
typedef struct {
  int itera;
  long num;// numero de items
  long numActivos; // numero de items activos
  char nombre[100+1];// nombre de la zlListap
  void **items;
  void *tag;
} zlListap_t;

zlListap_t *zlListapCrea(char * nombre,void *tag);

void *zlListapItem(zlListap_t *lista,long num);

zlBool_t zlListapUpdate(zlListap_t *lista,long pos,void *item);

void zlListapUpdateOrAppendPos(zlListap_t *lista,void *item, long pos);

long zlListapUpdateOrAppendCheckPos(zlListap_t *lista);

void zlListapAppend(zlListap_t *lista,void *item);

long zlListapUpdateOrAppend(zlListap_t *lista,void *item);

void zlListapFreeItem(zlListap_t *lista, long pos);

void zlListapFree(zlListap_t *lista);
void zlListapFor(zlListap_t *lista);
void *zlListapNext(zlListap_t *lista);
long  zlListapFirstItemFree(zlListap_t *lista);
