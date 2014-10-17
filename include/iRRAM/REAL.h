/*

iRRAM_REAL.h -- header file for the REAL class of the iRRAM library
 
Copyright (C) 2001-2013 Norbert Mueller
 
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

#ifndef iRRAM_REAL_H
#define iRRAM_REAL_H

#include <cmath>

#ifdef __SSE2__at_the_moment_not_used_due_to_alignment_problems_in_gcc_4_3
#include <emmintrin.h>
#define _use_SSE2_
#endif

//#include <cmath>

namespace iRRAM {

class iRRAM_double_pair{
	public: 
#ifdef _use_SSE2__
	union {struct{double lower_pos,upper_neg;}; __m128d sse_data;};
#else
	double lower_pos,upper_neg;
#endif
	iRRAM_double_pair(const double l,const double u)
		{lower_pos=l;upper_neg=u;};
	iRRAM_double_pair(){};
};


class REAL 
{
public:

// Constructors: -------------------------------

REAL();
REAL(const int  i);
REAL(const std::string s);
REAL(const char* s);
REAL(const DYADIC& y);
REAL(const REAL& y);
REAL(const double d);
REAL(const INTEGER& y);
REAL(const RATIONAL& y);

// Copy Constructor: ---------------------------

REAL& operator = (const REAL& y);

// Destructor: ---------------------------------

~REAL();

// Standard Arithmetic: ------------------------

friend REAL  operator +  (const REAL& x,	const REAL& y);
friend REAL  operator +  (const REAL& x,	const int n);
friend REAL  operator +  (const int n,		const REAL& x);
friend REAL  operator +  (const REAL& x,	const double y);
friend REAL  operator +  (const double y,	const REAL& x);
friend REAL& operator += (REAL& x,		const REAL& y);
			  
friend REAL  operator -  (const REAL& x, 	const REAL& y);
friend REAL  operator -  (const REAL& x,	const int n);
friend REAL  operator -  (const int n,		const REAL& x);
friend REAL  operator -  (const REAL& x,	const double y);
friend REAL  operator -  (const double y,	const REAL& x);
friend REAL  operator -  (const REAL& x);
friend REAL& operator -= (REAL& x,		const REAL& y);

friend REAL  operator *  (const REAL& x,	const REAL& y);
friend REAL  operator *  (const REAL& x, 	const int n);
friend REAL  operator *  (const int n,		const REAL& x);
friend REAL  operator *  (const REAL& x,	const double y);
friend REAL  operator *  (const double y,	const REAL& x);
friend REAL& operator *= (REAL& x,		const REAL& y);
			  
friend REAL  operator /  (const REAL& x,	const REAL& y);
friend REAL  operator /  (const REAL& x,	const int n);
friend REAL  operator /  (const int n,		const REAL& x);
friend REAL  operator /  (const REAL& x,	const double y);
friend REAL  operator /  (const double y,	const REAL& x);
friend REAL& operator /= (REAL& x,		const REAL& y);

friend REAL  operator << (const REAL& x,	const int n);
friend REAL  operator >> (const REAL& x,	const int n);


friend REAL  sqrt        (const REAL& x);

friend REAL  square      (const REAL& x);

friend REAL  scale       (const REAL& x,
                          const int k);

// Comparisons: --------------------------------

friend LAZY_BOOLEAN  operator <  (const REAL&    x, const REAL&    y);
friend LAZY_BOOLEAN  operator <  (const REAL&    x, const int      y);
friend LAZY_BOOLEAN  operator <  (const REAL&    x, const double   y);
friend LAZY_BOOLEAN  operator <  (const REAL&    x, const INTEGER& y);
friend LAZY_BOOLEAN  operator <  (const REAL&    x, const DYADIC&  y);
friend LAZY_BOOLEAN  operator <  (const int      x, const REAL&    y);
friend LAZY_BOOLEAN  operator <  (const double   x, const REAL&    y);
friend LAZY_BOOLEAN  operator <  (const INTEGER& x, const REAL&    y);
friend LAZY_BOOLEAN  operator <  (const DYADIC&  x, const REAL&    y);

friend LAZY_BOOLEAN  operator <= (const REAL&    x, const REAL&    y);
friend LAZY_BOOLEAN  operator <= (const REAL&    x, const int      y);
friend LAZY_BOOLEAN  operator <= (const REAL&    x, const double   y);
friend LAZY_BOOLEAN  operator <= (const REAL&    x, const INTEGER& y);
friend LAZY_BOOLEAN  operator <= (const REAL&    x, const DYADIC&  y);
friend LAZY_BOOLEAN  operator <= (const int      x, const REAL&    y);
friend LAZY_BOOLEAN  operator <= (const double   x, const REAL&    y);
friend LAZY_BOOLEAN  operator <= (const INTEGER& x, const REAL&    y);
friend LAZY_BOOLEAN  operator <= (const DYADIC&  x, const REAL&    y);

friend LAZY_BOOLEAN  operator >  (const REAL&    x, const REAL&    y);
friend LAZY_BOOLEAN  operator >  (const REAL&    x, const int      y);
friend LAZY_BOOLEAN  operator >  (const REAL&    x, const double   y);
friend LAZY_BOOLEAN  operator >  (const REAL&    x, const INTEGER& y);
friend LAZY_BOOLEAN  operator >  (const REAL&    x, const DYADIC&  y);
friend LAZY_BOOLEAN  operator >  (const int      x, const REAL&    y);
friend LAZY_BOOLEAN  operator >  (const double   x, const REAL&    y);
friend LAZY_BOOLEAN  operator >  (const INTEGER& x, const REAL&    y);
friend LAZY_BOOLEAN  operator >  (const DYADIC&  x, const REAL&    y);

friend LAZY_BOOLEAN  operator >= (const REAL&    x, const REAL&    y);
friend LAZY_BOOLEAN  operator >= (const REAL&    x, const int      y);
friend LAZY_BOOLEAN  operator >= (const REAL&    x, const double   y);
friend LAZY_BOOLEAN  operator >= (const REAL&    x, const INTEGER& y);
friend LAZY_BOOLEAN  operator >= (const REAL&    x, const DYADIC&  y);
friend LAZY_BOOLEAN  operator >= (const int      x, const REAL&    y);
friend LAZY_BOOLEAN  operator >= (const double   x, const REAL&    y);
friend LAZY_BOOLEAN  operator >= (const INTEGER& x, const REAL&    y);
friend LAZY_BOOLEAN  operator >= (const DYADIC&  x, const REAL&    y);

friend LAZY_BOOLEAN  operator == (const REAL&    x, const REAL&    y);
friend LAZY_BOOLEAN  operator == (const REAL&    x, const int      y);
friend LAZY_BOOLEAN  operator == (const REAL&    x, const double   y);
friend LAZY_BOOLEAN  operator == (const REAL&    x, const INTEGER& y);
friend LAZY_BOOLEAN  operator == (const REAL&    x, const DYADIC&  y);
friend LAZY_BOOLEAN  operator == (const int      x, const REAL&    y);
friend LAZY_BOOLEAN  operator == (const double   x, const REAL&    y);
friend LAZY_BOOLEAN  operator == (const INTEGER& x, const REAL&    y);
friend LAZY_BOOLEAN  operator == (const DYADIC&  x, const REAL&    y);

friend LAZY_BOOLEAN  operator != (const REAL&    x, const REAL&    y);
friend LAZY_BOOLEAN  operator != (const REAL&    x, const int      y);
friend LAZY_BOOLEAN  operator != (const REAL&    x, const double   y);
friend LAZY_BOOLEAN  operator != (const REAL&    x, const INTEGER& y);
friend LAZY_BOOLEAN  operator != (const REAL&    x, const DYADIC&  y);
friend LAZY_BOOLEAN  operator != (const int      x, const REAL&    y);
friend LAZY_BOOLEAN  operator != (const double   x, const REAL&    y);
friend LAZY_BOOLEAN  operator != (const INTEGER& x, const REAL&    y);
friend LAZY_BOOLEAN  operator != (const DYADIC&  x, const REAL&    y);


friend LAZY_BOOLEAN  positive    (const REAL& x, const int k);

friend LAZY_BOOLEAN  bound       (const REAL& x, const int k);

friend DYADIC approx     (const REAL& x, const int p);
DYADIC as_DYADIC (const int p) const;
DYADIC as_DYADIC () const;

double as_double (const int p) const;
double as_double () const { return this->as_double(53); };

INTEGER as_INTEGER() const;
friend REAL   round2     (const REAL& x); 
friend int    round      (const REAL& x); 


#define iRRAM_float_absolute 0
#define iRRAM_float_relative 1
#define iRRAM_float_show 2
friend std::string swrite     (const REAL& x, const int p, const int form);

// Output: -------------------------------------

friend void   rwrite     (const REAL& x, const int w);
friend void   rwritee    (const REAL& x, const int w);
friend void   rshow      (const REAL& x, const int w);

void rcheck(int n=50) const;

friend int    upperbound (const REAL& x);
friend int    size       (const REAL& x); 

friend REAL   abs        (const REAL& x);
 

// limit operators: ------------------------

//friend REAL    limit      (REAL f(int, const REAL&),
//                           const REAL& x);

friend REAL    limit      (REAL f(int, const REAL&, const REAL&),
                           const REAL& x, 
                           const REAL& y);

friend REAL    limit      (REAL f(int));

friend REAL    limit_hint (REAL f(int, const REAL&),
                           int hint,
                           const REAL& x);

friend REAL    limit_hint (REAL f(int, const REAL&, const REAL&),
                           int hint,
                           const REAL& x, 
                           const REAL& y);

friend REAL    limit_hint (REAL f(int),
                           int hint);

friend REAL    limit_lip  (REAL f(int, const REAL&),
                           int lip,
                           bool on_domain(const REAL&),
                           const REAL& x);

friend REAL    limit_lip  (REAL f(int, const REAL&),
                           int lip(const REAL&),
                           const REAL& x);

friend REAL    limit_lip   (REAL f(int, const REAL&, const REAL&),
                           int lip,
                           bool on_domain(const REAL&,const REAL&),
                           const REAL& x,
                           const REAL& y);

friend REAL    iterate     (REAL (*)(REAL&, REAL&, const REAL&),
                           const REAL&, const REAL&, const REAL&);

friend REAL    iterate     (REAL (*)(REAL&, REAL&),
                           const REAL&, const REAL&);

friend REAL    iteration   (void (*)(REAL&, REAL&,const int&),
                           const REAL&, const REAL&,const int&);

// reduced error propagation: ------------------------

friend REAL    lipschitz  (REAL f(const REAL&),
                           int lip,
                           bool on_domain(const REAL&),
                           const REAL& x);

friend REAL    lipschitz  (REAL f(const REAL&),
                           REAL lip_f(const REAL&),
                           bool on_domain(const REAL&),
                           const REAL& x);

friend REAL    lipschitz  (REAL f(const REAL&,const REAL&),
                           int lip,
                           bool on_domain(const REAL&,const REAL&),
                           const REAL& x,
                           const REAL& y);

friend REAL    lipschitz  (REAL f(int, const REAL&),
                           int lip,
                           bool on_domain(int k,const REAL&),
                           int k,
                           const REAL& x);

friend REAL    lipschitz  (REAL f(int, const REAL&,const REAL&),
                           int lip,
                           bool on_domain(int k,const REAL&,const REAL&),
                           int k,
                           const REAL& x,
                           const REAL& y);

friend REAL    lipschitz (REAL f(const REAL&),
                          REAL lip_f(const REAL&),
                          const REAL& x);

// coexistence with other classes: -------------

friend class REALMATRIX;
friend class SPARSEREALMATRIX;
friend class INTEGER;
friend class DYADIC;
friend class RATIONAL;

// for the sake of proving computational adequacy:
// if q=module(f,x,p), then |z-x|<2^q => |f(z)-f(x)| < 2^p
friend int module(REAL f(const REAL&),const REAL& x, int p);


// implementational issues: --------------------

iRRAM_double_pair dp;
MP_type       value; 
sizetype      error;
sizetype      vsize;

public:
REAL(MP_type y, const sizetype errorinfo);
REAL(const iRRAM_double_pair& ydp);
#ifdef _use_SSE2__
REAL(const __m128d& y_sse);
#endif
void adderror (sizetype error);
void seterror (sizetype error);
void geterror (sizetype& error) const;
void getsize  (sizetype& error) const;
void to_formal_ball (DYADIC&,sizetype& error) const;
friend REAL intervall_join (const REAL& x,const REAL& y);

// internal use:
void mp_copy(const REAL&);
void mp_copy_init(const REAL&);
void mp_make_mp();
void mp_from_mp(const REAL& y);
void mp_from_int(const int i);
void mp_from_double(const double d);
REAL& mp_conv ()const;
REAL mp_addition (const REAL& y)const;
REAL mp_addition (const int i)const;
REAL& mp_eqaddition (const REAL& y);
REAL mp_addition (const double i)const;//fehlt noch
REAL mp_subtraction (const REAL& y)const;
REAL mp_subtraction (const int i)const;
REAL mp_invsubtraction (const int i)const;
REAL mp_multiplication (const REAL& y)const;
REAL mp_multiplication (const int y)const;
REAL& mp_eqmultiplication (const REAL& y);
REAL& mp_eqmultiplication (const int i);
REAL mp_multiplication (const double y)const; //fehlt noch
REAL mp_division (const REAL& y)const;
REAL mp_division (const int y)const;
REAL mp_division (const double y)const;
REAL mp_square()const;
REAL mp_absval()const;
REAL mp_intervall_join (const REAL& y)const;
LAZY_BOOLEAN mp_less (const REAL& y)const;

};

std::string swrite(const REAL& x, const int p, const int form=iRRAM_float_absolute);

#define USE_HIGH_LEVEL iRRAM_unlikely(iRRAM_highlevel)
// inlined versions of most important functions:

//"private" internal  constructor
inline REAL::REAL(MP_type y,const sizetype errorinfo) 
{
    value=y;
    error=errorinfo;
    MP_getsize(value,vsize);
}

//"private" internal  constructor
inline REAL::REAL(const iRRAM_double_pair& ydp)
{
    value=NULL;
    dp = ydp;
}

#ifdef _use_SSE2__
inline REAL::REAL(const __m128d& y_sse)
{
    value=NULL;
    dp.sse_data=y_sse;
}
#endif

inline REAL::~REAL() 
{ 
    if ( iRRAM_unlikely(value) ) { MP_clear(value); value=NULL;}
}

inline REAL::REAL()
{
    value=NULL;
    dp.lower_pos=0.0;
    dp.upper_neg=0.0;
    if ( USE_HIGH_LEVEL ) mp_from_int(0);
}

inline REAL::REAL(const int i)
{
    value=NULL;
    dp.lower_pos=i;
    dp.upper_neg=-dp.lower_pos;
    if ( USE_HIGH_LEVEL ) mp_from_int(i);
}

inline REAL::REAL(const double d)
{
    if ( !std::isfinite(d) ) throw iRRAM_Numerical_Exception(iRRAM_conversion_from_infinite);
    value=NULL;
    dp.lower_pos=d;
    dp.upper_neg=-dp.lower_pos;
    if ( USE_HIGH_LEVEL ) mp_from_double(d);
}

inline REAL::REAL(const REAL& y){
    value=NULL;
    dp=y.dp;
    if ( iRRAM_unlikely(y.value) ) mp_copy_init(y);
}

inline REAL& REAL::mp_conv()const{
    if (! value) (const_cast<REAL&>(*this)).mp_make_mp();
    return const_cast<REAL&>(*this);
}

inline REAL& REAL::operator = (const REAL& y) {
    if ( iRRAM_unlikely(value||y.value) ){
	if ( value&&y.value ){
		this->mp_copy(y);
		return (*this);
	}
	if ( y.value ){
		if (ACTUAL_STACK.prec_step==0) this->mp_from_mp(y);
		else this->mp_copy_init(y);
		return (*this);
	}
	dp=y.dp;
	mp_make_mp();
	return (*this);
    }
    dp=y.dp;
    return (*this);
}

inline REAL operator << (const REAL& x, const int n) {
    return scale(x,n);
}

inline REAL operator >> (const REAL& x, const int n) {
    return scale(x,-n);
}

inline REAL operator + (const REAL& x, const REAL& y)
{
    if ( iRRAM_unlikely ( x.value||y.value ) )
	 { return x.mp_conv().mp_addition(y.mp_conv()); }
#ifdef _use_SSE2__
    return REAL(_mm_add_pd(x.dp.sse_data,y.dp.sse_data));
#else
    return REAL(iRRAM_double_pair(x.dp.lower_pos+y.dp.lower_pos,x.dp.upper_neg+y.dp.upper_neg));
#endif
}

inline REAL operator + (const REAL& x, const int i)
{
    if (iRRAM_unlikely( x.value ) )
	{ return x.mp_addition(i); }
    iRRAM_double_pair z(x.dp.lower_pos+i,x.dp.upper_neg-i);
    return REAL(z);
}

inline REAL operator + (const REAL& x, const double d)
{
    if (iRRAM_unlikely( x.value ) )
	{ return x.mp_addition(d); }
    iRRAM_double_pair z(x.dp.lower_pos+d,x.dp.upper_neg-d);
    return REAL(z);
}

inline REAL& operator += (REAL& x,const REAL& y)
{
    if ( iRRAM_unlikely ( x.value||y.value ) )
	 { x.mp_conv().mp_eqaddition(y.mp_conv()); return x;}
#ifdef _use_SSE2__
    x.dp.sse_data = _mm_add_pd(x.dp.sse_data,y.dp.sse_data);
#else
    x.dp.lower_pos+=y.dp.lower_pos;
    x.dp.upper_neg+=y.dp.upper_neg;
#endif
    return x;
}

inline REAL operator + (const int i, const REAL& x)
{
  return (x+i);
}

inline REAL operator + (const double d, const REAL& x)
{
  return (x+d);
}

inline REAL operator - (const REAL& x, const REAL& y)
{
    if ( iRRAM_unlikely ( x.value||y.value ) )
	 { return x.mp_conv().mp_subtraction(y.mp_conv()); }
    iRRAM_double_pair z(x.dp.lower_pos+y.dp.upper_neg,x.dp.upper_neg+y.dp.lower_pos);
    return REAL(z);
}

inline REAL operator - (const REAL& x, const int n)
{
    if ( iRRAM_unlikely ( x.value ) )
	 { return x.mp_subtraction(n); }
    iRRAM_double_pair z(x.dp.lower_pos-n,x.dp.upper_neg+n);
    return REAL(z);
}

inline REAL operator - (const int n, const REAL& x)
{
    if ( iRRAM_unlikely ( x.value ) )
	 { return x.mp_invsubtraction(n); }
    iRRAM_double_pair z(x.dp.upper_neg+n,x.dp.lower_pos-n);
    return REAL(z);
}

inline REAL operator - (const REAL& x)
{
    if ( iRRAM_unlikely ( x.value ) )
	{ return x.mp_invsubtraction(int(0)); }
    iRRAM_double_pair z(-x.dp.lower_pos,-x.dp.upper_neg);
    return REAL(z);
}

//not optimized yet, maybe MPFR contains "x-d"
inline REAL operator - (const REAL& x, const double d)
{
    return x - REAL(d);
}

//not optimized yet, maybe MPFR contains "d-x"
inline REAL operator - (const double d, const REAL& x)
{
    return REAL(d) - x;
}

inline REAL& operator -= (REAL& x, const REAL& y){
  x=x-y;
  return x;
}


// inline double my_fmin(const double& x,const double& y)
// { return  x<y?x:y ; }

inline REAL operator * (const REAL& x, const REAL& y) 
{
    if ( iRRAM_unlikely ( x.value||y.value ) )
	 { return x.mp_conv().mp_multiplication(y.mp_conv()); }
    iRRAM_double_pair z;
    if (x.dp.lower_pos >= 0 && y.dp.lower_pos >= 0) {
      z.lower_pos = x.dp.lower_pos * y.dp.lower_pos;
      z.upper_neg = (-x.dp.upper_neg) * y.dp.upper_neg;
    } else if (x.dp.upper_neg >= 0 && y.dp.upper_neg >= 0) {
      z.lower_pos = x.dp.upper_neg * y.dp.upper_neg;
      z.upper_neg = (-x.dp.lower_pos) * y.dp.lower_pos;
    } else if (x.dp.upper_neg >= 0 && y.dp.lower_pos >= 0) {
      z.lower_pos = x.dp.lower_pos * (-y.dp.upper_neg);
      z.upper_neg = x.dp.upper_neg * y.dp.lower_pos;
    } else if (x.dp.lower_pos >= 0 && y.dp.upper_neg >= 0 ) {
      z.lower_pos = (-x.dp.upper_neg) * y.dp.lower_pos;
      z.upper_neg = x.dp.lower_pos * y.dp.upper_neg;
    } else {
      z.lower_pos = fmin((-x.dp.upper_neg)*y.dp.lower_pos,x.dp.lower_pos*(-y.dp.upper_neg));
      z.upper_neg = fmin((-x.dp.upper_neg)*y.dp.upper_neg,x.dp.lower_pos*(-y.dp.lower_pos));
    }
    return REAL(z);
}

inline REAL operator * (const REAL& x, const int n) 
{
    if ( iRRAM_unlikely ( x.value) )
	 { return x.mp_multiplication(n); }
    iRRAM_double_pair z;
    if ( n >= 0) {
      z.lower_pos = x.dp.lower_pos * n;
      z.upper_neg = x.dp.upper_neg * n;
    } else {
      z.lower_pos = (-x.dp.upper_neg) * n;
      z.upper_neg = (-x.dp.lower_pos) * n;
    }
    return REAL(z);
}


inline REAL operator * (const int n, const REAL& x){
  return (x*n);
}

inline REAL& operator *= (REAL& x, const REAL& y){
  x=x*y;
  return x;
}

//not optimized yet, maybe MPFR contains "x*d"
inline REAL operator * (const REAL& x, const double d)
{
    return x * REAL(d);
}

inline REAL operator * (const double d, const REAL& x)
{
    return x * d;
}

inline REAL& operator *= (REAL& x, const int n){
    if ( iRRAM_unlikely ( x.value) )
	 { x=x.mp_multiplication(n); return x;}
    if ( n >= 0) {
      x.dp.lower_pos = x.dp.lower_pos * n;
      x.dp.upper_neg = x.dp.upper_neg * n;
    } else {
      double  tmp    = (-x.dp.upper_neg) * n;
      x.dp.upper_neg = (-x.dp.lower_pos) * n;
      x.dp.lower_pos = tmp;
    }
    return x;
}

inline REAL operator / (const REAL& x, const REAL& y) {
    if ( iRRAM_unlikely ( x.value||y.value ) )
	 { return x.mp_conv().mp_division(y.mp_conv()); }
    iRRAM_double_pair z;
    if (y.dp.lower_pos > 0.0 ) {
      if (x.dp.lower_pos > 0.0 ) {
        z.lower_pos  = x.dp.lower_pos/(-y.dp.upper_neg);
	z.upper_neg = x.dp.upper_neg/y.dp.lower_pos;
      } else if (x.dp.upper_neg  > 0.0){
        z.lower_pos  = x.dp.lower_pos/y.dp.lower_pos;
	z.upper_neg = x.dp.upper_neg/(-y.dp.upper_neg);
      } else {
        z.lower_pos  = x.dp.lower_pos/y.dp.lower_pos;
	z.upper_neg = x.dp.upper_neg/y.dp.lower_pos;
      }
    } else if (y.dp.upper_neg > 0.0) {
      if (x.dp.lower_pos > 0.0 ) {
        z.lower_pos  = x.dp.upper_neg/y.dp.upper_neg;
	z.upper_neg = (-x.dp.lower_pos)/y.dp.lower_pos;
      } else if (x.dp.upper_neg  > 0.0){
        z.lower_pos  = (-x.dp.upper_neg)/y.dp.lower_pos;
	z.upper_neg = x.dp.lower_pos/y.dp.upper_neg;
      } else {
        z.lower_pos  = x.dp.upper_neg/y.dp.upper_neg;
	z.upper_neg = x.dp.lower_pos/y.dp.upper_neg;
      }  
    } else {
        return x.mp_conv().mp_division(y.mp_conv()); // containing zero...
      }
    return REAL(z);
}

inline REAL operator / (const int n, const REAL& y) {
  return REAL(n)/y;
}
inline REAL operator / (const REAL& x, const int n) {
    if ( iRRAM_unlikely ( x.value ) )
	 { return x.mp_division(n); }
    iRRAM_double_pair z;
    if (n > 0 ) {
#ifdef _use_SSE2__
        __m128d n_sse= _mm_set_pd1(double(n));
        n_sse=_mm_div_pd(x.dp.sse_data,n_sse);
	return REAL(n_sse);
#else
        z.lower_pos = x.dp.lower_pos/n,
	z.upper_neg = x.dp.upper_neg/n;
#endif
    } else if (n < 0) {
        z.lower_pos = (-x.dp.upper_neg)/n;
	z.upper_neg = (-x.dp.lower_pos)/n;
    } else {
        return x.mp_conv().mp_division(0); // containing zero...
    }
    return REAL(z);
}


//not optimized yet, maybe MPFR contains "x/d"
inline REAL operator / (const REAL& x, const double d)
{
    return x / REAL(d);
}

//not optimized yet, maybe MPFR contains "d/x"
inline REAL operator / (const double d, const REAL& x)
{
    return REAL(d) / x;
}

inline REAL& operator /= (REAL& x, const REAL& y){
  x=x/y;
  return x;
}

inline REAL& operator /= (REAL& x, const int n){
  x=x/n;
  return x;
}

inline REAL square (const REAL& x) 
{
    if ( iRRAM_unlikely ( x.value ) )
	 { return x.mp_square(); }
    iRRAM_double_pair z;
    if (x.dp.lower_pos >= 0 ) {
      z.lower_pos = x.dp.lower_pos * x.dp.lower_pos;
      z.upper_neg = (-x.dp.upper_neg) * x.dp.upper_neg;
    } else if (x.dp.upper_neg >= 0 ) {
      z.lower_pos = x.dp.upper_neg * x.dp.upper_neg;
      z.upper_neg = (-x.dp.lower_pos) * x.dp.lower_pos;
    } else if (x.dp.lower_pos<x.dp.upper_neg){
      z.lower_pos = 0.0;
      z.upper_neg = x.dp.lower_pos*(-x.dp.lower_pos);
    } else {
      z.lower_pos = 0.0;
      z.upper_neg = (-x.dp.upper_neg)*x.dp.upper_neg;
    }
    return REAL(z);
}

inline LAZY_BOOLEAN operator < (const REAL& x, const REAL& y) {
    if ( iRRAM_unlikely ( x.value||y.value ) )
	 { return x.mp_conv().mp_less(y.mp_conv()); }
    if ((-x.dp.upper_neg) < y.dp.lower_pos) return true;
    if (x.dp.lower_pos > (-y.dp.upper_neg)) return false;
    return LAZY_BOOLEAN::BOTTOM;
}


inline LAZY_BOOLEAN operator <  (const REAL&	 x, const int	   y){ return  (x<REAL(y)); }
inline LAZY_BOOLEAN operator <  (const REAL&	 x, const double   y){ return  (x<REAL(y)); }
inline LAZY_BOOLEAN operator <  (const REAL&	 x, const INTEGER& y){ return  (x<REAL(y)); }
inline LAZY_BOOLEAN operator <  (const REAL&	 x, const DYADIC&  y){ return  (x<REAL(y)); }
inline LAZY_BOOLEAN operator <  (const int	 x, const REAL&    y){ return  (REAL(x)<y); }
inline LAZY_BOOLEAN operator <  (const double   x, const REAL&    y){ return  (REAL(x)<y); }
inline LAZY_BOOLEAN operator <  (const INTEGER& x, const REAL&    y){ return  (REAL(x)<y); }
inline LAZY_BOOLEAN operator <  (const DYADIC&  x, const REAL&    y){ return  (REAL(x)<y); }

inline LAZY_BOOLEAN operator <= (const REAL&    x, const REAL&    y){ return  (x<y); }
inline LAZY_BOOLEAN operator <= (const REAL&	 x, const int	   y){ return  (x<=REAL(y)); }
inline LAZY_BOOLEAN operator <= (const REAL&	 x, const double   y){ return  (x<=REAL(y)); }
inline LAZY_BOOLEAN operator <= (const REAL&	 x, const INTEGER& y){ return  (x<=REAL(y)); }
inline LAZY_BOOLEAN operator <= (const REAL&	 x, const DYADIC&  y){ return  (x<=REAL(y)); }
inline LAZY_BOOLEAN operator <= (const int	 x, const REAL&    y){ return  (REAL(x)<=y); }
inline LAZY_BOOLEAN operator <= (const double   x, const REAL&    y){ return  (REAL(x)<=y); }
inline LAZY_BOOLEAN operator <= (const INTEGER& x, const REAL&    y){ return  (REAL(x)<=y); }
inline LAZY_BOOLEAN operator <= (const DYADIC&  x, const REAL&    y){ return  (REAL(x)<=y); }

inline LAZY_BOOLEAN operator >  (const REAL&    x, const REAL&    y){ return  (y<x); }
inline LAZY_BOOLEAN operator >  (const REAL&	 x, const int	   y){ return  (x>REAL(y)); }
inline LAZY_BOOLEAN operator >  (const REAL&	 x, const double   y){ return  (x>REAL(y)); }
inline LAZY_BOOLEAN operator >  (const REAL&	 x, const INTEGER& y){ return  (x>REAL(y)); }
inline LAZY_BOOLEAN operator >  (const REAL&	 x, const DYADIC&  y){ return  (x>REAL(y)); }
inline LAZY_BOOLEAN operator >  (const int	 x, const REAL&    y){ return  (REAL(x)>y); }
inline LAZY_BOOLEAN operator >  (const double   x, const REAL&    y){ return  (REAL(x)>y); }
inline LAZY_BOOLEAN operator >  (const INTEGER& x, const REAL&    y){ return  (REAL(x)>y); }
inline LAZY_BOOLEAN operator >  (const DYADIC&  x, const REAL&    y){ return  (REAL(x)>y); }

inline LAZY_BOOLEAN operator >= (const REAL&    x, const REAL&    y){ return  (y<x); }
inline LAZY_BOOLEAN operator >= (const REAL&	 x, const int	   y){ return  (x>=REAL(y)); }
inline LAZY_BOOLEAN operator >= (const REAL&	 x, const double   y){ return  (x>=REAL(y)); }
inline LAZY_BOOLEAN operator >= (const REAL&	 x, const INTEGER& y){ return  (x>=REAL(y)); }
inline LAZY_BOOLEAN operator >= (const REAL&	 x, const DYADIC&  y){ return  (x>=REAL(y)); }
inline LAZY_BOOLEAN operator >= (const int	 x, const REAL&    y){ return  (REAL(x)>=y); }
inline LAZY_BOOLEAN operator >= (const double   x, const REAL&    y){ return  (REAL(x)>=y); }
inline LAZY_BOOLEAN operator >= (const INTEGER& x, const REAL&    y){ return  (REAL(x)>=y); }
inline LAZY_BOOLEAN operator >= (const DYADIC&  x, const REAL&    y){ return  (REAL(x)>=y); }

inline LAZY_BOOLEAN operator == (const REAL&    x, const REAL&    y){ return  (y<x)&&(x<y) ; }
inline LAZY_BOOLEAN operator == (const REAL&	 x, const int	   y){ return  (x==REAL(y)); }
inline LAZY_BOOLEAN operator == (const REAL&	 x, const double   y){ return  (x==REAL(y)); }
inline LAZY_BOOLEAN operator == (const REAL&	 x, const INTEGER& y){ return  (x==REAL(y)); }
inline LAZY_BOOLEAN operator == (const REAL&	 x, const DYADIC&  y){ return  (x==REAL(y)); }
inline LAZY_BOOLEAN operator == (const int	 x, const REAL&    y){ return  (REAL(x)==y); }
inline LAZY_BOOLEAN operator == (const double   x, const REAL&    y){ return  (REAL(x)==y); }
inline LAZY_BOOLEAN operator == (const INTEGER& x, const REAL&    y){ return  (REAL(x)==y); }
inline LAZY_BOOLEAN operator == (const DYADIC&  x, const REAL&    y){ return  (REAL(x)==y); }

inline LAZY_BOOLEAN operator != (const REAL&    x, const REAL&    y){ return  (y<x)||(x<y) ;}
inline LAZY_BOOLEAN operator != (const REAL&	 x, const int	   y){ return  (x!=REAL(y)); }
inline LAZY_BOOLEAN operator != (const REAL&	 x, const double   y){ return  (x!=REAL(y)); }
inline LAZY_BOOLEAN operator != (const REAL&	 x, const INTEGER& y){ return  (x!=REAL(y)); }
inline LAZY_BOOLEAN operator != (const REAL&	 x, const DYADIC&  y){ return  (x!=REAL(y)); }
inline LAZY_BOOLEAN operator != (const int	 x, const REAL&    y){ return  (REAL(x)!=y); }
inline LAZY_BOOLEAN operator != (const double   x, const REAL&    y){ return  (REAL(x)!=y); }
inline LAZY_BOOLEAN operator != (const INTEGER& x, const REAL&    y){ return  (REAL(x)!=y); }
inline LAZY_BOOLEAN operator != (const DYADIC&  x, const REAL&    y){ return  (REAL(x)!=y); }

inline REAL abs (const REAL& x){
    if ( iRRAM_unlikely ( x.value ) )
	 { return x.mp_absval(); }
    iRRAM_double_pair z;
    if (x.dp.lower_pos > 0.0 )
		return REAL(iRRAM_double_pair(x.dp.lower_pos,x.dp.upper_neg));
    if (x.dp.upper_neg > 0.0 )
		return REAL(iRRAM_double_pair(x.dp.upper_neg,x.dp.lower_pos));
    if (x.dp.lower_pos > x.dp.upper_neg)
		return REAL(iRRAM_double_pair(0.0,x.dp.upper_neg));
    return REAL(iRRAM_double_pair(0.0,x.dp.lower_pos));
}

// inline REAL intervall_join (const REAL& x,const REAL& y){
//     if ( iRRAM_unlikely ( x.value||y.value ) )
// 	 { return x.mp_conv().mp_intervall_join(y.mp_conv()); }
//     iRRAM_double_pair z;
//     if (x.dp.lower_pos < y.dp.lower_pos )
//       z.dp.lower_pos=x.dp.lower_pos
//     else
//       z.dp.lower_pos=y.dp.lower_pos;
//     if (x.dp.upper_neg < y.dp.upper_neg )
//       z.dp.upper_neg=x.dp.upper_neg
//     else
//       z.dp.upper_neg=y.dp.upper_neg;
//     return REAL(z);
// }

} // namespace iRRAM

#endif
