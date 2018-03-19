/**
  @file zlLista2.c
  @brief Lista de recursos de longitud fija            
  @author Jose Miguel Parra
  @date 8/2017
  
  Esta lista, copia los datos, no pueden borrarse items

  Using:
  suponemos una lista de estructuras de tipo prueba_t

  crear: lista de estructuras typedef struct prueba_t
  zlLista2_t * lista=listaCrea("lista de nombres",sizeof(prueba_t),NULL);
  
  add: añadimos un  elemento a la lista (prueba_t uno)
   listaAdd(lista,&uno); 

  bucle:
    for (i=0;i<lista->num;i++)
      prueba_t *item=itemLista(lista,i);

  free:
    listaFree(lista);

*/

#include <stdio.h>
#include <stdlib.h>
#include <memory.h>
#include "zlLista2.h"


zlLista2_t *zlLista2Crea(char * nombre,int len,void *tag)
{
zlLista2_t *lista =malloc(sizeof(zlLista2_t));
memset(lista,0,sizeof(zlLista2_t));
strcpy(lista->nombre,nombre);
lista->len=len;
lista->tag=tag;
lista->items=malloc(1);
return(lista);
}
void *zlLista2Item(zlLista2_t *lista,int num)
{
  return(lista->items+num*lista->len);
}
void zlLista2Add(zlLista2_t *lista,void *item)
{
int pos =lista->num;

lista->items=realloc(lista->items,lista->len *(pos+1));
memcpy(lista->items+(pos*lista->len),item,lista->len);
lista->num++;
}
void zlLista2Free(zlLista2_t *lista)
{
free(lista->items);
free(lista);

}
void zlLista2For(zlLista2_t *lista)
{
   lista->itera=0;
}
void *zlLista2Next(zlLista2_t *lista)
{
 if (lista->itera>=lista->num || lista->itera<0) return NULL;
 lista->itera++;
 return zlLista2Item(lista,lista->itera-1);
}

