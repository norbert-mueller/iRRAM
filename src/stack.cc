/*

stack.cc -- basic file for the iterations in the iRRAM library
 
Copyright (C) 2001-2006 Norbert Mueller
 
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

#include <cmath>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>

#include "iRRAM/core.h"


namespace iRRAM {

//*************************************************************************************
// runtime identification of the iRRAM version, cf. iRRAM_version.h
//*************************************************************************************

const char* iRRAM_VERSION_rt = iRRAM_VERSION_ct;



__thread ITERATION_DATA ACTUAL_STACK={-1,-1,-1,-1};
__thread cachelist * cache_active=0;
__thread int max_active=0;
__thread iRRAM_thread_data_class *iRRAM_thread_data_address=0; 
__thread bool iRRAM_highlevel=false;
__thread int iRRAM_DYADIC_precision=-60;
__thread bool iRRAM_COMPARE_exact=true;
__thread int iRRAM_COMPARE_precision=-60;

/*
The following boolean "inReiterate" is used to distinguish voluntary 
deletions of rstreams from deletions initiated by iterations. 
The latter should be ignored, as stream operations using this stream
might continue in later iterations!
*/
__thread bool inReiterate=false;


/*
Threads have an internal id for debugging purposes
The following global counter is use to get (almost) unique id's.
*/
unsigned int iRRAM_thread_maxid=0;



/*****************************************/
// module function (will be a template later...)

int module(REAL f(const REAL&),const REAL& x, int p){
// Semantics: If m=module(f,x,p), then
//    |x-z| <=2^m implies  |f(x)-f(z)| <= 2^p 

// If we are able to approximate f(x) by a DYADIC number d with an error of <=2^{p-1},
// then for any z in the argument interval of x, f(z) must differ by at most 
//  2^{p-1} from d, hence |f(x)-f(z)|<=2^p

  int result;
  if ( (ACTUAL_STACK.inlimit==0) && iRRAM_thread_data_address->cache_i.get(result)) return result;

  DYADIC d;
  REAL x_copy=x;

  
  sizetype argerror,testerror;
  
  x_copy.geterror(argerror);
  sizetype_set(testerror,1,argerror.exponent);
  x_copy.adderror(testerror);
  continous_begin();
  d=approx(f(x_copy),p-1); 
  continous_end();
// At this line, we are sure that x_copy (and so also x) is precise enough to allow 
// the computation of f(x), even with a slightly increased error of the argument.

// We now try to find the smallest p_arg such that the evaluation of f(x+- 2^p_arg) 
// is possible up to an error of at most  2^{p-1}

  ITERATION_STACK SAVED_STACK;

// To do this, we start with p_arg=p.
// If this is successfull, we increase the value of p_arg until the first failure
// It it is not, then we decrease until the first success...
  int direction=0,p_arg=p;
  bool try_it=true;

  while (try_it) {
    sizetype_set(testerror,1,p_arg);
    x_copy.seterror(argerror);
    x_copy.adderror(testerror);
    bool fail = false;
  if ( unlikely(iRRAM_debug > 0 ) ) {
   sizetype x_error;
   x_copy.geterror(x_error);
  DEBUG4(1,"Testing module: 1*2^%d + %d*2^%d\n",p_arg,argerror.mantissa,argerror.exponent);
  DEBUG3(1,"argument error: %d*2^%d\n",x_error.mantissa,x_error.exponent);
  }
  try { 
      continous_begin();
      REAL z=f(x_copy);
      if ( unlikely(iRRAM_debug > 0 ) ) {
        sizetype z_error;
        z.geterror(z_error);
        DEBUG3(1,"Module yields result %d*2^%d\n",z_error.mantissa,z_error.exponent);
      }
      d=approx(z,p-1); 
      continous_end();
	}
    catch ( Iteration it)  { fail=true; }
    switch ( direction ) {
      case 0:;
        if ( fail ) direction=-1; else direction=1;
	p_arg+=direction;
      break;
      case 1:;
        if ( fail ) { try_it=false; p_arg -=direction; }
	else { p_arg += direction; };
      break;
      case -1:;
        if ( fail ) { p_arg +=direction; }
	else { try_it=false; };
      break;    
    }
    }
  DEBUG2(1,"Modules resulting in p_arg=%d\n",p_arg);
  
  sizetype_set(testerror,1,p_arg);
  sizetype_inc(argerror,testerror);

  while (argerror.mantissa>1) {
    argerror.mantissa=argerror.mantissa>>1;
    argerror.exponent+=1;
  }
  
  result=argerror.exponent;
  if ( ACTUAL_STACK.inlimit==0 ) iRRAM_thread_data_address->cache_i.put(result);
  return result;

};


//*************************************************************************************
// instantiation of some iRRAM_exec routines
//*************************************************************************************

template double iRRAM_exec <double, double> (double (*) (const double&), const double&);
template int    iRRAM_exec <int,int> (int (*) (const int&), const int&);

} // namespace iRRAM
// 