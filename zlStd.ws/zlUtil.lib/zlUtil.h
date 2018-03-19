#include "file2Buf.h"
#include "zlValidIpv4.h"

long atolm(char *buf,int len);


/* La forma de recoger el directorio y nombre de un fichero a partir de un path */
/*
char path[100];
strcpy(path,"/home/concn/dat/prueba.txt");
char *file=basename(path);
printf("<%s>\n",file);
char *dir=dirname(path);
printf("<%s><%s><%s>\n",path,dir,file);
// Este sistema destroza la variable path

*/

/* Para recoger el directorio y fichero de un path hago estas funciones*/
char *zDirname(char *path,char *aux);
char *zBasename(char *path,char *aux);

