#include "iRRAM.h"

using namespace iRRAM;

REAL test_function(int p, const REAL& x){
  DYADIC_precision block1(p);
  DYADIC d=pi().as_DYADIC();
  cerr << "   prec1a" << DYADIC::getprec() <<"\n";
  {
  DYADIC_precision block2(DYADIC::getprec()-100);
  cerr << "   prec2" << DYADIC::getprec() <<"\n";
  }
  cerr << "   prec1b" << DYADIC::getprec() <<"\n";
  return d;
}

void compute(){
  cout << setRwidth(100);
  cerr << "   prec0a" << DYADIC::getprec() <<"\n";
  REAL x(1.0);
  x=limit(test_function,x);
  cout << x << "\n";
  cerr << "   prec0b" << DYADIC::getprec() <<"\n";
  
  DYADIC d(x.as_DYADIC());
  d=d+0;
  cout << d << "\n";
  cout << x-REAL(d)<<"\n";
  DYADIC_precision block1(-1000);
  d=pi().as_DYADIC();
  d=d+0;
  cout << setRflags(iRRAM_float_relative);
  cout << x-REAL(d)<<"\n";
}
