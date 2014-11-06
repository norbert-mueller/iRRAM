#include "iRRAM/lib.h"
#include "iRRAM/core.h"

using namespace iRRAM;

template int iRRAM::iRRAM_exec <int,int,char **> (int (*) (const int &, char **const &),const int &, char **const &);

int iRRAM_compute(const int &argc, char **const &argv) {

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
