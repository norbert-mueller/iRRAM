/*

REALS.cc -- main part of the implementation of reals for the iRRAM library
 
Copyright (C) 2001-2010 Norbert Mueller
 
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

#include <stdlib.h>
#include <stdarg.h>
#include <string.h>

#include "iRRAM/core.h"

#if ($iRRAM_BACKEND == MPFR)
	#include "MPFR_ext.h"
#else
	#include "Currently no further backends defined!"
#endif


namespace iRRAM {

void REAL::mp_make_mp(){
      if (!std::isfinite(dp.upper_neg) || !std::isfinite(dp.lower_pos)) { REITERATE(0); }
      if (dp.upper_neg== -dp.lower_pos) {
// here we have a point interval...
         if (!value) MP_init(value); 
         MP_double_to_mp(dp.lower_pos,value);
         sizetype_set(error,1,MP_min);
      } else {
// now we know that it is not a point interval:
      MP_init(value); MP_double_to_mp(dp.lower_pos,value);
      MP_type value1; MP_init(value1); MP_double_to_mp(-dp.upper_neg,value1);
      MP_type value2; MP_init(value2); MP_add(value,value1,value2,-1150);
      MP_shift(value2,value,-1);
      MP_clear (value1); MP_clear(value2);
// value0 differs from precise result (lower+upper)/2 by at most 2^{-1200}, 
// which is sufficiently more precise than all bits even of the
// smallest double number (2^-1075), so here the error is as small as reasonable
      double rwidth=(dp.upper_neg+dp.lower_pos);
      int e;
      unsigned int m=(unsigned int)(1073741824*frexp(-rwidth,&e))+2;
// Here, the "+2" accounts for the possible truncation error and the
// error on the computation of cvalue. The factor 1073741824 is 2^30
// So we have that the interval (dp.lower,dp.upper) is a subset
// of the interval (cvalue - m*2^(e+29),cvalue + m*2^(e+29) )
      sizetype_set(error,m,e-29);
    }
    MP_getsize(value,vsize);
}

void REAL::mp_from_mp(const REAL& y){
// d=MP_mp_to_double(y) is required to return a value d such that 
// there is no other double between d and x.
// the conversion should also generate infinities in case of overflow
// we don't require a directed rounding here!
      double center=MP_mp_to_double(y.value);
// in consequence, wd will be the negative of an upper bound
// for the width of the interval!
      double wd=ldexp(-double (y.error.mantissa),y.error.exponent);
      dp.upper_neg = nextafter(-center,-INFINITY)+wd;
      dp.lower_pos = nextafter(center,-INFINITY)+wd;
}

void REAL::mp_from_int(const int i){
  MP_init(value);
  MP_int_to_mp(i,value);
  sizetype_exact(error);
  MP_getsize(value,vsize);
};

void REAL::mp_from_double(const double d){
  MP_init(value);
  MP_double_to_mp(d,value);
  sizetype_exact(error);
  MP_getsize(value,vsize);
};

void REAL::mp_copy(const REAL& y){
	MP_duplicate_wo_init(y.value,value);
	error=y.error;
	vsize=y.vsize;
};

void REAL::mp_copy_init(const REAL& y){
	MP_duplicate_w_init(y.value,value);
	error=y.error;
	vsize=y.vsize;
};

REAL REAL::mp_addition (const REAL& y)const
{
  MP_type zvalue; sizetype zerror; int local_prec;
  if (ACTUAL_STACK.prec_policy == 0)
  local_prec=max3(y.error.exponent,this->error.exponent,ACTUAL_STACK.actual_prec);
  else {
  local_prec=max(this->vsize.exponent,y.vsize.exponent);
  local_prec=max3(y.error.exponent,this->error.exponent,local_prec-50+ACTUAL_STACK.actual_prec);
  }
  MP_init(zvalue);
  MP_mv_add(this->value,y.value,zvalue,local_prec);

  sizetype_add_wo_norm(zerror,this->error,y.error);
  sizetype_inc_one(zerror,local_prec);

  return REAL(zvalue,zerror);
};

REAL REAL::mp_addition (const int n)const 
{
  MP_type zvalue; sizetype zerror; int local_prec=0;
  if (ACTUAL_STACK.prec_policy == 0) 
    local_prec=max(this->error.exponent,ACTUAL_STACK.actual_prec);
  else {
    sizetype ysize;
    sizetype_set(ysize,n>0?n:-n,0);
    local_prec=max(this->vsize.exponent,ysize.exponent);
    local_prec=max(this->error.exponent,local_prec-50+ACTUAL_STACK.actual_prec);
  }
  MP_init(zvalue);
  MP_mv_addi(this->value,n,zvalue,local_prec);
  sizetype_add_one(zerror,this->error,local_prec);
  return REAL(zvalue,zerror);
}

REAL& REAL::mp_eqaddition (const REAL& y){
  MP_type zvalue;
  int local_prec;
  if (ACTUAL_STACK.prec_policy == 0)
  local_prec=max3(y.error.exponent,this->error.exponent,ACTUAL_STACK.actual_prec);
  else {
  local_prec=max(this->vsize.exponent,y.vsize.exponent);
  local_prec=max3(y.error.exponent,this->error.exponent,local_prec-50+ACTUAL_STACK.actual_prec);
  }
  MP_init(zvalue);
  MP_mv_add(this->value,y.value,zvalue,local_prec);

  sizetype_inc(this->error,y.error);
  sizetype_inc_one(this->error,local_prec);

/*  sizetype_set(zerror,1,local_prec);
  sizetype_inc2(this->error,y.error,zerror);*/

  MP_clear(this->value);
  this->value=zvalue;
  MP_getsize(this->value,this->vsize);
  return (*this);
};

