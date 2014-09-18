#include "iRRAM.h"

using namespace iRRAM;

/* Simple test program:  working with matrices */

void write  (const REALMATRIX& x){
  for (unsigned int i=0;i<rows(x);i++) {
  for (unsigned int j=0;j<columns(x);j++) {
      cout << x(i,j)<< " ";
  }
  cout << "\n";
  }
}


void compute(){
  int size=3;
  REALMATRIX m(size,size),n(size,size),o(size,size); 

  cout << setw(30);

/* Initializing matrix m*/
   m(0,0)=2;
   m(0,1)=0;
   m(0,2)=0;
   m(1,0)=0;
   m(1,1)=1;
   m(1,2)=2;
   m(2,0)=0;
   m(2,1)=2;
   m(2,2)=3;
 
   cout << "example matrix m:\n\n";
   write(m);

/* Initializing matrix o as unit */

   o=eye(size);

/* Computing the inverse of m */

   n=o/m;

/* Printing the inverse */
   cout << "\nmatrix n=o/m:\n\n";
   write(n);

/* Multiplying m and its inverse */

   cout << "\nmatrix m*n:\n\n";
   o=m*n;

/* Printing the resulting product */
   write(o);

/* Testing the matrix exponential function */

  o= exp(eye(size));
  cout << "\nmatrix exp(eye(size)):\n\n";
  write(o);

/* Another test for the "scaling and squaring" method */

  REALMATRIX p(2,2);
  p(0,0)=-49;
  p(0,1)= 24;
  p(1,0)=-64;
  p(1,1)= 31;
  cout << "\n\nmatrix p:\n\n";
  write(p);

  p= exp(p);

  cout << "\nmatrix exp(p):\n\n";
  write(p);

};
