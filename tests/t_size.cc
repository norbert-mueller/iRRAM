#include "iRRAM.h"
#include <cmath>

using namespace iRRAM;

int count_low,count_high;

inline void chk(int i,double &d1){
  REAL x;
  int d2;
  x=d1; d2=size(x); 
  if  ( d2 < i || d2 > i+1){
     cout << d1 << " "<< " " << i <<" ERROR! \n"; 
     exit(1);
  } else {
     if  ( d2 == i ) count_low +=1; 
     else count_high +=1;
  }
}

inline void rchk(int i,REAL x){
   int d;
   d=size(x); 
  if  ( d < i || d > i+1){
     cout << d << " "<< " " << i <<" ERROR! \n"; 
     exit(1);
  } else {
     if  ( d == i ) count_low +=1; 
     else count_high +=1;
  }
}

void compute(){
/* Test-Routines for computation of size(x) */
double d1;
int j;
count_low=0;
count_high=0;

 d1=1.0;
 for (int i=1; i<647; i++) {d1=d1*3;frexp(d1,&j),chk(j,d1);}
 for (int i=1; i<647; i++) {d1=d1/7;frexp(d1,&j),chk(j,d1);}
 d1=-1.0;
 for (int i=1; i<647; i++) {d1=d1*3;frexp(d1,&j),chk(j,d1);}
 for (int i=1; i<647; i++) {d1=d1/7;frexp(d1,&j),chk(j,d1);}
 d1=1.0;
 for (int i=1; i<1000; i++) {chk(i,d1);d1=d1*2;}
 d1=1.0;
 for (int i=1; i>-1000; i--){chk(i,d1);d1=d1/2;}
 d1=-1.0;
 for (int i=1; i<1000; i++) {chk(i,d1);d1=d1*2;}
 d1=-1.0;
 for (int i=1; i>-1000; i--){chk(i,d1);d1=d1/2;}

REAL x=1.0;
 for (int i=1; i<1000; i++) {
	rchk(i,x+1);x=x*2;	rchk(i,x-1);
	}


 cout << "test_size:          passed (" <<count_low<<" low / "<<count_high<<" high)\n";
};
