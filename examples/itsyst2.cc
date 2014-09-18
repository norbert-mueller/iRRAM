#include "iRRAM.h"

using namespace iRRAM;

/* Compute iterated system x=3.75*x*(1-x) (Kulisch) */

void compute(){
  int count;
  cout << "\nIterated functions system: x = 3.75*x*(1-x)\n";
  cout << "How many values: ";
  cin  >> count;

  REAL   xr= 0.5,   cr=3.75;
  RATIONAL xq= 0.5, cq=3.75;
  double xd= 0.5, cd=3.75;
  for (long i=1;i<=count;i++ ) {
    cout << xr << " " << xd << " " <<REAL(xq) << " " << i<< "\n";
    xr=cr*xr*(1-xr);
    xd=cd*xd*(1-xd);
    xq=cq*xq*(1-xq);
  }
};
