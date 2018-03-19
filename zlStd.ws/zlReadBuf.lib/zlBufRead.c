#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <memory.h>
#include <zlStdComun.h>
#include "zlBufRead.h"
/*
typedef struct
{
char *buffer; // El buffer no es guardado por la libreria
char separador[11];
long len;//len del buffer
long numLinea;
char *posicion;
char *endPosicion;
zlBool_t alloc;
}zlrBufRead_t;

*/


zlrBufRead_t * zlrBufReadFor(char *buf,long len,char *separador, zlBool_t alloc )
{

zlrBufRead_t *buffer;
buffer=malloc(sizeof(zlrBufRead_t));
memset(buffer,0,sizeof(zlrBufRead_t));
buffer->buffer=buf;
strcpy(buffer->separador,separador);
if (len==0)
     buffer->len=strlen(buf);
else
     buffer->len=len;
buffer->posicion=buffer->buffer;
buffer->endPosicion=buffer->buffer+buffer->len;
buffer->numLinea= -1;
buffer->alloc=alloc;
return(buffer);
}
void zlrBufReadFree(zlrBufRead_t *buffer)
{
if (buffer!=NULL)
  {
   free(buffer);
  }

}

char *zlrBufReadNext(zlrBufRead_t *buffer)
{
  char *pun;
  char *punRes;
  size_t lenBuffer;
  if (buffer->posicion==buffer->endPosicion)
  {
    zlrBufReadFree(buffer);
    return(NULL);
  }
  pun=strstr(buffer->posicion,buffer->separador);
  if (pun==NULL)
   {
    buffer->numLinea++;
    punRes=buffer->posicion;
    if (buffer->alloc==zlFalse)
     {
      buffer->posicion=buffer->endPosicion;
      return(punRes);
     }
    else
      {
        lenBuffer=buffer->endPosicion - buffer->posicion;
        punRes=malloc(lenBuffer +1);
        memset(punRes,0,lenBuffer +1);
        memcpy(punRes,buffer->posicion,lenBuffer);
        buffer->posicion=buffer->endPosicion;
        return(punRes);
      }
   }
  buffer->numLinea++;
  if (buffer->alloc==zlFalse)
   {
     punRes=buffer->posicion;
     buffer->posicion=pun +strlen(buffer->separador); 
     *pun=0;
     return(punRes);
   }
  else
   {
        lenBuffer= pun - buffer->posicion ;
        punRes=malloc(lenBuffer +1);
        memset(punRes,0,lenBuffer +1);
        memcpy(punRes,buffer->posicion,lenBuffer);
        buffer->posicion=pun +strlen(buffer->separador); 
        return(punRes);
     
   }

}

