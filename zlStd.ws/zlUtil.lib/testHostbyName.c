

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <memory.h>
#include <zlHostbyName.h>
    int main(int argc, char **argv)
    {
     char ip[200];
     char host[200];
     if (argc<2)
       {printf ("using:\nprogram host\n");return(0);}
     strcpy(host,argv[1]);printf("<%s><%s>\n",host,zlHostbyName(ip,host));
     return(0);
    }