std::string swrite(const REAL& x, const int w, const int form)
{
  if ( !x.value ){
    REAL y(x);
    y.mp_make_mp();
    return swrite(y,w,form);
  }

  std::string result;
  if ( ACTUAL_STACK.inlimit==0 && iRRAM_thread_data_address->cache_s.get(result)) return result;

  int width=w;
  if (width<9) width=9;
  char* erg;

  if (form==iRRAM_float_absolute) {
    sizetype psize; 
    int p=-10*(width-8)/3;
    sizetype_set(psize,1,p);
    int s=MP_size(x.value);
    int mantissa=(int)((s-x.error.exponent-GUARD_BITS)*.30103); 
    if (sizetype_less(psize,x.error)|| (s>p && mantissa+8 < width) ) {
      DEBUG4(1,"insufficient precision %d*2^(%d) in conversion with precision 2^(%d)\n",
         x.error.mantissa,x.error.exponent,p);
      REITERATE(p-x.error.exponent);
    }

    if (s<=p) {
      erg=(char*) calloc(width+1,sizeof(char));
      strcpy(erg,"  0 ");
      for (int j=4;j<width;j++) erg[j]=' ';
      erg[width]=0;
    } else {
      erg=MP_swrite(x.value,width);
    }

  } else if (form==iRRAM_float_relative){
     continous_begin();

// We want to have an error less than 0.65 ulp, where 0.51 ulp come from
// the conversion to decimal, so we need to know that the error is at most
// 0.15ulp.
// At the time, we take the size of x....
// It would be better to take the x.size, subtract x.error and approximately
// convert the result to decimal in order to get a correct approximation
// for a lower bound of x. Using this lower bound we can estimate what
// 0.15ulp means in binary.
     int p=size(x)-2-10*(width-8)/3; 
//We have to prevent that "size" changes the multi-value-cache here! 
//Otherwise, any iteration would not use the same amount of 
// cache as the first run!
     continous_end();

     sizetype psize;
     sizetype_set(psize,1,p);
     int s=MP_size(x.value);
     int mantissa=(int)((s-x.error.exponent-GUARD_BITS)*.30103); 
     if (sizetype_less(psize,x.error)|| (s>p && mantissa+8 < width) ) {
       DEBUG4(1,"insufficient precision %d*2^(%d) in writing with precision 2^(%d)\n",
         x.error.mantissa,x.error.exponent,p);
       REITERATE(p-x.error.exponent);
     }
     erg=MP_swrite(x.value,width);
  
  
  } else if (form==iRRAM_float_show){

    int mantissa=(int)(int((MP_size(x.value)-x.error.exponent-GUARD_BITS))*.30103); 
    if (mantissa < 1 ) { 
      erg=(char*) calloc(width+1,sizeof(char));
      sprintf(erg," .*E%05.0f",1+(x.error.exponent+GUARD_BITS)*.30103);
      for (int j=9;j<width;j++) erg[j]=' ';
    } else { 
      if ( mantissa>width-8 ) mantissa=width-8;
      char * erg2=MP_swrite(x.value,mantissa+8);
      erg=(char*) calloc(width+1,sizeof(char));
      strncpy(erg,erg2,mantissa+8);
      for (int j=mantissa+8;j<width;j++) erg[j]=' ';
      erg[width]=0;
      free(erg2);
    }

  }
 
  result=erg;
  free(erg);
  if ( ACTUAL_STACK.inlimit==0 ) iRRAM_thread_data_address->cache_s.put(result);
  return result;
}

