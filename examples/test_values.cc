#include "iRRAM.h"

using namespace iRRAM;

void compute(){
/* Test-Routines for LAZY BOOLEANS */

/* First tests for positive(x,k) */
 if  (positive(REAL(1),-2))
   cout << "Positive(1,-2)=true, OK\n";
 else
   cout << "Positive(-1,-2)=false, ERROR!\n"; 

 if  (positive(REAL(-1),-2))
   cout << "Positive(-1,-2)=true, ERROR!\n";
 else 
   cout << "Positive(-1,-2)=false, OK!\n"; 
 
 if  (positive(REAL(0),-2))
   cout << "Positive(0,-2) = true, OK!\n"; 
 else
   cout << "Positive(0,-2) = false, OK!\n";
 
 if  (positive(REAL("1e-100"),-2))
   cout << "Positive(1e-100,-2) = true, OK!\n"; 
 else
   cout << "Positive(1e-100,-2) = false, OK!\n";
 
 if  (positive(REAL("-1e-100"),-2))
   cout << "Positive(-1e-100,-2) = true, OK!\n"; 
 else
   cout << "Positive(-1e-100,-2) = false, OK!\n";
 
 if  (positive(REAL("-1e-100"),-1000)) 
   cout << "Positive(-1e-100,-1000) = true, ERROR!\n"; 
 else 
   cout << "Positive(-1e-100,-1000) = false, OK!\n";
 
 
/* Tests for choose(b1,b2,...) */
 REAL x1,x2,x3,x4;
 x1="1e-10000";
 x2="-1e-200";
 x3="-1e-100";
 x4="-1e-2000";
 switch (choose(x1>REAL(0),x2>REAL(0),x3>REAL(0),x4>REAL(0))) {
     case 0:;
       cout << "all values negative\n";
     break;
     case 1:;
       cout << "x1 positive\n";
     break;
     case 2:;
       cout << "x2 positive\n";
     break;
     case 3:;
       cout << "x3 positive\n";
     break;
     case 4:;
       cout << "x4 positive\n";
     break;
  }

};
