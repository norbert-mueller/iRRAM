#include "iRRAM.h"

using namespace iRRAM;

using std::setw;

/* Compute iterated system x=3.75*x*(1-x) (Kulisch) */


void itsyst_double(int count,int width){
  double x = 0.5;
  double c = 3.75;

  for ( int i=0; i<=count; i++ ) {
    if ( (i<100) || (i%10)==0 ) {
      cout << setw(width) << x  << " : " << setw(4) << i << "\n";
    }
   x= c*x*(1-x);
  }
}


void itsyst_RATIONAL(int count,int width){

  RATIONAL x = RATIONAL(1)/RATIONAL(2);
  RATIONAL c = RATIONAL(375)/RATIONAL(100);

  for ( int i=0; i<=count; i++ ) {
    if ( (i<100) || (i%10)==0 ) {
      cout << setRwidth(width) << x  << " : " << setw(4) << i << "\n";
    }
    x= c*x*(RATIONAL(1)-x);
  }
}



void itsyst_REAL(int count,int width){

  REAL x = 0.5;
  REAL c = 3.75;

// 0.5 and 3.75 can be represented exactly as double, so 
// c=3.75 and c="3.75" are the same, but the latter is a lot slower!

  for ( int i=0; i<=count; i++ ) {
    if ( (i<100) || (i%10)==0 ) {
      cout << setRwidth(width) << x  << " : " << setw(4) << i << "\n";
    }
    x= c*x*(REAL(1)-x);
  }
}



// Using additional information like the Lipschitz condition
// for the iteration improves on the error propagation.
// Although we have to evaluate the Lipschitz condition,
// the improvements due to the better error propagation 
// are significant (speedup about 70%).

REAL iteration(const REAL& x) {return  3.75*x*(1-x);}
REAL iteration_lip(const REAL& x) {return  abs(3.75 - 7.5*x);}

//LAZY_BOOLEAN iteration_dom(const REAL& x) {return (x>REAL(0.1) && x <REAL(1));}
bool iteration_dom(const REAL& x) {return true;}

void itsyst_REAL_with_lipschitz_condition(int count,int width){

  REAL x = 0.5;

  for ( int i=0; i<=count; i++ ) {
    if ( (i<100) || (i%10)==0 ) {
      cout << setRwidth(width) << x  << " : " << setw(4) << i << "\n";
    }
    x= lipschitz(iteration,iteration_lip,x);
  }
}


void itsyst_REAL_with_lipschitz_condition_and_domain(int count,int width){

  REAL x = 0.5;

  for ( int i=0; i<=count; i++ ) {
    if ( (i<100) || (i%10)==0 ) {
      cout << setRwidth(width) << x  << " : " << setw(4) << i << "\n";
    }
    x= lipschitz(iteration,iteration_lip,iteration_dom,x);
  }
}

void itsyst_with_INTEGER(int count,int width){


  REAL x = 0.5;

  for ( int i=0; i<=count; i++ ) {
    if ( (i<100) || (i%10)==0 ) {
      cout << setRwidth(width) << x  << " : " << setw(4) << i << "\n";
    }
    x= (x*(1-x)*375)/100;
  }
}


void compute(){
  int test,count,width;
  cout << "\nIterated functions system: x = 3.75*x*(1-x)\n";
  cout << "\nHow to compute (1=double, 2=RATIONAL, 3=REAL, 4=with_INTEGER, 5=with_Lipschitz, 6=with_L+D) : ";
  cin  >> test;
  cout << "How many values: ";
  cin  >> count;
  cout << "How many decimals: ";
  cin  >> width;
  cout << "\n";
  switch (test) {
     case 1:;
       itsyst_double(count,width);
     break;
     case 2:;
       itsyst_RATIONAL(count,width);
     break;
     case 3:;
       itsyst_REAL(count,width);
     break;
     case 4:;
       itsyst_with_INTEGER(count,width);
     break;
     case 5:;
       itsyst_REAL_with_lipschitz_condition(count,width);
     break;
     case 6:;
       itsyst_REAL_with_lipschitz_condition_and_domain(count,width);
     break;
  }
  cout << "\n";
}
