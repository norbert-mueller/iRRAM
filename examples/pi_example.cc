#include "iRRAM/lib.h"

using namespace iRRAM;

REAL maxapprox (int prec, const REAL& x, const REAL& y){
   if ( positive(x-y,prec) ) return x;
   else return y;
};

REAL max (const REAL& x, const REAL& y){
   return limit(maxapprox,x,y);
};

int iRRAM_compute(const int& dummy) {

cout << "Small test program showing some features...\n";

REAL x1= 3.14159;
REAL x2="3.14159";
REAL x3=pi();

cout << setRwidth(30) << "\n\n"; 
cout << x1 << "\n";
cout << x2 << "\n";
cout << max(x3,x2) << "\n\n";

return 0;
}

int main (int argc,char **argv)
{
iRRAM_initialize(argc,argv);
return iRRAM_exec(iRRAM_compute,0);
}
