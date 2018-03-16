/**
  @file leeMetaTab.c
  @brief Lectura de ficheros meta de configuracion z
  @author Jose Miguel Parra
  @date 8/2017
  
  Esta lista, copia los datos, no pueden borrarse items

  Using:
  suponemos una lista de estructuras de tipo prueba_t

  crear: lista de estructuras typedef struct prueba_t
  zlLista1_t * lista=listaCrea("lista de nombres",sizeof(prueba_t),NULL);
  
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
#include "zlLista1.h"


zlLista1_t *zlLista1Crea(char * nombre,int len,void *tag)
{
zlLista1_t *lista =malloc(sizeof(zlLista1_t));
memset(lista,0,sizeof(zlLista1_t));
strcpy(lista->nombre,nombre);
lista->len=len;
lista->tag=tag;
lista->items=malloc(1);
return(lista);
}
void *zlLista1Item(zlLista1_t *lista,int num)
{
  return(lista->items+num*lista->len);
}
void zlLista1Add(zlLista1_t *lista,void *item)
{
int pos =lista->num;

lista->items=realloc(lista->items,lista->len *(pos+1));
memcpy(lista->items+(pos*lista->len),item,lista->len);
lista->num++;
}
void zlLista1Free(zlLista1_t *lista)
{
free(lista->items);
free(lista);

}
void zlLista1For(zlLista1_t *lista)
{
   lista->itera=0;
}
void *zlLista1Next(zlLista1_t *lista)
{
 if (lista->itera>=lista->num || lista->itera<0) return NULL;
 lista->itera++;
 return zlLista1Item(lista,lista->itera-1);
}

