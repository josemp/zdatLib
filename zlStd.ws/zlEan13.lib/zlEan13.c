#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <zlStdComun.h>
#include "zlEan13.h"


// De articulo de 6
int zlEan13DigitodeArticulo(char *articulo)
{
char entrada[20];
int i=0;
int suma=0;
sprintf(entrada,"847000%-6.6s0",articulo);
return( zlEan13Digito(entrada));
}


// de string >=12 calcula el digito de los 12 primeros
int zlEan13Digito(char *entrada)
{

int i=0;
int suma=0;
for (i=0;i<12;i++)
   if (i%2==1)
      suma=suma+3*(entrada[i]-0x30);
   else
     suma=suma+entrada[i]-0x30;
return( ( (10- (suma % 10) ) %10 ) + 0x30 );

}
// de string de 13 pon el digito que corresponda
void setEan13Digito(char *entrada)
{

entrada[12]= (char) zlEan13Digito(entrada);
}

zlBool_t zlEan13Check(char *entrada)
{
if ( entrada[12]== (char) zlEan13Digito(entrada))
   return(zlTrue);
else
   return(zlFalse);

}
