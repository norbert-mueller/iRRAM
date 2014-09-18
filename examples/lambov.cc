#include "iRRAM.h"
#include <math.h>

using namespace iRRAM;

/* 
  analogue to the example by Branimir Lambov presented at the CCA2003,
  trivial, but the addition of rwritee was necessary... 
*/
void compute (){

REAL e=exp(REAL(1));
REAL s=0,m=1;

for ( int i=1;i<=1000;i++){
  s += m;
  m = m/i;
}

REAL diff = e-s;

cout << setRwidth(55);

cout << e<<"\n";

cout << s<<"\n";

cout << diff<<"\n";

}


