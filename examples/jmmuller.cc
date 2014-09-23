#include "iRRAM.h"

using namespace iRRAM;

/* Compute iteration by J.M.Muller */

bool print_flag=true;

void jmm_float(int count){

  float a=11.0/2.0, b=61.00/11.0, c;

  for (long i=0;i<count;i++ ) {
  
    if (print_flag) {
      cout << a << " " << i <<"\n" ;
    }
    
    c=111-(1130-3000/a)/b;
    a=b; b=c;   

  }
  cout << a << " " << count <<"\n" ;
}

void jmm_double(int count){

  double a=11.0/2.0, b=61.00/11.0, c;

  for (long i=0;i<count;i++ ) {
  
    if (print_flag) {
      cout << a << " " << i <<"\n" ;
    }
    c=111-(1130-3000/a)/b;
    a=b; b=c;   

  }
  cout << a << " " << count <<"\n" ;
}

void jmm_DYADIC(int count){

  DYADIC a=DYADIC(11)/2, b=DYADIC(61)/11, c;

  for (long i=0;i<count;i++ ) {
  
    if (print_flag) {
      cout << a << " " << i <<"\n" ;
    }

    c=111-(1130-3000/a)/b;
    a=b; b=c;   

  }
  cout << a << " " << count <<"\n" ;
}


void jmm_RATIONAL(int count){

  RATIONAL a=RATIONAL(11)/2, b=RATIONAL(61)/11, c;

  for (long i=0;i<count;i++ ) {

    if (print_flag) {
      cout << REAL(a) << " " << i <<"\n" ;
    }

    c=111-(1130-3000/a)/b;
    a=b; b=c;   

  }
  cout << REAL(a) << " " << count <<"\n" ;
}

void jmm_REAL(int count){

  REAL a= REAL(11)/2, b=REAL(61)/11, c;

  for (long i=0;i<count;i++ ) {

    if (print_flag) {
      cout << REAL(a) << " " << i <<"\n" ;
    }

    c=111-(1130-3000/a)/b;
    a=b; b=c;   

  }
  cout << REAL(a) << " " << count <<"\n" ;
}


void compute(){
  int test,count;
  cout << "\nJMM-example: c=111-(1130-3000/a)/b\n";
  cout << "\nHow to compute (1=float, 2=double, 3=DYADIC, 4=RATIONAL, 5=REAL) : ";
  cin  >> test;
  cout << "How many values: ";
  cin  >> count;
  cout << "Print intermediate values (0/1): ";
  cin  >> print_flag;
  cout << "\n";
  switch (test) {
     case 1:;
       jmm_float(count);
     break;
     case 2:;
       jmm_double(count);
     break;
     case 3:;
       {
       long p;
       cout << "Dyadic precision: ";
       cin  >> p;
       cout << "\n";
       DYADIC_precision P(p);
       jmm_DYADIC(count);
       }
     break;
     case 4:;
       jmm_RATIONAL(count);
     break;
     case 5:;
       jmm_REAL(count);
     break;
  }
  cout << "\n";
}
