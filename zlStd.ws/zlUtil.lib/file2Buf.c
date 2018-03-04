#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <memory.h>

//acordarse hacer un free al retorno si es distinto de nulo
char *zluFile2Buf(char *fichero)
{
char buf[100+1];
char *pun;
FILE *in;
in=fopen(fichero,"r");
if (!in) return(NULL);
pun=malloc(1);
pun[0]=0;
while (fgets(buf,100,in))
{
 pun=realloc(pun,strlen(pun)+strlen(buf)+1);
 sprintf(pun+strlen(pun),"%s",buf);
}
fclose(in);
return(pun);

}
