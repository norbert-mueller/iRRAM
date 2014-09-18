#include "iRRAM.h"

using namespace iRRAM;

void compute(){

printf("Start of small set of INTEGER tests.\n");

int x=1024;
int y=2047;

INTEGER a;
INTEGER b(x);
INTEGER c(y+0);
INTEGER d(c);

int n=d;
if (d != y ) printf("Error 1 in INTEGER part\n");
if (n != y ) printf("Error 2 in INTEGER part\n");


printf("End of small set of INTEGER tests.\n");

};
