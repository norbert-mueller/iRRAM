#include "iRRAM.h"

using namespace iRRAM;


//---------------------------------------------------------------------
// Compute an approximation to e=2.71.. up to an error of 2^p
 REAL e_approx (int p)
{
  if ( p >= 2 ) return 0;

  REAL y=1,z=2;
  int i=2;
  while ( !bound(y,p-1) ) {
    y=y/i;
    z=z+y;
    i+=1;
  }
  return z;
};

//---------------------------------------------------------------------
// Compute the exact value of  e=2.71.. 
REAL e()
{
  return limit(e_approx);
};


//---------------------------------------------------------------------
// Compute e=2.71...,
// then ask for a number of desired decimals, 
// then print e with the necessary precision:

void compute(){

  REAL euler_number=e();

  int deci_places;
  
  cout << "Desired Decimals: ";
  cin  >> deci_places;

  do {
    cout << setRwidth(deci_places+8) << euler_number; 

    cout << "\n\nAnother try? ";
    cin  >> deci_places;
 
  } while ( deci_places > 0 );
};


// example input/output of "echo 65|e_example":
// Desired Decimals: 65
//+.27182818284590452353602874713526624977572470936999595749669676277E+0001
