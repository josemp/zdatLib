#include <stdio.h>
#include <stdlib.h>
#include <zlStdComun.h>
#include <string.h>
#include "zlValidIpv4.h"





int main()
{
char ip[200];
strcpy(ip,"127.0.0.1");printf("is valid <%s> <%d>\n",ip,isValidIp4(ip));
return(0);
}

