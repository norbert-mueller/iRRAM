#include "iRRAM.h"

using namespace iRRAM;

/* Compute factorial using different data types */

void factorial_double(int count){
  double x = 1;

  for ( int i=1; i<=count; i++ ) {
    x= x*i;
    if ( (i<100) || (i%10)==0 ) {
      cout << x << " =  " << i << "!\n";
    }
  }
}


void factorial_REAL(int count){

  REAL x = 1;

  for ( int i=1; i<=count; i++ ) {
    x= x*i;
    if ( (i<100) || (i%10)==0 ) {
      cout << x << " =  " << i << "!\n";
    }
  }
}


void factorial_REAL_int(int count){

  REAL x = 1;

  for ( int i=1; i<=count; i++ ) {
    x= x*i;
    if ( (i<100) || (i%10)==0 ) {
      cout << x.as_INTEGER() << " =  " << i << "!\n";
    }
  }
}


void factorial_RATIONAL(int count){

  RATIONAL x = 1;
   for ( int i=1; i<=count; i++ ) {
    x= x*i;
    if ( (i<100) || (i%10)==0 ) {
      cout << x << " =  " << i << "!\n";
    }
  }
}



void factorial_INTEGER(int count){


  INTEGER x = 1;

  for ( int i=1; i<=count; i++ ) {
    x= x*i;
    if ( (i<100) || (i%10)==0 ) {
      cout << x << " =  " << i << "!\n";
    }
  }
}

void factorial_INTEGER_real(int count){


  INTEGER x = 1;

  for ( int i=1; i<=count; i++ ) {
    x= x*i;
    if ( (i<100) || (i%10)==0 ) {
      cout << REAL(x) << " =  " << i << "!\n";
    }
  }
}


void compute(){
  int test,count;
  cout << setRwidth(50);;

  cout << "Factorial: x = n!\n";
  cout << "How to compute (1=REAL, 2=double, 3=RATIONAL, 4=INTEGER "
       << "5=REAL with int-output, 6 INTEGER with REAL-output) : ";
  cin  >> test;
  cout << "How many values: ";
  cin  >> count;
  cout << "\n";
  switch (test) {
     case 1:;
       factorial_REAL(count);
     break;
     case 2:;
       factorial_double(count);
     break;
     case 3:;
       factorial_RATIONAL(count);
     break;
     case 4:;
       factorial_INTEGER(count);
     break;
     case 5:;
       factorial_REAL_int(count);
     break;
     case 6:;
       factorial_INTEGER_real(count);
     break;
  }


}
