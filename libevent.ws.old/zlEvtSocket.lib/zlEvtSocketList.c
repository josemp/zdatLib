#include <event2/listener.h>
#include <event2/bufferevent.h>
#include <event2/buffer.h>

#include <arpa/inet.h>
#include <netinet/tcp.h>


#include <signal.h>
#include <string.h>
#include <stdlib.h>

#include <memory.h>
#include <stdio.h>
#include "zlLog.h"
#include "zlEvtSocket.h"

int zlEvtSocketListAdd(zlEvtSocketList_t *lista,zlEvtSocket_t *item)
{
	int i;
	LogW(9,"Add conexion\n");
	for (i=0;i<lista->numConexiones;i++)
	{
		if (lista->items[i].item == NULL)
		{		
			item->nItem = i;
			item->nLog = lista->items[i].nLog;
            lista->items[i].item = item;			
			return(i);
		}
	}
	
/* --------------- */
/*     NUEVA       */
/* --------------- */
	i=lista->numConexiones;
	item->nItem = i;
	if (i==0)
		lista->items=malloc(sizeof( zlEvtSocketItems_t));
    else
		lista->items=realloc(lista->items,sizeof( zlEvtSocketItems_t)*(unsigned int)(i+1));
	//Crear el log
	LogW(9,"Intentando crear el log\n");
	lista->items[i].nLog = -1;
    if (strlen(lista->prefijoLog) > 0)
	{
		char aux[300];	
		sprintf(aux,"%s%03d.log",lista->prefijoLog,i);
		lista->items[i].nLog=LgInit(aux, lista->levelLog);
		LogW(9,"Log creado numeri<%d> en item <%d> fichero <%s> nivel <%d>\n",
		       lista->items[i].nLog,i,
			   aux,
			   lista->levelLog);
		
	}
	lista->items[i].item=item;
	item->nItem =i;
	item->nLog = lista->items[i].nLog;
	
	lista->numConexiones++;
	return(i);
}

// Nunca se borra de esta lista, salvo el borrado final
void zlEvtSocketListFreeItem(zlEvtSocketList_t *lista,int nItem)
{
	if (nItem<lista->numConexiones)
	    lista->items[nItem].item=NULL;
}

/* La lista propiamente dicha no se borra puede ser una variable estatica */
void zlEvtSocketListFree(zlEvtSocketList_t *lista)
{
	if (lista->numConexiones >0)
		free(lista->items);
}