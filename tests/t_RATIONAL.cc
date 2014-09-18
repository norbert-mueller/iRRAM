/*
 test_INTEGER.cc
 
 Small test routine to check basic properties of the RATIONAL data type.
 

*/
#include "iRRAM.h"

using namespace iRRAM;

void error(int i){
cout << "RATIONAL test: Error "<< i <<"\n";
exit(1);
}

void compute(){
 RATIONAL x= RATIONAL(3,5);
 RATIONAL y= RATIONAL(-7,3);
 RATIONAL z;
 INTEGER  u=-3;
 RATIONAL v= RATIONAL(u,INTEGER(-7));
 RATIONAL w= RATIONAL(INTEGER(5),u);

 if ( RATIONAL(16,3) != RATIONAL(-160,-30) )  error(1); 
 if ( RATIONAL(-16,3) != RATIONAL(160,-30) )  error(2); 
 if ( RATIONAL(INTEGER(16),INTEGER(-3)) != RATIONAL(160,-30) )  error(3); 
 if ( RATIONAL(INTEGER(3)) != -RATIONAL(-3) )  error(4); 
 
 if ( ! (x>0)   )  error(5); 
 if ( (x<0)     )  error(6); 
 if ( (x<=0)    )  error(7); 
 if ( (x>=1)    )  error(8); 
 if ( (x == 0)  )  error(9); 
 if ( !(x != 0) )  error(10); 

 if ( y> -1	)  error(11); 
 if ( y< -3	)  error(12); 
 if ( y<= -3	)  error(13); 
 if ( y>= -2	)  error(14); 
 if ( y == 0	)  error(15); 
 if ( !(y != 0) )  error(16); 

 if ( (z>0)	)  error(17); 
 if ( (z<0)	)  error(18); 
 if ( (z != 0)	)  error(19); 
 if ( !(z == 0)	)  error(20); 
 if ( !(z >= 0)	)  error(21); 
 if ( !(z >= 0)	)  error(22); 

 if ( x*y != RATIONAL(-7,5)	)  error(23); 
 if ( x+y != RATIONAL(-26,15)	)  error(24); 
 if ( x-y != RATIONAL(44,15)	)  error(25); 
 if ( x/x != 1 )   error(26); 
 
 if ( x*(-5) != -3)  error(27); 
 if ( x+(-1) != RATIONAL(2,-5)	)  error(28); 
 if ( x-(-2) != -RATIONAL(-13,5))  error(29); 
 if ( 3-y    != RATIONAL(-160,-30) )  error(30); 
 if ( (-3)-y != RATIONAL(2,-3)  )  error(31); 

 if ( x/3    !=  RATIONAL(1,5))   error(32); 
 if ( x*w    !=  -1)   error(33); 
 if ( x/(-3) != RATIONAL(-1,5))   error(34); 
 if ( 3/x    !=  5 )  error(35); 
 if ( (-3)/x != -5 )  error(36); 

 if ( abs(y*v) !=  1 )  error(37);
 if ( abs(x*(-w)) !=  1 )  error(38);

 RATIONAL a(2147483647,-1);
 RATIONAL b(-2147483647,-1);
 if (a+b != 0)  error(39);

 RATIONAL c(2147483647,-1);
 RATIONAL d(-2147483647,-1);
 if (c+d != 0)  error(40);
 
 
  z=x; z*=y; if (z != RATIONAL(-7,5))    error(41);
  z=x; z+=y; if (z != RATIONAL(-26,15))  error(42);
  z=x; z-=y; if (z != RATIONAL(44,15))   error(43);

  z=x; z*=(-7); if (z != RATIONAL(-21,5))    error(44); 
  z=x; z+=(-7); if (z != RATIONAL(-32,5) )  error(45); 
  z=x; z-=(-7); if (z != RATIONAL(38,5) )   error(46); 
 
  z=y; z/=x;  if (z != RATIONAL(-35,9))  error(47);
  z=y; z/=-3; if (z != RATIONAL(7,9))  error(48);

 cout << "test_RATIONAL:      passed\n";
 exit(0);
};
