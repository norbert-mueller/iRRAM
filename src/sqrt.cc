#include <cstdio>
#include <cstdlib>

#include <iRRAM/core.h>

#if iRRAM_BACKEND_MPFR
	#include "MPFR_ext.h"
#else
	#error "Currently no additional backend!"
#endif

namespace iRRAM {

REAL root_approx(int prec, const REAL& x,int n) {
   int u=upperbound(x)/n;
   REAL error;
   if (u < prec) return 0;
   REAL r=scale(REAL(1),u);
   REAL r_2=x/power(r,n-1);
   do {
      r+=(r_2-r)/n;
      r_2=x/power(r,n-1);
      error=r - r_2;
   } while ( !bound(error,prec) );
   return r;
}

REAL root(const REAL& x,int n) {
   return limit(root_approx,x,n);
}



#ifdef OLDSQRT
#ifdef MP_mv_sqrt

  REAL MP_sqrt_approx (int prec, const REAL& x){
    DYADIC xd=approx(x,prec-1);
    MP_mv_sqrt(xd.value,xd.value,prec-1);
    return REAL(xd);
  };
  #define SQRT_APPROX MP_sqrt_approx

#else

  REAL gen_sqrt_approx(int k,const REAL& x){
    REAL a=1,b=x/a;
    do {
       a=(a+b)/2;
       b=x/a;
    } while ( !bound(a-b,k) );
    return a;
  }
  #define SQRT_APPROX gen_sqrt_approx

#endif

REAL sqrt_approx (int prec, const REAL& x){
  if (bound(x, 2*prec) ) return 0;
  int s2=size(x)/2;
  REAL y=scale(x,-s2*2);
  y=lipschitz(SQRT_APPROX,2,prec-s2,y);
  y=scale(y,s2);
  return y; 
}


REAL sqrt (const REAL& x){
 return limit(sqrt_approx,x);
}
#else



#ifdef MP_mv_sqrt

  REAL sqrt(const REAL& x){
  if (!x.value) (const_cast<REAL&>(x)).mp_make_mp();
  MP_type zvalue;
  sizetype zerror,xsize,proderror;
  int local_prec;
  x.getsize(xsize);
  sizetype_shift(zerror,x.error,2);
  if (sizetype_less(xsize,zerror)) {
    MP_init(zvalue);
    MP_int_to_mp(0,zvalue); 
    sizetype_sqrt(zerror,x.error);
    sizetype_shift(zerror,zerror,1);
    return REAL(zvalue,zerror);
  }
  sizetype_dec(xsize,x.error);
  sizetype_sqrt(proderror,xsize);
  sizetype_div(zerror,x.error,proderror);
  sizetype_half(zerror,zerror);
  if (ACTUAL_STACK.prec_policy == 0)
    local_prec=max(ACTUAL_STACK.actual_prec,zerror.exponent);
  else
    local_prec=max(xsize.exponent+ACTUAL_STACK.actual_prec,zerror.exponent);
  MP_init(zvalue);
  MP_mv_sqrt(x.value,zvalue,local_prec);
  sizetype_inc_one(zerror,local_prec);
//printf("%d*2^%d\n",zerror.mantissa,zerror.exponent);
  return REAL(zvalue,zerror);
}

#else

  REAL gen_sqrt_approx(int k,const REAL& x){
    REAL a=1,b=x/a;
    do {
       a=(a+b)/2;
       b=x/a;
    } while ( !bound(a-b,k) );
    return a;
  }
  #define SQRT_APPROX gen_sqrt_approx


REAL sqrt_approx (int prec, const REAL& x){
  if (bound(x, 2*prec) ) return 0;
  int s2=size(x)/2;
  REAL y=scale(x,-s2*2);
  y=lipschitz(SQRT_APPROX,2,prec-s2,y);
  return scale(y,s2); 
}


REAL sqrt (const REAL& x){
 return limit(sqrt_approx,x);
}
#endif


} // namespace iRRAM

#endif

