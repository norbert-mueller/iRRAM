/*
 test_INTEGER.cc
 
 Small test routine to check basic properties of the INTEGER data type.
 

*/
#include "iRRAM.h"

using namespace iRRAM;

void error(int i){
cout << "INTEGER test: Error "<< i <<"\n";
exit(1);
}

void compute(){
 INTEGER x=3;
 INTEGER y=-7;
 INTEGER z;
 
 if ( ! (x>0)   )  error(1); 
 if ( (x<0)     )  error(2); 
 if ( (x == 0)  )  error(3); 
 if ( !(x != 0) )  error(4); 

 if ( (y>0)     )  error(5); 
 if ( !(y<0)    )  error(6); 
 if ( (y == 0)  )  error(7); 
 if ( !(y != 0) )  error(8); 

 if ( (z>0)   )    error(9); 
 if ( (z<0)     )  error(10); 
 if ( (z != 0)  )  error(11); 
 if ( !(z == 0) )  error(12); 

 if ( x*y != -21)  error(13); 
 if ( x+y != -4 )  error(14); 
 if ( x-y != 10 )  error(15); 
 
 if ( x*(-7) != -21)  error(16); 
 if ( x+(-7) != -4 )  error(17); 
 if ( x-(-7) != 10 )  error(18); 
 if ( 3-y    != 10 )  error(19); 
 if ( (-3)-y !=  4 )  error(20); 
 
 if ( abs(y) !=  7 )  error(21);
 if ( abs(x) !=  3 )  error(22);

 if ( size(INTEGER(0)) != 0 )  error(23);
 if ( size(INTEGER(1)) != 1 )  error(23);
 if ( size(INTEGER(2)) != 2 )  error(23);
 if ( size(INTEGER(3)) != 2 )  error(23);
 if ( size(INTEGER(4)) != 3 )  error(23);
 if ( size(INTEGER(1048576)) != 21 ) error(24);
 { INTEGER m=INTEGER(1048576);
   m=m*m;
   m=m*m;
   m=m*m;   
   if ( size(m) != 161 ) error(25);
   if ( m <= 0 ) error(26);
   if ( -m > 0 ) error(27);
 }
 if ( size(y) != 3 )  error(28);
 if ( size(z) != 0 )  error(29);
 

 { REAL x=pi();
   if ( (pi()).as_INTEGER()      !=   3 ) error (30);
   if ( (-pi()).as_INTEGER()     !=  -3 ) error (31);
   if ( (euler()).as_INTEGER()   !=   3 ) error (32);
   if ( (-euler()).as_INTEGER()  !=  -3 ) error (33);
   if ( (REAL( 99)).as_INTEGER() !=  99 ) error (34);
   if ( (REAL(-99)).as_INTEGER() != -99 ) error (35);
 }

  
  z=x; z*=y; if (z != -21)  error(36);
  z=x; z+=y; if (z != -4)   error(37);
  z=x; z-=y; if (z != 10)   error(38);

  z=x; z*=(-7); if (z != -21)  error(39); 
  z=x; z+=(-7); if (z != -4 )  error(40); 
  z=x; z-=(-7); if (z != 10 )  error(41); 
 
  z=y; z/=x;  if (z != -2)  error(42);
  z=y; z/=-3; if (z != 2)  error(43);

 
 cout << "test_INTEGER:       passed\n";
 exit(0);
}
