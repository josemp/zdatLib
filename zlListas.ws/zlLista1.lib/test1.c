#include <stdio.h>
#include <stdlib.h>
#include <memory.h>
#include <string.h>
#include "zlLista1.h"


typedef struct {

int uno;
char dos[10];
}prueba_t;
int main()
{
// Crear zlLista1
zlLista1_t *zlLista1=zlLista1Crea("mizlLista1",sizeof(prueba_t) ,NULL);

// Items
prueba_t item1={1,"uno"};
prueba_t item2={2,"dos"};

// Add items a zlLista1
zlLista1Add(zlLista1,&item1);
zlLista1Add(zlLista1,&item2);


// Ejemplo de formar bucles

printf("metodo 1 Lista completa\n");
prueba_t *items=(prueba_t *)zlLista1->items;
int i;
for (i=0;i<zlLista1->num;i++)
  printf("item[%d]=<%d><%s>\n",i,items[i].uno,items[i].dos);

printf("metodo 2 usar zlLista1Item\n");
for (i=0;i<zlLista1->num;i++)
  printf("item[%d]=<%d><%s>\n",i,
                                ((prueba_t *)zlLista1Item(zlLista1,i))->uno
                               ,((prueba_t *)zlLista1Item(zlLista1,i))->dos);
printf("metodo 3 usar zlLista1Item mejor\n");
for (i=0;i<zlLista1->num;i++)
   {
    prueba_t *item=(prueba_t *) zlLista1Item(zlLista1,i);
  printf("item[%d]=<%d><%s>\n",i,
                                item->uno
                               ,item->dos);
   }

printf("metodo 4 zlLista1For/zlLista1Next\n");
zlLista1For(zlLista1);
while(1)
{
 prueba_t *item=(prueba_t *) zlLista1Next(zlLista1);
 if (item==NULL) break;
  printf("item[%d]=<%d><%s>\n",zlLista1->itera -1,
                                item->uno
                               ,item->dos);

}
printf("metodo 5 zlLista1For/zlLista1Next\n");
zlLista1For(zlLista1);
prueba_t *it;
while((it=(prueba_t *) zlLista1Next(zlLista1))!=NULL)
{
  printf("item[%d]=<%d><%s>\n",zlLista1->itera -1,
                                it->uno
                               ,it->dos);

}
zlLista1Free(zlLista1);
return(0);
}
