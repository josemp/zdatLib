
typedef struct {
  int itera;
  int num;// numero de items
  char nombre[100+1];// nombre de la zlLista1
  int len;
  char *items;
  void *tag;
} zlLista1_t;

zlLista1_t *zlLista1Crea(char * nombre,int len,void *tag);

void *zlLista1Item(zlLista1_t *zlLista1,int num);
void zlLista1Add(zlLista1_t *zlLista1,void *item);
void zlLista1Free(zlLista1_t *zlLista1);
void zlLista1For(zlLista1_t *zlLista1);
void *zlLista1Next(zlLista1_t *zlLista1);
