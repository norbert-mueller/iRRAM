/*
 test_DYADIC.cc
 
 Small test routine to check basic properties of the DYADIC data type.
 

*/
#include "iRRAM.h"

using namespace iRRAM;

void error(int i){
cerr << i <<"\n";
exit(1);
}

void compute(){
 DYADIC x=3;
 DYADIC y=-7;
 DYADIC z;
 
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

 if ( abs(y) !=  7 )  error(20);
 if ( abs(x) !=  3 )  error(21);

cout << "test_DYADIC:        passed\n";
};
