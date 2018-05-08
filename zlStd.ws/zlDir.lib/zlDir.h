/* RUTINAS DE DIRECTORIOS */
/*
Dependencias
#include <dirent.h>
#include <fnmatch.h>

---------------------- FORMA DE USO ------------------------
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


*/







#define MIDIR_MAX_FILE_LEN 400
#define MIDIR_MAX_FILE_PATH 400

typedef enum {tipoFileNoValido = -1,tipoFileRaro = 0,tipoFileRegular=1,tipoFileCarpeta=2} tipoFile_t;

typedef struct
{
DIR *directorio;
char path[MIDIR_MAX_FILE_PATH+1];
char nombreFichero[MIDIR_MAX_FILE_LEN+1];
char match[100+1];
}zlDir_t;


/**

  @brief  zlDirTipoFichero, Nos indica si es un path valido de fichero, 
	  directorio, u otro tipo

  @param pahCompleto: Un string con el path relativo o completo

  @returns 
           	-1 -> No valido
		 0 -> valido pero ni regular ni carpeta
		 1 -> fichero regular
		 2 -> carpeta
  Notas :
	S_ISBLK(mode) -- Is this a block special file? (usually a block-based device of some sort)
	S_ISCHR(mode) -- Is this a character special file? (again, usually a character-based device of some sort)
	S_ISDIR(mode) -- Is this a directory?
	S_ISFIFO(mode) -- Is this a pipe or FIFO special file?
	S_ISLNK(mode) -- Is this a symbolic link?
	S_ISREG(mode) -- Is this a regular file?
*/

tipoFile_t zlDirTipoFichero(char *pathCompleto);

/**
  @brief zlDirTipoFichero1, Nos indica si es un path valido de fichero, 
	 directorio, u otro tipo

  @param path: Un string con el path relativo o completo a una carpeta
  @param file: Un string con el nombre del fichero

  @returns 
           	-1 -> No valido
		 0 -> valido pero ni regular ni carpeta
		 1 -> fichero regular
		 2 -> carpeta
Nota: igual que zlDirTipoFichero, pero con dos parametros
*/
tipoFile_t zlDirTipoFichero1(char *path,char *file);

/**
  @brief zlDirFor Usa la estructura zlDir_t para facilitar la lectura 
	 de los datos de un directorio

  @param pathCarpeta: Un string con el path relativo o completo a una carpeta
  @param match: Un string con el mascara tipo. ejemplo: "*.c"

  @returns  estructura manejadora. Si es NULL, no hay datos que leer

  Nota: Hay que llamar a zlDirNext para acceder a los datos
*/

zlDir_t *zlDirFor(char *pathCarpeta,char *match);

/**
  @brief zlDirClose libera estructura zlDir_t 

  @param zlDir: punter a la estructura zlDir_t que hay que liberar

  @returns  nada

  Nota: zlDirNext ya libera la estructura zlDir_t cuando termina 
	la lectura de ficheros
	
	Se usa cuando se quiere interrumpir el bucle de lectura
*/

void zlDirClose(zlDir_t *zlDir);

/**
  @brief Siguente nombre de fichero encontrado en el directorio
  @param zlDir : La estructura manejadora
  @returns el nombre del fichero , NULL si se terminan los datos
 
  nota: libera la estructura zlDir cuando se terminan los datos
*/
char *zlDirNext(zlDir_t *zlDir);

/**
  @brief Mira la existencia de un recurso (fichero) en un directorio
  @param directorio : string al directorio
  @param recurso : string al directorio
  @param tipo : el tipo de fichero
  @returns 1 si existe el recurso, 0 si no existe
 
*/
int zlDirExisteRecurso(char *directorio,char *recurso,tipoFile_t tipo);

