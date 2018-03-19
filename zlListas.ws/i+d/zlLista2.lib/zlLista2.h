//Lista variable



typedef struct {
char nombre[100+1];
int len;
void *item; // puntero a data;
}zlLista2Item_t;
typedef struct {
  int num;// numero de items
  int len;
  zlLista2Item_t *items;
  void *data; // aqui se guardan los datos
} zlLista2_t;

zlLista2_t *zlLista2Crea(char * nombre,int len,void *tag);

void *zlLista2Item(zlLista2_t *zlLista2,int num);
void zlLista2Add(zlLista2_t *zlLista2,void *item);
void zlLista2Free(zlLista2_t *zlLista2);
void zlLista2For(zlLista2_t *zlLista2);
void *zlLista2Next(zlLista2_t *zlLista2);
