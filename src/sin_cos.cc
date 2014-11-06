#include <cstdio>
#include <cstdlib>

#include <iRRAM/core.h>

namespace iRRAM {

/*****************************************************************/
/*                     sin(x), cos(x), tan(x)  etc               */
/*****************************************************************/

// Here, the Lipschitz bounds will usually be valid 
//on the whole set of real numbers;
bool total_domain(const REAL& x){ return true;}

REAL sin_taylor (int prec, const REAL& x){
// We compute sin(x) to a precision of 2^prec using the Taylor
// expansion. The evaluation of the expansion is improved by grouping
// wd values together.
// The argument x will be always smaller than 1, 
// usually it will be very small...

// All computations in this routine are exact, 
// but the truncation error must be taken into account.

  int sx=upperbound(x);
  if (prec >= sx ) return 0;
  int wd;

  if ( prec > -100 )        wd= 1;
  else if ( prec > -200 )   wd= 1;
  else if ( prec > -400 )   wd= 2;
  else if (prec > -800 )    wd =2;
  else if (prec > -1600 )   wd =2;
  else if (prec > -3200 )   wd =3;
  else if (prec > -6400 )   wd =4;
  else if (prec > -12000 )  wd =5;
  else if (prec > -24000 )  wd =8;
  else if (prec > -48000 )  wd =12;
  else if (prec > -100000 ) wd =18;
  else if (prec > -200000 ) wd =26;
  else wd=int(std::sqrt(double(-prec)/18));
  
  REAL x2=square(x);
  std::vector<REAL> xpow(wd);
  REAL xf=power(x2,wd),z=x;
  xpow[0]=x;
  for (int i=1; i<wd; i++) {
     xpow[i]=xpow[i-1]*x2/(-2*i*(2*i+1));
     z+=xpow[i];
  }
  REAL xp=1,e=z;
  INTEGER f=1;
  int i=1;
  int t=upperbound(xf),t1=t;
  while ( !bound(e,prec) || ! bound(e,-1) ) {
    xp*=xf; e=0;
    for (int j=0; j<wd; j++) {
      f=1;
      for (int k=i;k<i+wd;k++) f*=(-2*k*(2*k+1));
      xpow[j]=scale(xpow[j],t)/REAL(f);
      i=i+1;
      e+=xpow[j];
    }
    e*=scale(xp,-t1); t1=t1+t;
    z+=e;
  }
//  printf("%d %d %d\n",i,prec,upperbound(x));
  return z;
}

REAL sin_range_red3 (int hint,const REAL& x){
// This function computes sin(x) (exactly) on [-1,1]
// using a range reduction and a call to a further sin(x)-algorithm.

// For this function itself, it is not necessary that we have an
// argument from [-1,1], it is just easier for the subroutine!

// The number of range reduction steps is computed heuristically
// depending on the precision.

// This function works exactly, the precision is only needed as a hint 
// for improving the performance!
  int it;
  if ( hint > -100 )        it =4;
  else if ( hint > -200 )   it =6; 
  else if ( hint > -400 )   it =8; 
  else if (hint > -800 )    it=10; 
  else if (hint > -1600 )   it=12; 
  else if (hint > -3200 )   it=14; 
  else if (hint > -6400 )   it=16; 
  else if (hint > -12000 )  it=18;
  else if (hint > -24000 )  it=20;
  else if (hint > -48000 )  it=24;
  else if (hint > -100000 ) it=26;
  else if (hint > -200000 ) it=28;
  else  it=(int)(std::log(double(-hint))*2+20);

// We use the reduction scheme: sin(z)=3* sin(z/3) - 4sin(z/3)^3
  REAL y=x;
  int jt=it;
  while (jt>=10) {y=y/59049;jt=jt-10;};
  while (jt>=8)  {y=y/6561;jt=jt-8;};
  while (jt>=6)  {y=y/729;jt=jt-6;};
  while (jt>=4)  {y=y/81;jt=jt-4;};
  while (jt>=2)  {y=y/9;jt=jt-2;};
  while (jt>=1)  {y=y/3;jt=jt-1;};
  y=limit_lip(sin_taylor,0,total_domain,y);
  for (int i=1;i<=it;i+=1){//cerr<< i<<":\n"; y.rcheck();
    y*=(3-scale(square(y),2));
}
  return y;
}

REAL sin_range_red1 (int prec,const REAL& x){
// This function computes an approximation to sin(x) for arbitray x.

// To achieve this, we do a range reduction of x to the interval
// [0-eps,pi/4+eps] where eps<=0.01. This reduction is done precisely,
// so that is sufficient to use identities on sine
// for the actual computation of sin(x).

// With the help of a further algorithm working fast on this area,
// we then do the actual computation of sin(x).
// The precision is only needed as a hint how to do this quite fast!

// As |sin(x)|<=1, the number 0 is a valid approximation to cos(x)
// with precision 2^{-0}=1
   if (prec >= 0 ) return 0;
// If the required precision is higher, we actually have to compute sin(x)
// In this case, the result is not an approximation but precise, indeed!


   int x_int;
   REAL x_red;
   bool sin_neg;
//For larger numbers, we increase the working precision a bit:
   {
   stiff code(!bound(x,2) ? +1 : 0);

// We reduce x to -2pi..2pi by taking the value modulo 2pi
   x_red=modulo(x,2*pi());
// We take an integer approximation x_int of 100*x_red,
// which must be between -629..629
// This allows us to easily find a reasonable reduction interval
   x_int=round(x_red*100);
// We have |x_int:100-x_red| <= 0.01

// In case x_int is negative, we shift by 2pi(),
// so we still have sin(x)=sin(x_red)
   if ( x_int <  0 ) {x_int=x_int+628;x_red=x_red+2*pi();}
// We now have /x_int:100 - x_red/ < 0.02, 0<=x_int<=629 and
// -0.01 <= x_red <= 2pi

// In case x_int > 314, we reduce using sin(x)=-sin(x-pi)
// so we still have sin(x)=sin(x_red)*sin_sign;
   sin_neg=false;
   if ( x_int >  314 ) {x_int=x_int-314;x_red=x_red-pi();sin_neg=true;}
// We now have /x_int:100 - x_red/ < 0.03 and
// -0.01 <= x_red <= pi+0.01

// In case x_int > 157, we reduce using sin(x)=sin(pi-x),
// so still sin(x)=sin(x_red)*sin_sign;
   if ( x_int >  157 ) {x_int=157-x_int;x_red=pi()-x_red;}
// We now have /x_int:100 - x_red/ < 0.04 and
// -0.02 <= x_red <= pi/2+0.02
  }

// Now we do an exact(!) computation of sin(x).
// The procedure uses the argument prec just as an hint for optimization!
   REAL sin_abs;
   if ( x_int > 80 ) 
      sin_abs = sqrt(1-square(sin_range_red3(prec,x_red-pi()/2)));
   else
      sin_abs = sin_range_red3(prec,x_red);
   return sin_neg ? -sin_abs : sin_abs;
}

REAL cos  (const REAL& x){
   return  limit_lip(sin_range_red1,0,total_domain,x+pi()/2);
}

REAL sin (const REAL& x){
   return  limit_lip(sin_range_red1,0,total_domain,x);
}

REAL tan (const REAL&x){
  REAL pi1=pi();
  REAL x_reduced=modulo(x,pi1);
  REAL s=sin(x_reduced);

  pi1=scale(pi1,-1);

  if ( (x_reduced>-pi1) && (x_reduced<pi1) ) {
    return s/sqrt(1-s*s);
  } else{
    return -s/sqrt(1-s*s);
  }
}

///////////////////////////////////////////////////////////////////
REAL atan_approx (int prec, const REAL& x){
   if ( prec >= 2 ) return 0;
   int i=2;
   REAL x2=x*x,y=-x*x2,z=x+y/3;
   while ( !bound(y,prec) || ! bound(y,-1) ) {
     y=-y*x2;
     z=z+y/(i+3);
     i+=2;
   }
   return z;
}

REAL atan_reduction (int p, const REAL& x) {
//only applied for |x|<2
 if (bound (x,p)) return 0;
 int s=size(x)-3;
 int red=2;
 REAL y=(sqrt(x*x+1)-1)/x; 
  for (int i=-p;i>-5*s;i=int(i/(1.3))){ 
   red+=1; 
   y=  (sqrt(y*y+1)-1)/y;
 }
 return scale(limit_lip(atan_approx,0,total_domain,(sqrt(y*y+1)-1)/y),red);
}

REAL atan (const REAL& x) {
// arbitrary parameter x
  REAL result;
  single_valued code;
  switch (choose( x<-1,x>1,abs(x)<REAL(1.1)))
  { case 1:
		result= -(pi()/2 - atan(-1/x));
		break;
    case 2:
		result=   pi()/2 - atan(1/x);
		break;
    case 3:
		result= limit_lip(atan_reduction,0,total_domain,x);
		break;
  }
 return result;
}


REAL asin (const REAL& x){
   return atan(x/sqrt(1-x*x));
}

REAL acos (const REAL& x){
   return pi()/2 - asin(x);
}

REAL asec (const REAL& x){
   return acos(1/x);
}

REAL acosec (const REAL& x){
   return asin(1/x);
}

///////////////////////////////////////////////////////////////////
REAL sec (const REAL& x) {
 return 1/cos(x);
}

REAL cosec (const REAL& x) {
 return 1/sin(x);
}

REAL cotan (const REAL& x) {
 return cos(x)/sin(x);
}

///////////////////////////////////////////////////////////////////

REAL sech (const REAL& x) {
// 2/(exp(x)+exp(-x));
  single_valued code;
  REAL epx,result;
  switch (choose(x>0,true))
  { case 1: 
		epx=exp(-x);
		result= 2/(1/epx+epx);
		break;
    case 2:
		epx=exp(x);
		result= 2/(epx+1/epx);
		break;
  }
  return result;
}

REAL cosech (const REAL& x) {
// 2/(exp(x)-exp(-x));
  single_valued code;
  REAL epx,result;
  switch (choose(x>0,true))
  { case 1: 
		epx=exp(-x);
		result= 2/(1/epx-epx);
		break;
    case 2:
		epx=exp(x);
		result= 2/(epx-1/epx);
		break;
  }
  return result;
}

REAL sinh (const REAL& x) {
// (exp(x)-exp(-x))/2;
  single_valued code;
  REAL epx,epxi,result;
  switch (choose(x>0,true))
  { case 1: 
		epx=exp(-x);
		result= (1/epx-epx)/2;
		break;
    case 2:
		epx=exp(x);
		result= (epx-1/epx)/2;
		break;
  }
  return result;
}

REAL cosh (const REAL& x) {
// (exp(x)+exp(-x))/2
  single_valued code;
  REAL epx,epxi,result;
  switch (choose(x>0,true))
  { case 1: 
		epx=exp(-x);
		result= (epx+1/epx)/2;
		break;
    case 2:
		epx=exp(x);
		result= (epx+1/epx)/2;
		break;
  }
  return result;
}

REAL tanh (const REAL& x) {
// just do a reformulation of (exp(x)-exp(-x))/(exp(x)+exp(-x));
// in order try not to work with numbers that are very large...
  single_valued code;
  REAL epx,epxi,th;
  switch (choose(x>0,true))
  { case 1: 
		epxi=exp(-x);
		epxi=epxi*epxi;
		th= ((1-epxi)/(1+epxi));
		break;
    case 2:
		epxi=exp(x);
		epxi=epxi*epxi;
		th =  ((epxi-1)/(1+epxi));
		break;
  }
  return th;
}

REAL coth (const REAL& x) {
// just do a reformulation of (exp(x)+exp(-x))/(exp(x)-exp(-x));
// in order try not to work with numbers that are very large...
  single_valued code;
  REAL epx,epxi,th;
  switch (choose(x>0,true))
  { case 1: 
		epxi=exp(-x);
		epxi=epxi*epxi;
		th= ((1+epxi)/(1-epxi));
		break;
    case 2:
		epxi=exp(x);
		epxi=epxi*epxi;
		th =  ((epxi+1)/(1-epxi));
		break;
  }
  return th;
}

REAL asinh (const REAL& x) {
 return log(x+sqrt(x*x+1));
}

REAL acosh (const REAL& x) {
 return log(x+sqrt(x*x-1));
}

REAL atanh (const REAL& x) {
 return log((1+x)/(1-x))/2;
}

REAL acoth (const REAL& x) {
 return log((1-x)/(1+x))/2;
}


REAL asech (const REAL& x){
   return acosh(1/x);
}

REAL acosech (const REAL& x){
   return asinh(1/x);
}


} // namespace iRRAM