REAL REAL::mp_subtraction (const REAL& y)const {
  MP_type zvalue; sizetype zerror; int local_prec;
  if (ACTUAL_STACK.prec_policy == 0)
  local_prec=max3(y.error.exponent,this->error.exponent,ACTUAL_STACK.actual_prec);
  else
  {
  local_prec=max(this->vsize.exponent,y.vsize.exponent);
  local_prec=max3(y.error.exponent,this->error.exponent,local_prec-50+ACTUAL_STACK.actual_prec);
  }
  MP_init(zvalue);
  MP_mv_sub(this->value,y.value,zvalue,local_prec);

  sizetype_add_wo_norm(zerror,this->error,y.error);
  sizetype_inc_one(zerror,local_prec);
/*
  sizetype_set(zerror,1,local_prec);
  sizetype_inc2(zerror,this->error,y.error);
*/
  return REAL(zvalue,zerror);
};

REAL REAL::mp_subtraction (const int n)const
{
  MP_type zvalue; 
  sizetype zerror; 
  int local_prec;
  if (ACTUAL_STACK.prec_policy == 0)
    local_prec=max(this->error.exponent,ACTUAL_STACK.actual_prec);
  else {
    sizetype ysize;
    sizetype_set(ysize,n>0?n:-n,0);
    local_prec=max(this->vsize.exponent,ysize.exponent);
    local_prec=max(this->error.exponent,local_prec-50+ACTUAL_STACK.actual_prec);
  }
  MP_init(zvalue);
  MP_mv_subi(this->value,n,zvalue,local_prec);
  sizetype_add_one(zerror,this->error,local_prec);
  return REAL(zvalue,zerror);
};

REAL REAL::mp_invsubtraction (const int n)const
{
  	MP_type zvalue;
	sizetype zerror;
	int local_prec;
  	if (ACTUAL_STACK.prec_policy == 0)
  		local_prec=max(this->error.exponent,ACTUAL_STACK.actual_prec);
  	else
  		{
  		sizetype xsize,ysize;
  		MP_getsize(this->value,xsize);
		sizetype_set(ysize,n>0?n:-n,0);
		local_prec=max(xsize.exponent,ysize.exponent);
  		local_prec=max(this->error.exponent,local_prec-50+ACTUAL_STACK.actual_prec);
  		}
  	MP_init(zvalue);
  	MP_mv_isub(n,this->value,zvalue,local_prec);
  	sizetype_add_one(zerror,this->error,local_prec);
  	return REAL(zvalue,zerror);
};

REAL REAL::mp_multiplication (const REAL& y)const
{
  MP_type zvalue;
  sizetype zerror,proderror,sumerror;
  int local_prec;
  sizetype_mult(zerror,this->vsize,y.error);
  sizetype_add_wo_norm(sumerror,y.vsize,y.error);
  sizetype_mult(proderror,sumerror,this->error);
  sizetype_inc(zerror,proderror);
  if (ACTUAL_STACK.prec_policy == 0)
    local_prec=max(zerror.exponent,ACTUAL_STACK.actual_prec);
  else
    local_prec=max(zerror.exponent,this->vsize.exponent+y.vsize.exponent-50+ACTUAL_STACK.actual_prec);
  MP_init(zvalue);
  MP_mv_mul(this->value,y.value,zvalue,local_prec);
  sizetype_inc_one(zerror,local_prec);
  return REAL(zvalue,zerror);
};

