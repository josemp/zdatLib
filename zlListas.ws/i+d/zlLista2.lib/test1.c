#include <stdio.h>
#include <stdlib.h>
#include <memory.h>
#include <string.h>
#include "zlLista2.h"


typedef struct {

int uno;
char dos[10];
}prueba_t;
int main()
{
// Crear zlLista2
zlLista2_t *zlLista2=zlLista2Crea("mizlLista2",sizeof(prueba_t) ,NULL);

// Items
prueba_t item1={1,"uno"};
prueba_t item2={2,"dos"};

// Add items a zlLista2
zlLista2Add(zlLista2,&item1);
zlLista2Add(zlLista2,&item2);


// Ejemplo de formar bucles

printf("metodo 1 Lista completa\n");
prueba_t *items=(prueba_t *)zlLista2->items;
int i;
for (i=0;i<zlLista2->num;i++)
  printf("item[%d]=<%d><%s>\n",i,items[i].uno,items[i].dos);

printf("metodo 2 usar zlLista2Item\n");
for (i=0;i<zlLista2->num;i++)
  printf("item[%d]=<%d><%s>\n",i,
                                ((prueba_t *)zlLista2Item(zlLista2,i))->uno
                               ,((prueba_t *)zlLista2Item(zlLista2,i))->dos);
printf("metodo 3 usar zlLista2Item mejor\n");
for (i=0;i<zlLista2->num;i++)
   {
    prueba_t *item=(prueba_t *) zlLista2Item(zlLista2,i);
  printf("item[%d]=<%d><%s>\n",i,
                                item->uno
                               ,item->dos);
   }

printf("metodo 4 zlLista2For/zlLista2Next\n");
zlLista2For(zlLista2);
while(1)
{
 prueba_t *item=(prueba_t *) zlLista2Next(zlLista2);
 if (item==NULL) break;
  printf("item[%d]=<%d><%s>\n",zlLista2->itera -1,
                                item->uno
                               ,item->dos);

}
printf("metodo 5 zlLista2For/zlLista2Next\n");
zlLista2For(zlLista2);
prueba_t *it;
while((it=(prueba_t *) zlLista2Next(zlLista2))!=NULL)
{
  printf("item[%d]=<%d><%s>\n",zlLista2->itera -1,
                                it->uno
                               ,it->dos);

}
zlLista2Free(zlLista2);
return(0);
}
