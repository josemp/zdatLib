/**
  @file zlBufRead.h
  @brief Lectura de Buffers con separadores
  @author Jose Miguel Parra
  @date 8/2017

  Ejemplo de uso:

\verbatim

  #include <stdio.h>
  #include <string.h>
  #include <stdlib.h>
  #include <zlStdComun.h>
  #include "zlBufRead.h"
  
  
  int main()
  {
  char buf[]="1000$123123$qwerqwerfin$$\n123123$asdfasdf$asdfasdfin";
  zlrBufRead_t *bufRead =zlrBufReadFor(buf,0,"\n", true );
  if (bufRead==NULL) {printf("algo va mal\n");return(-1);}
  while(1)
  {
     char *campo=zlrBufReadNext(bufRead);
     if (campo==NULL) break;
     printf("campo <%s>\n",campo);
     zlrBufRead_t *bufRead1 =zlrBufReadFor(campo,0,"$", false );
     while(1)
      {
        char *campo1=zlrBufReadNext(bufRead1);
        if (campo1==NULL) break;
        printf("subcampo <%s>\n",campo1);
      }
     free(campo);
  }
  printf("fin\n");
  }

\endverbatim





*/



/**
 * @brief  zlrBufRead_t es la estructura que usamos para la lectura del buffer
 *
 * Tal y como está definido
 * 
*/
typedef struct
{
char *buffer; /* El buffer no es guardado por la libreria*/
char separador[11];
long len;//len del buffer
long numLinea;
char *posicion;
char *endPosicion;
zlBool_t alloc;
}zlrBufRead_t;



/**
  @brief zlBufReadFor. Prepara un bucle de lectura de un buffer separado por un separador

  @param buf : EL buffer que se va a leer
  @param len : La longitud del buffer, o 0 si es strlen
  @param separador : El string con los separadores
                    (no son uno u otro, sino que todos juntos)
  @param alloc : si es true significa que cada campo será creado con un malloc
                            El buffer inicial no se manipula
  @returns la estructura manejadora
  Nota : Si alloc es false, al leer el buffer se rellena con 0 los separadores
*/
zlrBufRead_t * zlrBufReadFor(char *buf,long len,char *separador, zlBool_t alloc );

/**
  @brief zlBufReadNext. lectura del buffer separado por los separadores

  @param buffer : el manejador creado con zlrBufReadFor
  @returns el buffer del campo, o NULL si ya no hay mas campos
  Nota : si alloc es false, el campo retornado es un puntero al buffer original que se ha rellenado con ceros en las posiciones del separador
         si alloc es true, el campo retornado esta creado con malloc por lo que es responsabilidad del usuario liberarlo
  Nota: El Handler zlrBufRead_t se libera cuando se  retorna NULL, por lo que no se pueden tomar por ejemplo el dato numLinea
*/
char *zlrBufReadNext(zlrBufRead_t *buffer);