REAL REAL::mp_multiplication (const int n)const
{
  	MP_type zvalue;
  	sizetype zerror,ysize;
  	int local_prec;
	sizetype_set(ysize,n>0?n:-n,0);
  	sizetype_mult(zerror,ysize,this->error);

  	if (ACTUAL_STACK.prec_policy == 0)
    		local_prec=max(zerror.exponent,ACTUAL_STACK.actual_prec);
  	else
    		local_prec=max(zerror.exponent,this->vsize.exponent+ysize.exponent-50+ACTUAL_STACK.actual_prec);
  	MP_init(zvalue);
  	MP_mv_muli(this->value,n,zvalue,local_prec);
  	sizetype_inc_one(zerror,local_prec);
  	return REAL(zvalue,zerror);
}

REAL REAL::mp_division (const REAL& y)const
{
  MP_type zvalue; 
  sizetype zerror,h1,h2,h3;
  int local_prec;
  sizetype_half(h1,y.vsize);
  if (sizetype_less(h1,y.error)) {
      DEBUG5(1,"insufficient precision %d*2^(%d) in denominator of size %d*2^(%d)\n",
              y.error.mantissa,y.error.exponent,y.vsize.mantissa,y.vsize.exponent);
      REITERATE(0);
    }
  sizetype_mult(h1,this->vsize,y.error);
  sizetype_mult(h2,y.vsize,this->error);
  sizetype_inc(h1,h2);
  h3=y.vsize;
  sizetype_dec(h3);
  sizetype_dec(h3,y.error);
  sizetype_mult(h2,h3,y.vsize);
  sizetype_div(zerror,h1,h2);
  if (ACTUAL_STACK.prec_policy == 0)
    local_prec=max(zerror.exponent,ACTUAL_STACK.actual_prec);
  else
    local_prec=max(zerror.exponent,this->vsize.exponent-y.vsize.exponent-50+ACTUAL_STACK.actual_prec);
  MP_init(zvalue);
  MP_mv_div(this->value,y.value,zvalue,local_prec);
  sizetype_inc_one(zerror,local_prec);
  return REAL(zvalue,zerror);
};

REAL REAL::mp_division (const int n)const
{
  	MP_type zvalue; 
  	sizetype zerror,ysize;
  	int local_prec;
	sizetype_set(ysize,n>0?n:-n,0);
  	sizetype_div(zerror,this->error,ysize);
  	if (ACTUAL_STACK.prec_policy == 0)
    		local_prec=max(zerror.exponent,ACTUAL_STACK.actual_prec);
  	else
    		local_prec=max(zerror.exponent,this->vsize.exponent-ysize.exponent-50+ACTUAL_STACK.actual_prec);
  	MP_init(zvalue);
  	MP_mv_divi(this->value,n,zvalue,local_prec);
  	sizetype_inc_one(zerror,local_prec);
  	return REAL(zvalue,zerror);
}


void   rwrite  (const REAL& x, const int w){ iRRAM::cout << swrite(x,w,iRRAM_float_absolute); }
void   rwritee (const REAL& x, const int w){ iRRAM::cout << swrite(x,w,iRRAM_float_relative); }
void   rshow   (const REAL& x, const int w){ iRRAM::cout << swrite(x,w,iRRAM_float_show); }



#define ABSOLUTE 0
#define RELATIVE 1
void precision_policy (int policy){ACTUAL_STACK.prec_policy=policy;};
void stiff_begin (){
	ACTUAL_STACK.prec_step++;
	ACTUAL_STACK.actual_prec=iRRAM_prec_array[ACTUAL_STACK.prec_step];
	iRRAM_highlevel = (ACTUAL_STACK.prec_step > 1);
	}
void stiff_end   (){
	ACTUAL_STACK.prec_step--;
	ACTUAL_STACK.actual_prec=iRRAM_prec_array[ACTUAL_STACK.prec_step];
	iRRAM_highlevel = (ACTUAL_STACK.prec_step > 1);
	}
