#include "iRRAM.h"

using namespace iRRAM;

#include <iostream>

/*
 Test of string conversion of reals, similar to 'hilbert.cc', but with
 different output
*/

void compute(){
  REALMATRIX m,n,o;
  int size,i,j;

  rprintf("Inversion of Hilbert matrix\n");
  rprintf("Size of matrix: ");
  rscanf("%d",&size);
  rprintf("\n");

  m=REALMATRIX(size,size);
  for (i=0;i<size;i++)
  for (j=0;j<size;j++)
    m(i,j)=1/REAL(i+j+1);
 
  o=eye(size);
 
  rprintf("Matrix H:\n");
  for (i=0;i<size;i++){
  for (j=0;j<size;j++){
    char * s=swrite(m(i,j),10);
    rprintf("%s ",s);
    delete s;}
    rprintf("\n");
  }
  rprintf("\n");

  n=o/m;

  rprintf("Matrix K=inv(H):\n");
//  rwrite(n,-50,10);
  for (i=0;i<size;i++){
  for (j=0;j<size;j++){
    char * s=swrite(n(i,j),10);
    rprintf("%s ",s);
    delete s;}
    rprintf("\n");
  }
  rprintf("\n");

  rprintf("Maximum norm of K: ");
  REAL mn=maxnorm(n);
  rshow(mn,20);
  rprintf("\n");
  rprintf("\n");

  o=m*n;

  rprintf("Matrix K*H:\n");
//  rwrite(o,-50,10);
  for (i=0;i<size;i++){
  for (j=0;j<size;j++){
    char * s=swrite(o(i,j),10);
    rprintf("%s ",s);
    delete s;}
    rprintf("\n");
  }
  rprintf("\n");
};
