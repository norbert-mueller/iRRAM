#include "iRRAM.h"

using namespace iRRAM;

/* Experimental, treat with care  */

/* Bernoulli numbers (modern definition, "even-index") */
/* program is only able to compute bernoulli_max=... value (see below) */

/* time for the Bernoulli numbers Pentium III-1200 :
   B_1000 = 2   sec
   B_2000 = 20  sec
   B_4000 = 246 sec
   so the complexity of B_n seems to be about O(n^3.3) and
   B_20000 ~ 10 h
     
   The values are evaluated only once, then cached...
   An implementaion with variable sized arrays would surely be 
   better than just using a fixed size array... 
*/

/* Gamma function is just an approximation, not an exact value! */
/* see below */

/* Bernoulli numbers (modern definition, "even-index") */

const unsigned int bernoulli_max=20000;
RATIONAL bernoulli_saved[bernoulli_max/2];
INTEGER binomial_saved[bernoulli_max];
unsigned int bernoulli_min_unknown=0;


RATIONAL bernoulli(const unsigned int index)
{
if (index >= bernoulli_max ){ fprintf(stderr,"noch nicht implementiert\n"); exit(1); }
  
  if ( index   == 1 ) return RATIONAL(-1,2);
  if ( index%2 == 1 ) return RATIONAL(0);
  if (bernoulli_min_unknown == 0){
     bernoulli_saved[0]=RATIONAL(1);
     binomial_saved[0]=1;
     binomial_saved[1]=2;
     binomial_saved[2]=1;
     bernoulli_min_unknown=2;
  }  
  while ( bernoulli_min_unknown <= index ) {
    INTEGER saved_value=1; 
    INTEGER old_saved_value;
    for (unsigned int i = 1; i <= bernoulli_min_unknown; i++ ){
      old_saved_value=saved_value;
      saved_value=binomial_saved[i];
      binomial_saved[i] += old_saved_value;
    }

    binomial_saved[bernoulli_min_unknown+1]=1;

    if ( bernoulli_min_unknown%2==0 ){
      RATIONAL summe=bernoulli_saved[0]+binomial_saved[1]*RATIONAL(-1,2);
      for (unsigned int i = 2; i < bernoulli_min_unknown; i+=2 ){
        summe=summe + bernoulli_saved[i/2]*binomial_saved[i];
      }
      summe= -1*summe/binomial_saved[bernoulli_min_unknown];
      bernoulli_saved[bernoulli_min_unknown/2]= summe;	    
    }
    bernoulli_min_unknown++;  
  }
  return bernoulli_saved[index/2];
}

// Approximations to the log of the Gamma function
// Problem: The series is alternating, but does not converge....
// We just compute a few terms:

REAL ln_gamma_approx(int max_index, const REAL& z)
{
  REAL result= (z-REAL(1)/2)*log(z)-z+log(2*pi())/2;
  
  REAL coefficient;
  REAL inv_power_z=1/z;
  REAL inv_square_z=1/(z*z);
  for (int index =1;index <= max_index; index ++ ){
    coefficient  = REAL(bernoulli(2*index))/(2*index-1)/(2*index) * inv_power_z;
    result += coefficient;
    inv_power_z= inv_power_z*inv_square_z;
  };
  return result;
}

// An experiment to compute the Gamma function
// We make unproven(!) guesses on 
// - how to do the range reduction
// - how to approximate the log_gamma function

// TREAT THIS WITH CARE!
// The result looks ok for small value of x and p,
// but e.g. for x=150 and p~-3000, about 300 bits seem to be wrong!
// Again: TREAT THIS WITH CARE!

REAL gamma_approx(int p, const REAL& x){
  if (p>0) p=0;
  REAL y=x;
  int x_red = y.as_INTEGER();
  REAL y_factorial = 1;
  for ( int j = x_red; j < -p/5; j++)
    {
      y_factorial *= y;
      y += 1;
    }  
  return exp(ln_gamma_approx(-p/10, y))/y_factorial;
}

REAL gamma(const REAL& x){
  return limit(gamma_approx,x);
}

void compute (){
  int w;
  REAL d;

  cout << "Print gamma(d) with width w : Please input d and w!\n";
  cin >> d;
  cin >> w;

  REAL r=gamma(d);
  cout << setRwidth(w) << r <<" at "<<d<<"\n";
}


