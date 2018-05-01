#include <stdio.h>
#include <stdlib.h>
#include <memory.h>
#include <string.h>
#include <zlStd.h>
#include "zlListap.h"


typedef struct {

int uno;
char dos[10];
}prueba_t;
int main()
{
// Crear zlListap
zlListap_t *zlListap=zlListapCrea("mizlListap",NULL);

// Items
prueba_t item1={1,"uno"};
prueba_t item2={2,"dos"};

// Add items a zlListap
zlListapUpdateOrAppend(zlListap,&item1);
zlListapUpdateOrAppend(zlListap,&item2);



// Ejemplo de formar bucles

printf("metodo 1 Lista completa\n");
prueba_t **items=(prueba_t **)zlListap->items;
int i;
for (i=0;i<zlListap->num;i++)
  printf("item[%d]=<%d><%s>\n",i,items[i]->uno,items[i]->dos);

printf("metodo 2 usar zlListapItem\n");
for (i=0;i<zlListap->num;i++)
  printf("item[%d]=<%d><%s>\n",i,
                                ((prueba_t *)zlListapItem(zlListap,i))->uno
                               ,((prueba_t *)zlListapItem(zlListap,i))->dos);
printf("metodo 3 usar zlListapItem mejor\n");
for (i=0;i<zlListap->num;i++)
   {
    prueba_t *item=(prueba_t *) zlListapItem(zlListap,i);
  printf("item[%d]=<%d><%s>\n",i,
                                item->uno
                               ,item->dos);
   }

printf("metodo 4 zlListapFor/zlListapNext\n");
zlListapFor(zlListap);
while(1)
{
 prueba_t *item=(prueba_t *) zlListapNext(zlListap);
 if (item==NULL) break;
  printf("item[%d]=<%d><%s>\n",zlListap->itera -1,
                                item->uno
                               ,item->dos);

}
printf("metodo 5 zlListapFor/zlListapNext\n");
zlListapFor(zlListap);
prueba_t *it;
while((it=(prueba_t *) zlListapNext(zlListap))!=NULL)
{
  printf("item[%d]=<%d><%s>\n",zlListap->itera -1,
                                it->uno
                               ,it->dos);

}
zlListapFree(zlListap);
return(0);
}
