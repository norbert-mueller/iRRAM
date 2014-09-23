#include "iRRAM.h"

using namespace iRRAM;

void compute(){

try { 
  INTERVAL x(REAL(-1),REAL(1));
  INTERVAL y=REAL(1)/x;
}
catch (const iRRAM_Numerical_Exception exc)
{
   if ( exc.type == iRRAM_interval_divide_by_zero )
   fprintf(stderr,"found %s...\n",iRRAM_error_msg[exc.type]);
//   throw iRRAM_Numerical_Exception(exc);
}

try {
  REAL x;
  double d=1.0;
  while (true) {x=d;d=2*d;}
}
catch (const iRRAM_Numerical_Exception exc)
{
   if ( exc.type == iRRAM_conversion_from_infinite )
   fprintf(stderr,"found %s...\n",iRRAM_error_msg[exc.type]);
//   throw iRRAM_Numerical_Exception(exc);
}

}
