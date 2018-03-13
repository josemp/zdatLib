#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <libgen.h>

int limpiaIniciales(char *buf ,int len)
{
 int i;
 int lenp=0;
 for (i=0;i<len;i++)
   {
     if (lenp==0 && buf[i]==' ')
	       continue;
     buf[lenp]=buf[i];
     lenp++;
   }
 for (i=lenp;i<len;i++)
      buf[i]=0;
 return(lenp);
}
int quita0d0a(char *buf)
{
int i;
for (i=0;i<strlen(buf);i++)
{
if(buf[i]=='\n' || buf[i]=='\r')
{
 buf[i]=0;
return(1);
}
}
return(0);
}

int limpiaFinales(char *buf ,int len)
{
 int i;
 int lenp=len;
 for (i=(len)-1;i>=0;i--)
   {
     if (buf[i]==' ')
     { buf[i]=0;lenp--;}
     else
       break;
   }
 return(lenp);
}

int strLower(char *buf,int len)
{
int i;
for (i=0;i<len;i++)
  buf[i]=tolower(buf[i]);
}

void quitaMates(char *aux)
{
 int len;
 int i;
 len=strlen(aux);
 int pos=0;
 for (i=0;i<len;i++)
 {
  if (aux[i]=='.')
    continue;
  aux[pos]=aux[i];
  pos++;
  
 }
 aux[pos]=0;
 len=pos;
 for (i=0;i<len;i++)
 {
  if (aux[i]==',')
	  aux[i]='.';
  
 }

return;
}


int atoim(char *buf,int len)
{
char aux[200];
sprintf(aux,"%.*s",len,buf);
return(atoi(aux));



}
long atolm(char *buf,int len)
{
char aux[200];
sprintf(aux,"%.*s",len,buf);
return(atol(aux));
}

int limpiaBuffer(char *buf,int len)
{
 int lenp,ret;
 lenp=limpiaIniciales(buf,len);
 ret=limpiaFinales(buf,lenp);
 return(ret);
}



/* En estas funciones aux tiene que estar inicializado */

char *zDirname(char *path,char *aux)
{
char *pathCopia = strdup(path);
strcpy(aux,dirname(pathCopia));
free(pathCopia);
return(aux);

}
char *zBasename(char *path,char *aux)
{
char *pathCopia = strdup(path);
strcpy(aux,basename(pathCopia));
free(pathCopia);
return(aux);

}

