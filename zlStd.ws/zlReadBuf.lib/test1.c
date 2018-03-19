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