void continous_begin (){ACTUAL_STACK.inlimit+=1;};
void continous_end   (){ACTUAL_STACK.inlimit-=1;};


REAL REAL::mp_square ()const
{
  MP_type zvalue;
  sizetype zerror,proderror;
  int local_prec;
  sizetype_mult(zerror,this->vsize,this->error);
  sizetype_mult(proderror,this->vsize,this->error);
  sizetype_inc(zerror,proderror);
  sizetype_mult(proderror,this->error,this->error);
  sizetype_inc(zerror,proderror);
  if (ACTUAL_STACK.prec_policy == 0)
    local_prec=max(zerror.exponent,ACTUAL_STACK.actual_prec);
  else
    local_prec=max(zerror.exponent,this->vsize.exponent+this->vsize.exponent-50+ACTUAL_STACK.actual_prec);
  MP_init(zvalue);
  MP_mv_mul(this->value,this->value,zvalue,local_prec);
  sizetype_inc_one(zerror,local_prec);
  return REAL(zvalue,zerror);
};

LAZY_BOOLEAN REAL::mp_less (const REAL& y)const
{
  REAL z=y-(*this);
  sizetype s;
  MP_getsize(z.value,s);
  if (sizetype_less(s,z.error) )
	{
	DEBUG5(1,"insufficient precisions %d*2^(%d) and %d*2^(%d) in comparing\n",
              this->error.mantissa,this->error.exponent,
              y.error.mantissa,y.error.exponent);
        return BOTTOM;
	}
  return  ((MP_sign((z.value))==1));
};

REAL REAL::mp_absval ()const
{
  MP_type zvalue;
  MP_init(zvalue);
  MP_abs(value,zvalue);
  return REAL(zvalue,error);
}

// REAL REAL::mp_interval_join (const REAL& y)const
// {
// /* The purpose of this routine is to compute interval hull (as a simplified interval) to 
//    the union of two intervals (where both are also given a simplified intervals).
// */
// 
// error("this is unfinished!")
// 
//   MP_type zvalue; sizetype zerror; int local_prec;
//   if (ACTUAL_STACK.prec_policy == 0)
//   local_prec=max3(y.error.exponent,this->error.exponent,ACTUAL_STACK.actual_prec);
//   else {
//   local_prec=max(this->vsize.exponent,y.vsize.exponent);
//   local_prec=max3(y.error.exponent,this->error.exponent,local_prec-50+ACTUAL_STACK.actual_prec);
//   }
//   MP_init(zvalue);
//   MP_mv_add(this->value,y.value,zvalue,local_prec);
// 
//   sizetype_add_wo_norm(zerror,this->error,y.error);
//   sizetype_inc_one(zerror,local_prec);
// 
//   return REAL(zvalue,zerror);
// };


/*****************************************************/

#ifdef MP_shift

REAL scale(const REAL& x, int n) {
 if (!x.value){REAL y(x);return scale(y.mp_conv(),n);}
  sizetype zerror;
  MP_type zvalue;
  MP_init(zvalue);
  MP_shift(x.value,zvalue,n);
  x.geterror(zerror);
  sizetype_shift(zerror,zerror,n);
  return REAL(zvalue,zerror);
}

#else

REAL scale(const REAL& x, int n) {
   if (n==0) return x;
   if (n==1) return 2*x;
   stiff_begin();
   REAL y=1;
   REAL xc=2;
   if (n<0) {xc=y/2;n=-n;}
   if (n==1) {
     stiff_end();
     return xc*x;
   }
   for (int k=n;k>0;k=k/2) { 
     if (k%2==1) y*=xc;
     if ( k ==1) break;
     xc*=xc;}
   stiff_end();
   return y*x; 
}

#endif


LAZY_BOOLEAN positive (const REAL& x, int k)
{
 if (!x.value){REAL y(x);return positive(y.mp_conv(),k);}
 bool erg;
  sizetype ksize;
  sizetype_set(ksize,1,k);
  if (sizetype_less(ksize,x.error) && sizetype_less(x.vsize,x.error) ) 
    {
      DEBUG3(1,"insufficient precision %d*2^(%d) in test on positive\n",
		x.error.mantissa,x.error.exponent);
      return BOTTOM;
    }
  erg=(MP_sign(x.value)==1);
  return erg; 
};

