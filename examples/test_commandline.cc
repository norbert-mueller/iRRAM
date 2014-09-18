#include "iRRAM/lib.h"
#include "iRRAM/core.h"

using namespace iRRAM;

template int iRRAM_exec <int, char **> (int (*) (int, char **),int, char **);

int iRRAM_compute(int argc, char **argv) {

REAL x1,x2;
if (argc >=2 ) x1=argv[1];
if (argc >=3 ) x2=argv[2];
cout <<x1<<"\n"<<x2<<"\n";

return 0;
}

int main (int argc,char **argv)
{
iRRAM_initialize(argc,argv);
return iRRAM_exec(iRRAM_compute,argc,argv);
}
