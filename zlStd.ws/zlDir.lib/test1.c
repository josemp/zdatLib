#include <stdio.h>
#include <dirent.h>
#include <string.h>
#include <stdlib.h>
#include <fnmatch.h>

#include "zlDir.h"




int main()
{

 zlDir_t *zlDir=zlDirFor("./","*.c");

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
