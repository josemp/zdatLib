#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <memory.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include <event2/event.h>
#include <event2/bufferevent.h>

#include "zlEventMain.h"
#include "zlLog.h"

struct event_base *base;
int main(int argc,char **argv)
{
eventoIniciaPrevio(argc,argv);
base = event_base_new ();
eventoInicia(base,argc,argv);
//printf("Main iniciados los eventos\n");
  event_base_dispatch(base );
  event_base_free(base );
return(0);
}
struct event_base *zlEventMainBase()
{
	return(base);
}	


void zlEventDescBevEvent (int evento )
{
return;
printf ("evento {\n" );
if ( evento & BEV_EVENT_CONNECTED )
   printf ("BEV_EVEVENT_CONNECTED\n" );

if ( evento & BEV_EVENT_READING )
   printf ("BEV_EVENT_READING\n" );

if ( evento & BEV_EVENT_WRITING )
   printf ("BEV_EVENT_WRITING\n" );

if ( evento & BEV_EVENT_ERROR )
   printf ("BEV_EVENT_ERROR\n" );

if ( evento & BEV_EVENT_TIMEOUT )
   printf ("BEV_EVENT_TIMEOUT\n" );

if ( evento & BEV_EVENT_EOF )
   printf ("BEV_EVENT_EOF\n" );

printf ("}\n" );

}
/* pasado a zlEvtSockets.lib 
struct sockaddr *zlEventSockaddr ( struct sockaddr_in *adr ,char * ip, int puerto )
{
//  int socks ;
//int fd;
  //struct hostent * hostlocal ;
  //int ret ;
  unsigned long miip;
  //int lgadr = sizeof (*adr );
 memset (adr , 0, sizeof (struct sockaddr_in  ));
 miip =(unsigned long) inet_addr ( ip);            /-Direcc: Pasar formato puntos a long -/
 adr ->sin_addr . s_addr= miip ;     /- /usr/include/netinet> in.h  in_f.h -/
 adr ->sin_port = htons( (uint16_t) puerto ); /-Num puerto  -->/etc/services -/
 adr ->sin_family = AF_INET ;       /- Familia direccion --> dominio Internet -/

  return ((struct sockaddr *)adr );
}
*/
