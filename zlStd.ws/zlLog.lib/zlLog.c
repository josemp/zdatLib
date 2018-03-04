/* RUTINAS DE LOGS */

#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <memory.h>
#include <time.h>
#include <assert.h>
#include "zlLogs.h"
#include "zlLog.h"
zlLog_t MiPanic;
zlLog_t *zlLog;         
zlLog_t zlLogs;
static int zlLogFlush;
static zlLog_t zlLog0;         
static unsigned int zlLogNum;
void LgW(int NumLog,int nivel_pedido,char *fmt, ...);
void Panic(int nivel_pedido,char *fmt, ...);
void LogW(int nivel_pedido,char *fmt, ...);




void zlLogFlushOff()
{
	zlLogFlush=0;
}
void zlLogFlushOn()
{
	zlLogFlush=1;
}
int validaNumLog(int NumLog,int nivel_pedido)
{
 if (!zlLog[NumLog].LogFd)
    return(0);
 if (zlLog[NumLog].LogLevel <=0)
    return(0);
 if (nivel_pedido >zlLog[NumLog].LogLevel)
    return(0);
 return(1);
}	

int validaLog(int nivel_pedido)
{
 if (!zlLogs.LogFd)
    return(0);
 if (zlLogs.LogLevel <=0)
    return(0);
 if (nivel_pedido >zlLogs.LogLevel)
    return(0);
 return(1);
	
}

/* Inicializo Logs */
/* Si devuelve 0 ha habido error  */
/*            -1 todo ha ido bien */
void PanicInit(char *FileName,char *ProgramName)
{
 strcpy(MiPanic.LogFileName,FileName);
 strcpy(MiPanic.Programa, ProgramName);
 MiPanic.Pasa=0;
 MiPanic.MaxLen=2000000l;

 MiPanic.LogFd=fopen(FileName,"a");
 if (MiPanic.LogFd)
   return;
 fprintf(stderr,"Error Abriendo Panic\n");
 sleep(3);
 exit(1);

}

int LgInit(char *FileName, int Nivel)
{
unsigned int ret;
 ret=zlLogCrea();
 
 /* Copiamos el nombre del Log y el Nivel Maximo en la estructura */
 zlLog[ret].LogLevel=Nivel;
 if (FileName==NULL)
 	 strcpy(zlLog[ret].LogFileName, "");
 else
	 strcpy(zlLog[ret].LogFileName, FileName);
 zlLog[ret].Pasa=0;
 zlLog[ret].MaxLen=2000000l;

 if (Nivel <=0) return((int)ret);   /* el Nivel Maximo debe ser mayor que 0 */
 /* abrimos el log (no lo sobreescribimos, añadimos al final) */ 
 if (FileName==NULL)
 {
	 zlLog[ret].LogFd=stdout;
	 return((int)ret);
 }
 zlLog[ret].LogFd=fopen(FileName,"a");
 if (zlLog[ret].LogFd)
   return((int) ret);                /* error en apertura de fichero */
 zlLog[ret].LogLevel=0;
 return((int)ret);
}

// Equivale a LogInit(NULL,nivel)
void LogInitStd(int Nivel)
{
 /* Copiamos el nombre del Log y el Nivel Maximo en la estructura */
 zlLogs.LogLevel=Nivel;
 zlLogs.Pasa=0;
 zlLogs.MaxLen=2000000l;

 if (Nivel <=0) return;   /* el Nivel Maximo debe ser mayor que 0 */
 /* abrimos el log (no lo sobreescribimos, añadimos al final) */ 
 zlLogs.LogFd=stdout;
}
// Nombre nulo hacia std
int LogInit(char *FileName, int Nivel)
{
int ret=0;
 /* Copiamos el nombre del Log y el Nivel Maximo en la estructura */
 zlLogs.LogLevel=Nivel;
 if (FileName==NULL)
	 strcpy(zlLogs.LogFileName, "");	 
 else 
  strcpy(zlLogs.LogFileName, FileName);
 zlLogs.Pasa=0;
 zlLogs.MaxLen=2000000l;

 if (Nivel <=0) return(-1);   /* el Nivel Maximo debe ser mayor que 0 */
 /* abrimos el log (no lo sobreescribimos, añadimos al final) */ 
if (FileName==NULL)
 {
	 zlLogs.LogFd=stdout;
	 return(0);
 }

 zlLogs.LogFd=fopen(FileName,"a");
 if (zlLogs.LogFd)
   return(ret);                /* error en apertura de fichero */
 zlLogs.LogLevel=0;
 return(0);
}


