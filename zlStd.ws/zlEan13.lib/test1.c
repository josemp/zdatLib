#include <stdio.h>
#include <stdlib.h>
#include <zlStdComun.h>
#include "zlEan13.h"


int main()
{
printf("847000678656%c\n",(char) zlEan13DigitodeArticulo("678656"));
return(0);
}
