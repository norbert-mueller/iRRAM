#include "iRRAM.h"

using namespace iRRAM;
using std::endl;

/*
 Test Inversion of matrices, similar to 'matrix.cc', but with
 matrices of very bad condition.
*/

orstream outfile("fileio.dat2",std::_S_out);


void write  (const REALMATRIX& x){
  for (unsigned int i=0;i<rows(x);i++) {
    for (unsigned int j=0;j<columns(x);j++) {
      outfile << x(i,j)<< " ";
      }
    outfile << "\n";
    }
}


void compute(){
  REALMATRIX m,n,o;
  int size,i,j;

  cout << "Inversion of Hilbert matrix\n"
       << "Please give dimension n! ";

  cin  >> size;
  cout << size << "\n";
  
  {
    orstream outfile("fileio.dat");
    for (i=0;i<size;i++) {
      for (j=0;j<size;j++)
        outfile << i+j+1 << " ";
      outfile << "\n";
    } 
    outfile << endl;
  }

  m=REALMATRIX(size,size);
  {
    REAL d;
    irstream infile("fileio.dat");
    for (i=0;i<size;i++)
    for (j=0;j<size;j++){
      if (!( infile >> d )) {cerr << "IO-Problem 3";exit(1);}
      m(i,j)=1/d;
    }
  }
  
  o=eye(size);
 
  cout << "Matrix H:\n";
  outfile << "Matrix H:\n";
  write(m);
  cout << "\n";
  outfile << "\n";

  n=o/m;

  cout << "Matrix K=inv(H):\n";
  outfile << "Matrix K=inv(H):\n";
  write(n);
  cout << "\n";
  outfile << "\n";

  REAL mn=maxnorm(n);

  cout << "Maximum norm of K: ";
  cout << mn << "\n\n";

  o=m*n;

  cout << "Matrix K*H:\n";
  outfile << "Matrix K*H:\n";
  write(o);
 
  outfile << endl;
  cout << "\n\nResults are in fileio.dat2\n";
};
