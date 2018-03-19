
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <memory.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <errno.h>
#include <signal.h>


#include <fcntl.h>

/* -----------------------------------------------------------------
   Con el nombre de host  --> devuelve el IP.
   Si hay exito tenemos el IP en la variable y en el retorno.
   Retorna 0 si no existe IP (IP="").
  
   Si no damos el suficiente espacio a la variable ip, la cosa petará

   Using:
    int main()
    {
     char ip[200];
     char host[200];
     strcpy(host,"0far.com");printf("<%s><%s>\n",host,MskIpDeHost(ip,host));
    }

   ----------------------------------------------------------------- */
char *zlHostbyName(char *ip, char *host)
{
 struct hostent *hostlocal;
 struct in_addr mi_addr;
 int ret;

 strcpy(ip,"");

 /* Buscar la direccion de la red en el host-origen */
 if ((hostlocal=gethostbyname(host))==0)
    return(NULL);
 memcpy(&mi_addr, hostlocal->h_addr, hostlocal->h_length);
 strcpy(ip, inet_ntoa(mi_addr));
 return(ip);
}                   


