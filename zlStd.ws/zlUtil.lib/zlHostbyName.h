

/* -----------------------------------------------------------------
/**
  @brief Con el nombre de host  --> devuelve el IP.

  @param ip: input : es la ip que retorna la funcion, ha de estar suficientemente espaciada
  @param host : output : el nombre de host (tambien ip si queremos)
  @returns : retorna un puntero a ip o un NULL si es erroneo

   Si hay exito tenemos el IP en la variable y en el retorno.
   Retorna 0 si no existe IP (IP="").
  
   Si no damos el suficiente espacio a la variable ip, la cosa petará

   NOTA: Si es una ip, retorna la misma ip

   Using:
    int main()
    {
     char ip[200];
     char host[200];
     strcpy(host,"0far.com");printf("<%s><%s>\n",host,MskIpDeHost(ip,host));
    }

   ----------------------------------------------------------------- */
char *zlHostbyName(char *ip, char *host);


