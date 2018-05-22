/**
  @file zlListap.c
  @brief Lista de recursos de punteros
  @author Jose Miguel Parra
  @date 8/2017
  
  Esta lista, crea punteros, no controla el tipo de puntero por lo que no se comveniente para guardar distintos tipos de punteros, no pueden borrarse items
  Se pueden guardar como excepcion char * acabados en nulo, porque tenemos el strlen para ver el contenido.

  Using:
  suponemos una lista de estructuras de tipo prueba_t

  crear: lista de estructuras typedef struct prueba_t
  zlListap_t * lista=listaCrea("lista de nombres",sizeof(prueba_t),NULL);
  
  add: añadimos un  elemento a la lista (prueba_t uno)
   listaAdd(lista,&uno); 

  bucle:
    for (i=0;i<lista->num;i++)
      prueba_t *item=itemLista(lista,i);

  free:
    listaFreeItem(lista);
    zlListapFreeItem(lista,  pos); //Cuidado es long
    listaFree(lista);

*/

#include <stdio.h>
#include <stdlib.h>
#include <memory.h>
#include <zlStd.h>
#include "zlListap.h"


zlListap_t *zlListapCrea(char * nombre,void *tag)
{
zlListap_t *lista =malloc(sizeof(zlListap_t));
memset(lista,0,sizeof(zlListap_t));
strcpy(lista->nombre,nombre);
lista->tag=tag;
lista->items=malloc(1);
return(lista);
}

void *zlListapItem(zlListap_t *lista,long num)
{
  return((void *) lista->items[num]);
}



zlBool_t zlListapUpdate(zlListap_t *lista,long pos,void *item)
{
if (pos>= lista->num) return(zlFalse);
if (lista->items[pos]!=NULL) return(zlFalse);
// no hay realloc en este caso //lista->items=realloc(lista->items,sizeof(void *)  *(pos+1));
lista->items[pos] = item;
lista->numActivos++;
}



void zlListapAppend(zlListap_t *lista,void *item)
{
long pos =lista->num;

lista->items=realloc(lista->items,sizeof( void * ) *(pos+1));
lista->items[pos] = item;
lista->num++;
lista->numActivos++;
}


// La posicion Donde se grabara un nuevo item de la lista
long zlListapUpdateOrAppendCheckPos(zlListap_t *lista)
{
  return zlListapFirstItemFree(lista);

}

// Colocar un item en la lista en la posicion debida
void zlListapUpdateOrAppendPos(zlListap_t *lista,void *item, long pos)
{
  if (pos==lista->num)
       zlListapAppend( lista, item);
  else
       zlListapUpdate( lista, pos, item);


}
long zlListapUpdateOrAppend(zlListap_t *lista,void *item)
{
  long pos= zlListapFirstItemFree(lista);
  zlListapUpdateOrAppendPos(lista,item, pos);
  return(pos);
}
void zlListapFreeItem(zlListap_t *lista, long pos)
{
  lista->numActivos--;
  lista->items[pos]=NULL;
}

void zlListapFree(zlListap_t *lista)
{
free(lista->items);
free(lista);
}
void zlListapFor(zlListap_t *lista)
{
   lista->itera=0;
}
void *zlListapNext(zlListap_t *lista)
{
 if (lista->itera>=lista->num || lista->itera<0) return NULL;
 lista->itera++;
 return zlListapItem(lista,lista->itera-1);
}
long zlListapFirstItemFree(zlListap_t *lista)
{
 long i;
 for(i=0;i<lista->num;i++)
   if (lista->items[i]==NULL)
     return(i);
   return(lista->num);
}
