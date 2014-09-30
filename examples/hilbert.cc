#include "iRRAM.h"

using namespace iRRAM;

using std::setw;

/*
 Test Inversion of matrices, similar to 'matrix.cc', but with
 matrices of very bad condition.
*/
void write  (const REALMATRIX& x){
  for (unsigned int i=0;i<rows(x);i++) {
    for (unsigned int j=0;j<columns(x);j++) {
      cout << x(i,j)<< " ";
      }
    cout << "\n";
    }
}

void compute(){
  REALMATRIX m,n,o;
  int size,i,j;

  cout << "Inversion of Hilbert matrix\n"
       <<"Size of matrix: ";
  cin >> size;
  cout << "\n";

  cout << setw(10);

  m=REALMATRIX(size,size);
  for (i=0;i<size;i++)
  for (j=0;j<size;j++)
    m(i,j)=1/REAL(i+j+1);
 
  o=eye(size);
 
  cout << "Matrix H:\n";
  write(m);
  cout << "\n";

  n=o/m;

  cout << "Matrix K=inv(H):\n";
  write(n);
  cout << "\n";

  REAL mn=maxnorm(n);

  cout << "Maximum norm of K: ";
  cout << mn << "\n\n";

  o=m*n;

  cout << "Matrix K*H:\n";
  write(o);
  cout << "\n";
}
