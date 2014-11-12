#include <cstdio>
#include <cstdlib>

#include <iRRAM/core.h>

namespace iRRAM {

REAL exp_approx (int prec, const REAL& x){
precision_policy(iRRAM_RELATIVE);
  REAL xs=x/ln2();
  REAL z;

  int s=round(xs)+1;
  int it,wd;

  int min_bound=prec-s;
  if (min_bound > -2) min_bound=-2;
  
  wd= 1+ int(std::sqrt(double(-prec)))/16;
  
  if ( wd <2 )    it=int(std::log(double(-prec)))*2;
  else            it=int(std::log(double(-prec)))*4;


  xs=scale(x-ln2()*s,-it);

  if ( wd > 1 ) {
    std::vector<REAL> xpow(wd);
    REAL xf=power(xs,wd);
    xpow[0]=1;
    z=1;
    for (int i=1; i<wd; i++) {
      xpow[i]=xpow[i-1]*xs/i;
      z+=xpow[i];
      }
    REAL xp=1,e=z;
    INTEGER f=1;
    int i=1;
    int t=upperbound(xf),t1=0;
    while ( !bound(e,min_bound) ) {
      t1=t1+t;
      xp*=xf; 
      e=0;
      for (int j=0; j<wd; j++) {
        f=1;
        for (int k=i;k<i+wd;k++) f*=k;
        xpow[j]=scale(xpow[j],t)/REAL(f);
        i=i+1;
        e+=xpow[j];
        }
      e*=scale(xp,-t1);
      z+=e;
      }
    }
  else {
    REAL e=xs;
    z=1+e;
    int i=1;
    while ( !bound(e,min_bound) ) {
      i+=1;
      e*=(xs/i); 
      z+=e;
      }
    }

  for (int i=1; i<= it;i+=1)z*=z;

  return scale(z,s);
}

int exp_bound(const REAL& x)
{return (int)((round(x)+1)*1.443);}

REAL exp (const REAL& x)
{
  if ( positive(x-750001,0)  ) {fprintf(stderr,"Overflow in exp(x)\n");exit(1);}
  single_valued code;
  REAL y= limit_lip(exp_approx,exp_bound,x);
  return y;
}

REAL euler_approx (int prec)
{
  if ( prec >= 2 ) return 0;
  REAL z=1,y=z;
  int i=1;
  while ( !bound(y,prec-1) ) {
    y=y/i;
    z=z+y;
    i+=1;
  }
  return z;
}

REAL euler ()
{
  return limit(euler_approx);
}



REAL log_taylor_approx(int prec, const REAL& x0) {
// Computing series expansion for log(x) for x > 1/2
   REAL x=x0;
//  printf("####log ser#\n");
   int it=(int)(std::sqrt((double) -prec/4)); 
   for (int i=1;i<=it;i+=1) x=sqrt(x);
   REAL y=1-1/x;
   REAL y2=y*y,y3=y*y2,y4=y2*y2,y5=y2*y3,y6=y3*y3,yf=y5,yp=1;
   REAL s=y;
   int i=5;
   REAL z=y+y2/2+y3/3+y4/4+y5/5+y6/6;
   while ( !bound(s,prec-it) ) {
    yp*=yf;
    s=yp*(y2/(i+2)+y3/(i+3)+y4/(i+4)+y5/(i+5)+y6/(i+6));
    i=i+5;
    z+=s;
   }
   return scale(z,it);
}


REAL iterate
   (REAL f(REAL&, REAL&),
   REAL const& a_0, REAL const& b_0)
{ REAL a=a_0,b=b_0,e;
sizetype a_error,e_error;
single_valued code;
do {
  e=f(a,b);
} while (! bound(e,15*ACTUAL_STACK.actual_prec/16));
//} while (! bound(e,ACTUAL_STACK.actual_prec));
a.geterror(a_error);
e.geterror(e_error);
sizetype_inc(a_error,e_error);
e.getsize(e_error);
sizetype_inc(a_error,e_error);
a.seterror(a_error); 
return a;
}

REAL agm(REAL& a,REAL& b){
  REAL e=a*b;
  a=scale(a+b,-1); 
  b=sqrt(e);
  return a-b;
}



REAL log_agm_approx(int prec, const REAL& z) {
  int N=25-prec/2;
  precision_policy(7);
  REAL delta1= scale(z,-N);
  REAL l = ln2()*(N+2)-scale(pi()/iterate(agm,REAL(1),delta1),-1);
  precision_policy(0);
  return l;
}

REAL log_approx(int prec, const REAL& z) {
  if ( prec > -100) return log_taylor_approx(prec,z);
  return log_agm_approx(prec,z);
}

bool log_domain(const REAL& x)
{return x> REAL(1);}

REAL log(const REAL& x) {
//x.rcheck(200);
 int s=size(x);
 REAL y= sqrt(sqrt(scale(x,2-s)));
//y.rcheck(200);
 y=limit_lip(log_approx,2,log_domain,y);
 return 4*y+(s-2)*ln2();
}


} // namespace iRRAM
