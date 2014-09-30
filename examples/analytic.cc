#include "iRRAM.h"
#include <math.h>

/* 
  a first attempt to implement a data type for real functions
  here restricted to functions that are analytic on [-1,1] 

  unfinished!
*/

using namespace iRRAM;

using std::setw;

class ANALYTIC 
{
public:

// Constructors: -------------------------------

ANALYTIC();

/* constructor for the function f(x)=a*x^n */
ANALYTIC(const REAL& a, const unsigned int& n);

ANALYTIC&   operator = (const ANALYTIC& y);

~ANALYTIC();

// Standard Arithmetic: ------------------------

friend ANALYTIC  operator +  (const ANALYTIC& f,
                             const ANALYTIC& g);
friend ANALYTIC  operator -  (const ANALYTIC& f,
                             const ANALYTIC& g);
friend ANALYTIC  operator *  (const ANALYTIC& f,
                             const ANALYTIC& g);
friend ANALYTIC  operator /  (const ANALYTIC& f,
                             const ANALYTIC& g);

friend REAL evaluate  (const ANALYTIC& f,
                             const REAL& x);
friend void debug  (const ANALYTIC& f);

private :
unsigned int degree;
REAL* coeff;
DYADIC M;
DYADIC R;
};

ANALYTIC::ANALYTIC(){
degree=0;
coeff=new REAL[1];
coeff[0]=0;
DYADIC M,R;
}

ANALYTIC::~ANALYTIC(){
delete []coeff;
}

ANALYTIC::ANALYTIC(const REAL& a, const unsigned int& n){
degree=n;
coeff=new REAL[n+1];
coeff[n]=a;
M=0;
R=0;
}

REAL evaluate(const ANALYTIC& f,const REAL& x){
REAL result=f.coeff[0];
REAL powx=1;
for (unsigned int i=1; i<= f.degree;i++)
  {
   powx=powx*x;
   result=result+f.coeff[i]*powx;
   }
return result;
}

ANALYTIC& ANALYTIC::operator = (const ANALYTIC& y) {
  if ( degree != y.degree) {
     delete []coeff;
     coeff=new REAL[y.degree];
     }
  degree = y.degree;
  for (unsigned int i=0;i<=degree;i++) coeff[i]=y.coeff[i];
  M=y.M;
  R=y.R;
  return(*this);
}

void debug(const ANALYTIC& f){
cout <<"Degree:" << f.degree << "\n";
  for (unsigned int i=0;i<=f.degree;i++)
    {
    cout <<setw(4) << i <<" " << setRwidth(40) << f.coeff[i] << "\n";
    }
}

void compute (){
ANALYTIC f(REAL(1),20);
debug(f);

cout << setRwidth(45)<< evaluate(f,REAL(1)/2) << "\n";

}
