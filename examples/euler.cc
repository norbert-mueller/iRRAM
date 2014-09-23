#include "iRRAM.h"

// Euler's constant gamma=0.5772...,
// Approximation due to Brent and McMillan, 1980
//

using namespace iRRAM;


REAL gamma_approx(int p)
{
  if (p > 0 )return REAL(0.57721566490);

  int n=(2-p)/11;
  int k=0;
  int beta_n;
  REAL k2;
  REAL n2;
  
  beta_n=5*n;
  REAL s0=0;
  REAL i0=1;
  REAL b=1;
  REAL a=0;

  n2=REAL(n)*n;
  while ( k < beta_n  ) {
     k=k+1;
     k2=REAL(k)*k;
     b=b*n2/k2;
     a=(a*n2+b*k)/k2;
// instead of
//    a=(a*n2/k+b)/k;   
// as in the original paper
// saving about 7% of time
     i0+=b;
     s0+=a;
  }

  k=0;
  REAL k0=1;
  REAL y=1;
  while ( k < 4*n ) {
     k=k+1;
     INTEGER nom=2*k;
     nom*=(2*k-1);
     nom*=(2*k);
     nom*=(2*k-1);
     INTEGER denom=k;
     denom*=k;
     denom*=k;
     denom*=64;
     denom*=n;
     y= -y * REAL(nom) / REAL(denom);
     k0+=y;
  }
  k0=sqrt(pi()/REAL(4*n))*exp(REAL(-2*n))*k0;

  return (s0-k0)/i0-log(REAL(n));
}

REAL gamma()
{
  return limit(gamma_approx);
}


//---------------------------------------------------------------------
// Compute eulers number gamma= 0.577...,
// then ask for a number of desired decimals, 
// then print gamma with the necessary precision:

void compute(){


  int deci_places;
  cout << "Desired Decimals: ";
  cin  >> deci_places;

  do {
    cout << "\n gamma=";
    cout << setRwidth(deci_places+8)<<gamma();
    cout << "\n\nAnother try? ";
    cin  >> deci_places;
  } while ( deci_places > 0 );
}
