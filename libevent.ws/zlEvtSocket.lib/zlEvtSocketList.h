typedef struct
{
  struct zlEvtSocket *item; // si es nulo, es que no está activo
  int nLog;
} zlEvtSocketItems_t;
typedef struct
{
   int numConexiones;
   char prefijoLog[201];
   int  levelLog;
   //struct zlConexionItem **item;
   zlEvtSocketItems_t *items; 
   
} zlEvtSocketList_t;

int zlEvtSocketListAdd(zlEvtSocketList_t *lista,struct zlEvtSocket *item);

void zlEvtSocketListFreeItem(zlEvtSocketList_t *lista,int nItem);

void zlEvtSocketListFree(zlEvtSocketList_t *lista);
