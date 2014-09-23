#include "iRRAM.h"
#include "iRRAM/core.h"

using namespace iRRAM;

// test implementation of RATIONAL numbers using reals and a separation bound
// ... showing that separation bounds have a limited usability...

class RATIONAL_SEP
{
public:
RATIONAL_SEP (int i);
RATIONAL_SEP (const REAL& x,sizetype s1,sizetype s2,int len_arg);
~RATIONAL_SEP(){};
RATIONAL_SEP& operator = (const RATIONAL_SEP& y);

friend RATIONAL_SEP 	operator + 	(const RATIONAL_SEP& x, const RATIONAL_SEP& y);
friend RATIONAL_SEP 	operator - 	(const RATIONAL_SEP& x, const RATIONAL_SEP& y);
friend RATIONAL_SEP 	operator * 	(const RATIONAL_SEP& x, const RATIONAL_SEP& y);
friend RATIONAL_SEP 	operator / 	(const RATIONAL_SEP& x, const RATIONAL_SEP& y);
friend void 	rwrite		(const RATIONAL_SEP& x, const int w);

REAL r;
sizetype sep1,sep2;
int length;

};

RATIONAL_SEP::RATIONAL_SEP(int i){
 r=i;
 sizetype_set(sep1,i,0);
 sizetype_set(sep2,1,0);
 if (i<0) i=-i;
 if (i<10) length=1;
 else  
 if (i<100) length=2;
 else  
 if (i<1000) length=3;
 else  
 if (i<10000) length=4;
 else  
 if (i<100000) length=5;
 else  
 if (i<1000000) length=6;
 else  
 if (i<10000000) length=7;
 else  
 if (i<100000000) length=8;
 else  
 if (i<1000000000) length=9;
 else length=10;
}

RATIONAL_SEP::RATIONAL_SEP(const REAL& r_arg,sizetype sep1_arg,sizetype sep2_arg,int len_arg){
  r=r_arg;
  sep1=sep1_arg;
  sep2=sep2_arg;
  length=len_arg;
}

RATIONAL_SEP& RATIONAL_SEP::operator = (const RATIONAL_SEP& y){
  r=y.r;
  sep1=y.sep1;
  sep2=y.sep2;
  length=y.length;
  return (*this);
}

RATIONAL_SEP 	operator + 	(const RATIONAL_SEP& x, const RATIONAL_SEP& y)
{ REAL r=x.r+y.r;
  sizetype s1,s2,h1,h2;
  sizetype_mult(h1,x.sep1,y.sep2);
  sizetype_mult(h2,x.sep2,y.sep1);
  sizetype_add(s1,h1,h2);
  sizetype_mult(s2,x.sep2,y.sep2);
  return RATIONAL_SEP(r,s1,s2,x.length+y.length+1);
}

RATIONAL_SEP 	operator - 	(const RATIONAL_SEP& x, const RATIONAL_SEP& y){
 REAL r=x.r-y.r;
  sizetype s1,s2,h1,h2;
  sizetype_mult(h1,x.sep1,y.sep2);
  sizetype_mult(h2,x.sep2,y.sep1);
  sizetype_add(s1,h1,h2);
  sizetype_mult(s2,x.sep2,y.sep2);
  return RATIONAL_SEP(r,s1,s2,x.length+y.length+1);
}

RATIONAL_SEP 	operator * 	(const RATIONAL_SEP& x, const RATIONAL_SEP& y){
 REAL r=x.r*y.r;
  sizetype s1,s2;
  sizetype_mult(s1,x.sep1,y.sep1);
  sizetype_mult(s2,x.sep2,y.sep2);
  return RATIONAL_SEP(r,s1,s2,x.length+y.length+1);
}

RATIONAL_SEP 	operator / 	(const RATIONAL_SEP& x, const RATIONAL_SEP& y){
 REAL r=x.r/y.r;
  sizetype s1,s2;
  sizetype_mult(s1,x.sep1,y.sep2);
  sizetype_mult(s2,x.sep2,y.sep1);
  return RATIONAL_SEP(r,s1,s2,x.length+y.length+1);
}

void 	rwrite		(const RATIONAL_SEP& x, const int w){
}

void compute(){

  RATIONAL_SEP x = RATIONAL_SEP(1)/RATIONAL_SEP(2);
  RATIONAL_SEP c = RATIONAL_SEP(15)/RATIONAL_SEP(4);

  int count=25;
  cout << setRwidth(20);
  for ( int i=0; i<=count; i++ ) {
    if ( (i<100) || (i%10)==0 ) {
        cout << x.r << " " << i<<"\n"
          << " separators: "<<x.sep1.mantissa<< "*2^"<<x.sep1.exponent
          << " separators: "<<x.sep2.mantissa<< "*2^"<<x.sep2.exponent<<"\n"
          << " UC separator: length:"<<x.length
          << ", precision 10^-"<<13*x.length/10<< "\n";
    }
    x= c*x*(RATIONAL_SEP(1)-x);
  }
} 
