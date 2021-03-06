#include <stdio.h>
#include <sys/time.h>
#include <sys/types.h>
#include <errno.h>
#include <dirent.h>
#include <sys/stat.h>
#include <string.h>
#include <stdlib.h>
#include <string.h>

#include <fcntl.h>
#include <fnmatch.h>

#include "zlDir.h"



tipoFile_t zlDirTipoFichero(char *pathCompleto)
{
struct stat st;
if (stat(pathCompleto, &st) == -1)
  {
    return(-1);
  }
if (S_ISREG(st.st_mode))
 return(1);
if (S_ISDIR(st.st_mode))
  return(2);
return(0);

}

// El path puede ser terminado en '/' o no, como queramos
tipoFile_t zlDirTipoFichero1(char *path,char *file)
{
char pathCompleto[MIDIR_MAX_FILE_LEN + MIDIR_MAX_FILE_PATH +1 ];
if (strlen(path)==0)
   sprintf(pathCompleto,"%s",file);
else 
{
    if (path[strlen(path)-1]=='/')
    sprintf(pathCompleto,"%s%s",path,file);
    else
    sprintf(pathCompleto,"%s/%s",path,file);
}
return(zlDirTipoFichero(pathCompleto));
}


zlDir_t *zlDirFor(char *pathCarpeta,char *match)
{
int ret;
DIR *path;
zlDir_t *zlDir;
printf("pathCarpeta = <%s>\n",pathCarpeta);
DIR *midir=opendir(pathCarpeta);
if (midir==NULL) {printf("null1\n");return(NULL);}

zlDir=malloc(sizeof(zlDir_t));
memset(zlDir,0,sizeof(zlDir_t));

zlDir->directorio=midir;
sprintf(zlDir->match,"%.100s",match);
strcpy(zlDir->path,pathCarpeta);
/*
printf("Leemos <%s>\n",zlDir->path);
zlDir->directorio=opendir(zlDir->path);
if (zlDir->directorio==NULL)
{
     printf("null2\n");
     
     free(zlDir);
     return(NULL);
}
*/
return(zlDir);
}




void zlDirClose(zlDir_t *zlDir)
{
if (zlDir->directorio!=0)
    closedir(zlDir->directorio);
free(zlDir);
}




char *zlDirNext(zlDir_t *zlDir)
{
struct dirent *dp;
int ret;
tipoFile_t tipoFile;
  if (zlDir->directorio==NULL)
     { // en teoria esto no es posible
      zlDirClose(zlDir);
      return(NULL);
     }
while(1)
 {
   dp=readdir(zlDir->directorio);
   if (dp ==NULL)
     {
      zlDirClose(zlDir);
      return(NULL);
     }
/*
   if (dp->d_name[0]=='.')
     {
        printf("borra con punto <%s>\n",dp->d_name);
      continue;// Pasa de estos 
     }
*/
   tipoFile=zlDirTipoFichero1(zlDir->path,dp->d_name);
   if (tipoFile != tipoFileRegular)
       {
       continue;/* no son ficheros normales */
       }
   // atencion, match jamas es NULL, pues no es un puntero
   if ( strlen(zlDir->match) != 0 )
     {
      
      ret=fnmatch(zlDir->match,dp->d_name,0);
      if (ret!=0)
       {
        continue;/* ficheros que no match no nos sirven */
       }
     }
    sprintf(zlDir->nombreFichero,"%.*s",MIDIR_MAX_FILE_LEN,dp->d_name);
    return(zlDir->nombreFichero);

 }
}
int zlDirExisteRecurso(char *directorio,char *recurso,tipoFile_t tipo)
{
int ret;
tipoFile_t tipoFile;
DIR *dir;
char pathCompleto[300];
struct dirent *entry;
dir = opendir( directorio );
if (!dir)
    return(0);
while(1)
 {
   entry = readdir( dir );
   if (entry==NULL) break;
   if (strcmp(recurso,entry->d_name)!=0) continue;
   if (strlen(directorio)==0 || directorio[strlen(directorio)-1]=='/')
      sprintf(pathCompleto,"%s%s",directorio,entry->d_name);
   else
      sprintf(pathCompleto,"%s/%s",directorio,entry->d_name);
   tipoFile= zlDirTipoFichero(pathCompleto);
   if (tipoFile>0 && tipo==tipoFile)
      {
       closedir(dir);
       return(1);
      }
}
closedir( dir );
return(0);


}