DYADIC approx (const REAL& x, const int p) {
 if (!x.value){REAL y(x);return approx(y.mp_conv(),p);}
  MP_type result;
  MP_type erg;
  if ((ACTUAL_STACK.inlimit==0) && iRRAM_thread_data_address->cache_mp.get(result)){
    MP_duplicate_w_init(result,erg);
    return DYADIC(erg);
  }

  sizetype psize;
  sizetype_set(psize,1,p+1);
  if (sizetype_less(psize,x.error) )
     {
       DEBUG4(1,"insufficient precision %d*2^(%d) in approx(%d)\n",
           x.error.mantissa,x.error.exponent,p);
       REITERATE(p-x.error.exponent);
     }
  MP_init(erg);
  MP_copy(x.value,erg,p-1);
  
  if ( ACTUAL_STACK.inlimit==0 ) {
    MP_duplicate_w_init(erg,result);
    iRRAM_thread_data_address->cache_mp.put(result);
  }
  return DYADIC(erg); 
};

int size (const REAL& x) {
 if (!x.value){REAL y(x);return size(y.mp_conv());}
  int result=0;
  if ((ACTUAL_STACK.inlimit==0) && iRRAM_thread_data_address->cache_i.get(result)) return result;
  sizetype xsize=x.vsize;
  sizetype ergsize;
  sizetype_add(ergsize,xsize,x.error);
  unsigned int value = ergsize.mantissa;
  for (unsigned int m=32>>1; m>0; m=m>>1 ) {
    if (value>= ( ((unsigned int) 1)<<m) ){
      result+=m; value=value>>m;
    }
  }
  if ( ((unsigned int) 1<< result) != ergsize.mantissa) result+=1;
  result+=ergsize.exponent;

  if ( result < MP_min) throw iRRAM_Numerical_Exception(iRRAM_underflow_error);

  sizetype_set(xsize,1,result-2);
  sizetype_inc(xsize,x.error);
  if (sizetype_less(x.vsize,xsize) )
	{  
	DEBUG5(1,"insufficient precision %d*2^(%d) in size %d*2^(%d)\n",
           x.error.mantissa,x.error.exponent,x.vsize.mantissa,x.vsize.exponent);
	REITERATE(0);
	}

  if ( ACTUAL_STACK.inlimit==0 ) iRRAM_thread_data_address->cache_i.put(result);
  return result;
};

int upperbound (const REAL& x) {
 if (!x.value){REAL y(x);return upperbound(y.mp_conv());}
  int result;
  sizetype ergsize;
  if ( (ACTUAL_STACK.inlimit==0) && iRRAM_thread_data_address->cache_i.get(result)) return result;
  sizetype_add(ergsize,x.vsize,x.error);
  while (ergsize.mantissa>(1<<16)) {
    ergsize.mantissa >>= 16;
    ergsize.exponent  += 16;
  }
  while (ergsize.mantissa>(1<<4)) {
    ergsize.mantissa >>= 4;
    ergsize.exponent  += 4;
  }
  while (ergsize.mantissa>1) {
    ergsize.mantissa >>= 1;
    ergsize.exponent  += 1;
  }
  result=ergsize.exponent;
  if (ACTUAL_STACK.inlimit==0 ) iRRAM_thread_data_address->cache_i.put(result);
  return result;
};

LAZY_BOOLEAN bound (const REAL& x, const int k) {
 if (!x.value){REAL y(x);return bound(y.mp_conv(),k);}
  sizetype lowsize,ksize,highsize;
  sizetype_set(ksize,1,k);
  sizetype_add_one(lowsize,x.error,k-1);
  sizetype_add(highsize,x.vsize,x.error);
  if (sizetype_less(x.vsize,lowsize) && sizetype_less(ksize,highsize) ) 
    {  
      DEBUG6(1,"insufficient precision %d*2^(%d) in bounding by 2^(%d) for argument of size  %d*2^(%d)\n",
              x.error.mantissa,x.error.exponent,k,x.vsize.mantissa,x.vsize.exponent);
      return BOTTOM;
    }
  return (sizetype_less(x.vsize,ksize) );
};



