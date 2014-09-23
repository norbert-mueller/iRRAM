#include "iRRAM.h"

using namespace iRRAM;

void compute(){
REAL x;
INTEGER m;
int n;
for (int i=-220;i<220;i++){
  x=REAL(i)/100;
  m=x.as_INTEGER();
  n=int(m);
  cout << m <<" "<< n <<" "<< i <<"\n";
  }
for (int i=-220;i<220;i++){
  x=REAL(i)*10000000;
  m=x.as_INTEGER();
  n=int(m);
  cout << m <<" "<< n <<" "<< i <<"\n";
  }
}
