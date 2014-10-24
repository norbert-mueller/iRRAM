/*

mpfr_extension.cc -- extension to the GMP float functions for the iRRAM library
 
Copyright (C) 2002-2006 Norbert Mueller
 
This file is part of the iRRAM Library.
 
The iRRAM Library is free software; you can redistribute it and/or modify
it under the terms of the GNU Library General Public License as published by
the Free Software Foundation; either version 2 of the License, or (at your
option) any later version.
 
The iRRAM Library is distributed in the hope that it will be useful, but
WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Library General Public
License for more details.
 
You should have received a copy of the GNU Library General Public License
along with the iRRAM Library; see the file COPYING.LIB.  If not, write to
the Free Software Foundation, Inc., 59 Temple Place - Suite 330, Boston,
MA 02111-1307, USA. 
*/

#include <iRRAM/core.h>
#include <iRRAM/mpfr_extension.h>

#define MPFR_PREC(x) ((x)->_mpfr_prec)
#define MPFR_SIZE(x) ((MPFR_PREC(x)-1)/BITS_PER_MP_LIMB+1)
#define MPFR_EXP(x) ((x)->_mpfr_exp)
#define MPFR_MANT(x) ((x)->_mpfr_d)

using namespace iRRAM;

typedef  DYADIC (DYADIC_function) (const DYADIC &,const int &);

inline void mpfr_wrapper(DYADIC_function f, mpfr_ptr r, mpfr_srcptr u,int p){
  DYADIC arg,res;
  mpfr_set_prec(arg.value,mpfr_get_prec(u));
  mpfr_set(arg.value,u,__gmp_default_rounding_mode);
  res = iRRAM_exec(f,arg,p);
  mpfr_set_prec(r,mpfr_get_prec(res.value));
  mpfr_set(r,res.value,__gmp_default_rounding_mode);
}


DYADIC compute_test(const DYADIC &x, const int &p) { return approx(sin(REAL(x)) ,p); }

void mpfr_ext_test (mpfr_ptr y, mpfr_srcptr u, int p, mp_rnd_t rnd_mode)
   {
    mpfr_t r;
    mpfr_init(r);
    int err=50;
    if (p< 0) err= p;
    do  {
       err=err-10;
       mpfr_wrapper(compute_test,r,u,err);
//     printf(" %d %d %d\n",MPFR_EXP(r),err,MPFR_EXP(r)+err);
     } while (!  mpfr_can_round(r,MPFR_EXP(r)+err ,__gmp_default_rounding_mode, rnd_mode, p ));
 
     mpfr_set_prec(y,p);
     mpfr_set(y,r,rnd_mode);
     mpfr_clear(r);
   }

DYADIC compute_exp(const DYADIC &x, const int &p) { return approx(exp(REAL(x)),p); }
void mpfr_ext_exp (mpfr_ptr r, mpfr_srcptr u,int p){mpfr_wrapper(compute_exp,r,u,p);}

DYADIC compute_log(const DYADIC &x, const int &p) { return approx(log(REAL(x)),p); }
void mpfr_ext_log (mpfr_ptr r, mpfr_srcptr u,int p){mpfr_wrapper(compute_log,r,u,p);}

DYADIC compute_sin(const DYADIC &x, const int &p) { return approx(sin(REAL(x)),p); }
void mpfr_ext_sin (mpfr_ptr r, mpfr_srcptr u,int p){mpfr_wrapper(compute_sin,r,u,p);}

DYADIC compute_cos(const DYADIC &x, const int &p) { return approx(cos(REAL(x)),p); }
void mpfr_ext_cos (mpfr_ptr r, mpfr_srcptr u,int p){mpfr_wrapper(compute_cos,r,u,p);}

DYADIC compute_tan(const DYADIC &x, const int &p) { return approx(tan(REAL(x)),p); }
void mpfr_ext_tan (mpfr_ptr r, mpfr_srcptr u,int p){mpfr_wrapper(compute_tan,r,u,p);}
