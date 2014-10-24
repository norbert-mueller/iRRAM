#include "iRRAM/lib.h"  // basic includes
#include "iRRAM/core.h" // additional includes for templates

using namespace iRRAM;

// iRRAM-routine to compute a DYADIC cosine
DYADIC compute_cos(const DYADIC &x, const int &p) { return approx(cos(REAL(x)),p); }

template DYADIC iRRAM::iRRAM_exec <DYADIC,DYADIC,int>
(DYADIC (*) (const DYADIC &,const int &),const DYADIC &,const int &);



// main routine that internally calls iRRAM three times:
int main (int argc,char **argv)
{
iRRAM_initialize(argc,argv);
DYADIC d=2.0,d2;
int p=-10;

d2=iRRAM_exec(compute_cos,d,p);

cout << setRwidth(100);
cout << iRRAM_exec(compute_cos,d,p)<<"\n";

}


