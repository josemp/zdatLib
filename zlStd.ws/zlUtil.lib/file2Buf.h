
/* Ejemplo de uso 
main()
{
char *buf;
buf=zluFile2Buf("pedido.txt");
if (buf==NULL)
{printf("no se pudo cargar\n");return;}
printf("<%s>",buf);
free(buf);
printf("terminado\n");



}
*/

char *zluFile2Buf(char *fichero);

