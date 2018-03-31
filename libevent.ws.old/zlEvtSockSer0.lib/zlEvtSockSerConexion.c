#include <event2/listener.h>
#include <event2/bufferevent.h>
#include <event2/buffer.h>

#include <arpa/inet.h>
#include <netinet/tcp.h>

#include <signal.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "zlLog.h"
#include "zlEvtSockSer.h"


int zlRegistroConexionAdd(zlConexionItem_t *conexionItem)
{
	int i;
	LogW(9,"Add conexion\n");
	zlRegistroConexion_t *conexiones=&conexionItem->server->registroConexion;
	for (i=0;i<conexiones->numConexiones;i++)
	{
		if (conexiones->conexion[i].item == NULL)
		{		
			conexionItem->nItem = i;
            conexiones->conexion[i].item = conexionItem;			
			return(i);
		}
	}
	i=conexiones->numConexiones;
	conexionItem->nItem = i;
	if (i==0)
		conexiones->conexion=malloc(sizeof(zlConexion_t));
    else
		conexiones->conexion=realloc(conexiones->conexion,sizeof(zlConexion_t)*(unsigned int)(i+1));
	//Crear el log
	LogW(9,"Intentando crear el log\n");
    if (conexionItem->server->prefijoLog!=NULL && strlen(conexionItem->server->prefijoLog) > 0)
	{
		char aux[300];	
		sprintf(aux,"%s%03d.log",conexionItem->server->prefijoLog,i);
		conexiones->conexion[i].nLog=LgInit(aux, conexionItem->server->levelLog);
		LogW(9,"Log creado numeri<%d> en item <%d> fichero <%s> nivel <%d>\n",
		       conexiones->conexion[i].nLog,i,
			   aux,
			   conexionItem->server->levelLog);
		
	}
	conexiones->conexion[i].item=conexionItem;
	conexiones->numConexiones++;
	return(i);
}
void zlRegistroConexionItemFree(zlRegistroConexion_t *registroConexion,int nItem)
{
	if (nItem<0 || nItem>=registroConexion->numConexiones || registroConexion->conexion[nItem].item==NULL)
		return;
	
	registroConexion->conexion[nItem].item=NULL;// esto es como marcar registro no activo
}
void zlRegistroConexionFree(zlRegistroConexion_t *registroConexion)
{
	//int i;
	/*
	for (i=0;i<registroConexion->numConexiones;i++)
		if (regostroConexion->item[i]!=NULL)
			free(registroConexion[i]);
	*/
	if (registroConexion->numConexiones >0)
		free(registroConexion->conexion);
	free(registroConexion);	
}