/* Cierro Logs */
/* para cerrarlo su Nivel Maximo ha de ser >0, debe existir y estar abierto */

void LgClose(int NumLog)
{
  if (NumLog >= zlLogNum) return;
  zlLog[NumLog].Activo=0;
 if (zlLog[NumLog].LogLevel <=0)
 {
   zlLog[NumLog]=zlLog0;
   return;
 }
 if (zlLog[NumLog].LogFd && zlLog[NumLog].LogFd!=stdout)
   fclose(zlLog[NumLog].LogFd);
 zlLog[NumLog]=zlLog0;
}


/*  Generico para el prefijo del log */

void logPrefijo(FILE *out,int nivel_pedido)
{
 struct tm *tiempos;
 time_t x;
 long y;
  x=time(&y);
 tiempos=localtime(&x);
 fprintf(out,"%d %02d/%02d %02d:%02d:%02d ",
         nivel_pedido,
         tiempos->tm_mday,
         tiempos->tm_mon+1,
         tiempos->tm_hour,
         tiempos->tm_min,
         tiempos->tm_sec
         );
	
	
}

void LogClose()
{
  zlLogs.Activo=0;
 if (zlLogs.LogLevel <=0)
 {
   zlLogs=zlLog0;
   return;
 }
 if (zlLogs.LogFd && zlLogs.LogFd!=stdout)
   fclose(zlLogs.LogFd);
   zlLogs=zlLog0;
}




/* Escribo en fichero Log */
/* el fichero debe existir y estar abierto                        */
/* a la funcion se le pasa una lista de parametros:               */
/*    nivel, formato, dia, mes, hora, min, seg                    */
/*     si nivel>Nivel Maximo no escribe en fichero nada           */
/*     formato es el texto entre " que se escribira en el log     */
/* en el fichero la informacion se guarda de la siguiente forma:  */    
/*    nivel, dia, mes, hora, min, seg, formato                    */
void PanicOld(int nivel_pedido,char *format, ...)
{
 if (MiPanic.LogFd==NULL) return;
 logPrefijo(MiPanic.LogFd,nivel_pedido);
 
va_list ap;
va_start(ap,format);
vfprintf(MiPanic.LogFd,format,ap);
va_end(ap);
 fflush(MiPanic.LogFd);   
 if (nivel_pedido>0)
 {
   exit(1);
 }
}

void Panic(int expresion,char *format, ...)
{
 if (expresion!=0) return; // la expresion es true
 if (MiPanic.LogFd==NULL) return;
 logPrefijo(MiPanic.LogFd,expresion);

va_list ap;
va_start(ap,format);
vfprintf(MiPanic.LogFd,format,ap);
va_end(ap);
 fflush(MiPanic.LogFd);
assert(expresion!=0);
}

void LgW(int NumLog,int nivel_pedido,char *format, ...)
{
 FILE *ou;
if (NumLog == -1)
{
  if (validaLog(nivel_pedido)==0) return;
  ou=zlLogs.LogFd;
}
else
{
	if (validaNumLog(NumLog,nivel_pedido)==0) return;
	ou=zlLog[NumLog].LogFd;
}
	 

 logPrefijo(ou,nivel_pedido);
 va_list ap;
 va_start(ap,format);
 vfprintf(ou,format,ap);
 va_end(ap);
 if (zlLogFlush==1) fflush(ou);   
 return;
}

void LogW(int nivel_pedido,char *format, ...)
{
if (validaLog(nivel_pedido)==0) return;
logPrefijo(zlLogs.LogFd,nivel_pedido);
va_list ap;
va_start(ap,format);
vfprintf(zlLogs.LogFd,format,ap);
va_end(ap);
   

if (zlLogFlush==1) fflush(zlLogs.LogFd);   		 
}	


unsigned int zlLogCrea()
{
unsigned int i;
if (zlLogNum==0)
   {
    zlLog=(zlLog_t *)malloc(sizeof(zlLog_t));
    zlLog[0]=zlLog0;
    zlLog[0].Activo=1;
    zlLogNum=1;
    return(0);
   }

for (i=0;i<zlLogNum;i++)
 {
  if (zlLog[i].Activo==0 )
   {
    zlLog[i].Activo=1;
    return(i);
   }
 }



zlLog=(zlLog_t *)realloc(zlLog,(zlLogNum+1)*sizeof(zlLog_t));
zlLog[zlLogNum]=zlLog0;
zlLog[zlLogNum].Activo=1;
zlLogNum += 1;
return(zlLogNum-1);

}



