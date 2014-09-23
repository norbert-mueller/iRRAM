#include "iRRAM.h"

using namespace iRRAM;

inline void chk(double &d1){
  REAL x;
  double d2;
  x=d1; d2=x.as_double(); 
  if  ( d1 != d2){
     cout << std::setprecision(25) << d1 << " "<< x << " " << d2 <<" ERROR! \n"; 
     exit(1);
  }
}

void compute(){
/* Test-Routines for conversion between REAL and double */

REAL x;
double d1;

 d1=1.0;
 for (int i=1; i<647; i++) {d1=d1*3;chk(d1);}
 for (int i=1; i<750; i++){d1=d1/7;chk(d1);}
 d1=-1.0;
 for (int i=1; i<647; i++) {d1=d1*3;chk(d1);}
 for (int i=1; i<750; i++){d1=d1/7;chk(d1);}
 d1=1.0;
 for (int i=1; i<1024; i++) {d1=d1*2;chk(d1);}
 d1=1.0;
 for (int i=1; i<1140; i++){d1=d1/2;chk(d1);}
 d1=-1.0;
 for (int i=1; i<1024; i++) {d1=d1*2;chk(d1);}
 d1=-1.0;
 for (int i=1; i<1140; i++){d1=d1/2;chk(d1);}

 cout << "test_double_conv:   passed\n";
}
