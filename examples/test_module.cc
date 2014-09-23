#include "iRRAM.h"

using namespace iRRAM;

void compute(){
long p,m;


/* Test-Routines for the module of functions*/
REAL x=sqrt(REAL(2));


DYADIC z=approx(pi(),-100);
z=approx(log(REAL(1E-100)),-10);

p=-10; m=module(log,REAL(0.000001),p);
cout << "module of log at 0.000001 for precision "<<p<<" is "<< m <<"\n";


p=-100; m=module(sqrt,REAL(0.001),p);
cout << "module of sqrt at 0.001 for precision "<<p<<" is "<< m <<"\n";

p=-100; m=module(sqrt,REAL(0.0000001),p);
cout << "module of sqrt at 0.0000001 for precision "<<p<<" is "<< m <<"\n";

p=-100; m=module(sqrt,REAL(100000000),p);
cout << "module of sqrt at 100000000 for precision "<<p<<" is "<< m <<"\n";

p=-100; m=module(sin,REAL(100000000),p);
cout << "module of sin at 100000000 for precision "<<p<<" is "<< m <<"\n";

p=-100; m=module(sin,REAL(0.0000001),p);
cout << "module of sin at 0.0000001 for precision "<<p<<" is "<< m <<"\n";

p=10; m=module(sin,REAL(0.0000001),p);
cout << "module of sin at 0.0000001 for precision "<<p<<" is "<< m <<"\n";


p=10; m=module(exp,REAL(0.0000001),p);
cout << "module of exp at 0.0000001 for precision "<<p<<" is "<< m <<"\n";
}
