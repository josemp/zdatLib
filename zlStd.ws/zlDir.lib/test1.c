#include <stdio.h>
#include <dirent.h>
#include <string.h>
#include <stdlib.h>
#include <fnmatch.h>

#include "zlDir.h"




int main()
{

// No hacer match==NULL, porque no es un puntero dejarlo en 0 ,""
 zlDir_t *zlDir=zlDirFor("/home/concn/pedidosLaboratorio/laboratorios/lab1/Pendientes/","*.ped");

 if (zlDir==NULL)
  {printf("Error leyendo Directorio\n");return(0);}

 while(1)
  {
   char *nombre=zlDirNext(zlDir);

   if (nombre==NULL) break;
    printf("Esto es <%s>\n",nombre);
  }

 int ret= zlDirExisteRecurso("./","cosa",tipoFileCarpeta);

 printf("nos vamos<%d>\n",ret);

}
