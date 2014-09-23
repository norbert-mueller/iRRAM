#include "iRRAM.h"
// Algebraic numbers on top of the iRRAM using BMFS bounds
//
// This implementation is non-optimal in many ways!
// It's just a proof of concept!

using namespace iRRAM;


class ALGEBRAIC {public:
ALGEBRAIC();
ALGEBRAIC(const int& i);

ALGEBRAIC& operator = (const ALGEBRAIC& y);

friend ALGEBRAIC operator + (const ALGEBRAIC& x, const ALGEBRAIC& y);
friend ALGEBRAIC operator - (const ALGEBRAIC& x, const ALGEBRAIC& y);
friend ALGEBRAIC operator * (const ALGEBRAIC& x, const ALGEBRAIC& y);
friend ALGEBRAIC operator / (const ALGEBRAIC& x, const ALGEBRAIC& y);
friend ALGEBRAIC sqrt (const ALGEBRAIC& x);
//ALGEBRAIC root (const ALGEBRAIC& x,unsigned int k);

friend bool   zerotest (const ALGEBRAIC& x);

REAL val;
unsigned long long deg;
REAL c_u;
REAL c_l;
};

ALGEBRAIC::ALGEBRAIC(){
  val = 0; 
  deg = 1; 
  c_u = 0; 
  c_l = 1;
}

ALGEBRAIC::ALGEBRAIC(const int& i){
  val = i;      
  deg = 1; 
  c_u = abs(i); 
  c_l = 1;
}

ALGEBRAIC& ALGEBRAIC::operator = (const ALGEBRAIC& x){
  val = x.val; 
  deg = x.deg; 
  c_u = x.c_u; 
  c_l = x.c_l;
  return (*this);
}

ALGEBRAIC operator + (const ALGEBRAIC& x, const ALGEBRAIC& y){
  ALGEBRAIC z;
  z.val = x.val + y.val;                 
  z.deg = x.deg * y.deg;
  if (z.deg > 1000000000) z.deg= 0;
	if (z.deg == 0 ){  z.c_u=1; z.c_l=1; } else {
	z.c_u = x.c_u * y.c_l + x.c_l * y.c_u; 
	z.c_l = x.c_l * y.c_l;
	if (size(z.c_u) > 500000000 || size (z.c_l) > 500000000){
		z.deg=0; z.c_u=1;z.c_l=1;
		}
	}
  return z;
}

ALGEBRAIC operator - (const ALGEBRAIC& x, const ALGEBRAIC& y){
  ALGEBRAIC z;
  z.val = x.val - y.val;
  z.deg = x.deg * y.deg;
  if (z.deg > 1000000000) z.deg= 0;
  if (z.deg == 0 ){  z.c_u=1; z.c_l=1; } else {
	z.c_u = x.c_u * y.c_l + x.c_l * y.c_u; 
	z.c_l = x.c_l * y.c_l;
	if (size(z.c_u) > 500000000 || size (z.c_l) > 500000000){
		z.deg=0; z.c_u=1;z.c_l=1;
		}
	} 

  return z;
}

ALGEBRAIC operator * (const ALGEBRAIC& x, const ALGEBRAIC& y){
  ALGEBRAIC z;
  z.val = x.val * y.val;
  z.deg = x.deg * y.deg;
  if (z.deg > 1000000000) z.deg= 0;
  if (z.deg == 0 ){  z.c_u=1; z.c_l=1; } else {
	z.c_u = x.c_u * y.c_u; 
	z.c_l = x.c_l * y.c_l;
	if (size(z.c_u) > 500000000 || size (z.c_l) > 500000000){
		z.deg=0; z.c_u=1;z.c_l=1;
		}
	} 
  return z;
}

ALGEBRAIC operator / (const ALGEBRAIC& x, const ALGEBRAIC& y){
ALGEBRAIC z;
  z.val = x.val / y.val;
  z.deg = x.deg * y.deg;
  if (z.deg > 1000000000) z.deg= 0;
  if (z.deg == 0 ){  z.c_u=1; z.c_l=1; } else {
	z.c_u = x.c_u * y.c_l; 
	if (size(z.c_u) > 500000000 || size (z.c_l) > 500000000){
		z.deg=0; z.c_u=1;z.c_l=1;
		}
	} 
  return z;
}

ALGEBRAIC sqrt (const ALGEBRAIC& x){
  ALGEBRAIC z;
  z.val = sqrt(x.val);
  z.deg = 2*x.deg;
  if (z.deg > 1000000000) z.deg= 0;
  if (z.deg == 0 ){  z.c_u=1; z.c_l=1; } else {
	z.c_u = sqrt(x.c_u);
	z.c_l = sqrt(x.c_l);
	if (size(z.c_u) > 500000000 || size (z.c_l) > 500000000){
		z.deg=0; z.c_u=1;z.c_l=1;
		}
	} 
  return z;
}

bool  zerotest (const ALGEBRAIC& x){
  if (x.deg == 0) return abs(x.val) == 0 ;
  switch ( choose (abs(x.val) > 0 , true ) ) {
  case 1: 
	break;
  case 2:
	REAL zerobound = 1 / power( x.c_u, x.deg*x.deg-1 ) / x.c_l;
	return choose ( abs(x.val) > 0 , abs(x.val) < zerobound )  -  1 ;
  }
  return false;
}

void compute (){
  int count,width;
  cout << "\nExample:\n\talgebraic-BFMS.cc\n\n";
  cout << "Purpose:\n\tCompute values from the iterated functions system\n\n";
  cout << "\t\tx_{i+1} = 3.75*x_i*(1-x_i)\n\n";
  cout << "\tstarting with x_0=0.5\n\n";
  cout << "\tThe computation is done with algebraic numbers based on BFMS bounds\n\n";
  cout << "Status:\n\tUnfinished! Implementation of algebraic numbers is only provisional!\n\n";
  cout << "Parameters:\n\tnone\n\n";
  cout << "Input:\n\t(1) Number of values to be computed, typical range: [5..50]\n";
  cout << "\t(2) width of the output, typical range [10..100]\n\n";
  cout << "Output:\n\tValues of the iteration, together with some info over the resulting BFMS bounds\n\n";
  cout << "Typical run:\n\techo 10 20 | ./algebraic-BFMS \n\n";
  cout << "How many values: ";
  cin  >> count;
  cout << "How many decimals: ";
  cin  >> width;
  cout << "\n";
    
  ALGEBRAIC c(375); c= c/100;
  ALGEBRAIC x(1);   x= sqrt(x/4);
//  ALGEBRAIC x(1);   x= x/2;

  for ( int i=0; i<=count; i++ ) {
    if ( (i<100) || (i%10)==0 ) {
      cout << i<< ": "<< setRwidth(width) << x.val;
      if ( x.deg > 0 ){
	cout << ", bounds="  << x.deg<<":"<< upperbound(x.c_u) << ":"  << upperbound(x.c_l);
      } else {
	cout << ", bounds disabled";
      } 
      if ( zerotest (x) )   cout << ", x_i == 0"; 
      else   cout << ", x_i != 0";

      if ( zerotest ( x - 1/ALGEBRAIC(2) ) )   cout << ", x_i == 0.5"; 
      else   cout << ", x_i != 0.5";

//      if ( zerotest (x-x) ) cout << ", x_i == x_i"; 
//      else   cout << ", x_i != x_i"; 

      cout << "\n";

    }

    x=c*x*(1-x);
  }
}