void REAL::rcheck(int n) const
{
  if ( !value ){
    cerr << "Value: (" 
		<< std::setprecision(n)<< std::setw(n)<<  dp.lower_pos << ";" 
		<< std::setprecision(n)<< std::setw(n)<< -dp.upper_neg << ")\n";
  } else {
    cerr << "Value: ";
    char* c=MP_swrite(value,n);
    cerr << c;
    free(c);
    cerr << ", Size:  "<<vsize.mantissa<<"*2^("<<vsize.exponent<<")\n\n";
    cerr << ", Error: "<<error.mantissa<<"*2^("<<error.exponent<<")\n\n";
  }
}

void REAL::adderror (sizetype nerror)
{
  if (!value){
     REAL y(*this);
     y.mp_conv().adderror(nerror);
     *this=y;
  } else {
    sizetype_inc((*this).error,nerror);
  }
}

void REAL::seterror (sizetype nerror)
{
  if (!value){
     REAL y(*this);
     y.mp_conv().seterror(nerror);
     *this=y;
  } else {
    (*this).error=nerror;
  }
}

void REAL::geterror (sizetype& nerror) const 
{
  if (!value){
     REAL y(*this);
     y.mp_conv().geterror(nerror);
  } else {
     nerror=(*this).error;
  }
}

void REAL::getsize (sizetype& nsize) const 
{
  if (!value){
     REAL y(*this);
     y.mp_conv().getsize(nsize);
  } else {
     nsize=(*this).vsize;
  }
}

void REAL::to_formal_ball (DYADIC& d,sizetype& nerror) const 
{
  if (!value){
     REAL y(*this);
     y.mp_conv().geterror(nerror);
     MP_duplicate_wo_init(y.value,d.value);
  } else {
     nerror=(*this).error;
     MP_duplicate_wo_init(this->value,d.value);
  }
}


REAL::REAL(const DYADIC& y){ 
  MP_duplicate_w_init(y.value,value);
  sizetype_exact(error);
  MP_getsize(value,vsize);
};

REAL::REAL(const std::string s){
  REAL result=atoREAL(s.c_str());
  error=result.error;
  MP_duplicate_w_init(result.value,value);
  MP_getsize(value,vsize);
};

REAL::REAL(const char* s){
  REAL result=atoREAL(s);
  error=result.error;
  MP_duplicate_w_init(result.value,value);
  MP_getsize(value,vsize);
};

INTEGER REAL::as_INTEGER() const {
  if (!this->value){
	return this->mp_conv().as_INTEGER();
  }
  MP_int_type result,value;
  if ( (ACTUAL_STACK.inlimit==0) && iRRAM_thread_data_address->cache_mpi.get(result)){
    MP_int_duplicate_w_init(result,value);
    return value;
  }

  sizetype psize;
  sizetype_set(psize,1,-4);
  REAL y=(*this);
  if (MP_sign(y.value)==1) y=y+REAL(0.5);
  else y=y-REAL(0.5);
  y.mp_conv();
  if (sizetype_less(psize,y.error) )
     {
       	DEBUG3(1,"insufficient precision %d*2^(%d) converting to integer\n",this->error.mantissa,this->error.exponent);
       	REITERATE(-y.error.exponent);
     }
  MP_int_init(value);
  MP_mp_to_INTEGER(y.value,value);

  if ( ACTUAL_STACK.inlimit==0 ) {
    MP_int_duplicate_w_init(value,result);
    iRRAM_thread_data_address->cache_mpi.put(result);
  }
  return value;
}

// conversion to type REAL from smaller types

REAL::REAL(const RATIONAL& r)
{
  INTEGER numi,deni;

  MP_rat_get_numerator(r.value,numi.value);
  MP_rat_get_denominator(r.value,deni.value);

  REAL result=REAL(numi)/REAL(deni);
  result.mp_conv();
  error=result.error;
  MP_duplicate_w_init(result.value,value);
  MP_getsize(value,vsize);
}

REAL::REAL(const INTEGER& y)
{
  MP_init(value);
  MP_INTEGER_to_mp(y.value,value);
  sizetype_exact(error);
  MP_getsize(value,vsize);
}



} // namespace iRRAM
