/*

iRRAM_core.h -- basic file for the errorsize arithmetic 
                and for the templates in the iRRAM library
 
Copyright (C) 2001-2009 Norbert Mueller
 
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
#ifndef IRRAM_CORE_H
#define IRRAM_CORE_H

#include <cstdio>
#include <vector>
#include <cfenv>
#include <pthread.h>

#include <iRRAM/lib.h>
#include <iRRAM/version.h>
#include <iRRAM/cache.h>

namespace iRRAM {

inline int min(const int a, const int b)
   { return (a < b ? a : b); }
inline int max(const int a, const int b)
   { return (a >= b ? a : b); }
inline int max3(const int a,const int b,const int c)
   { return max(max(a,b),c); }
inline int max4(const int a,const int b,const int c,const int d)
   { return max(max(a,b),max(c,d)); }

extern __thread int iRRAM_debug;
extern __thread int iRRAM_infinite;
extern void resources(double&,unsigned int&);
extern double ln2_time;
extern double pi_time;
void show_statistics();

extern const int iRRAM_prec_steps;
extern int iRRAM_prec_array[];
extern __thread  int iRRAM_prec_skip;
extern __thread  int iRRAM_max_prec;
extern __thread  int iRRAM_prec_start;

#ifndef NODEBUG
  #define iRRAM_DEBUG0(level,...)                                               \
	do {                                                                    \
		if (iRRAM_unlikely(iRRAM_debug>=ACTUAL_STACK.inlimit+(level))) {\
			__VA_ARGS__;                                            \
		}                                                               \
	} while (0)
#else
  #define iRRAM_DEBUG0(level,...)
#endif
#define iRRAM_DEBUG1(level,p)	iRRAM_DEBUG0((level),cerr << p)
#define iRRAM_DEBUG2(level,...)	iRRAM_DEBUG0((level),fprintf(stderr,__VA_ARGS__))


struct Iteration {int prec_diff; Iteration (int p){prec_diff=p;}; };

// inline void REITERATE(int p_diff){inReiterate = true; throw Iteration(p_diff); }
#define REITERATE(x)   {inReiterate = true;throw Iteration(x);};




const int MANTISSA_BITS = (int) (8*sizeof(SIZETYPEMANTISSA)) ;
const int DIFF_BITS     = 3; 
const int BIT_RANGE     = 8; 
const int BIT_RANGE2    = 8; 
const int GUARD_BITS    =  MANTISSA_BITS - DIFF_BITS;


inline unsigned int scale(const unsigned int w,const int p) {return ((p<=GUARD_BITS)?(w>>p):0); }

const SIZETYPEMANTISSA max_mantissa= 1 <<  GUARD_BITS   ; 
const SIZETYPEMANTISSA min_mantissa= 1 << (GUARD_BITS-BIT_RANGE);

/* if the exponent of value is smaller than MP_min, it should be increased(!) to (1,min_exponent) */
const SIZETYPEEXPONENT  min_exponent=MP_min + MANTISSA_BITS;


// sizetype_normalize(e) **********************************************
// Try to keep the mantissa between max_mantissa and min_mantissa.
// It may not be larger than  max_mantissa.
// It still may stay smaller than min_mantissa afterwards.
// The value of e may slighty increase during this operation!
// The value of e must never decrease!
// If the resulting exponent is out of range (MP_min,MP_max)
// correct it with an increase in value or produce an error.
// ********************************************************************


inline void sizetype_normalize( sizetype& e) {
  if (iRRAM_unlikely(e.mantissa < min_mantissa)) {
      e.mantissa <<= BIT_RANGE;
      e.exponent -= BIT_RANGE;
  }
  if (iRRAM_unlikely( e.mantissa >=  max_mantissa ) ) {
      e.mantissa = ( e.mantissa>> DIFF_BITS ) + 1; 
      e.exponent += DIFF_BITS;
  } 
  if (iRRAM_unlikely( e.exponent < MP_min ) ){
    e.exponent = min_exponent;
  }
  if (iRRAM_unlikely( e.exponent >= MP_max ) ) 
  {
    iRRAM_DEBUG1(1,"exponent too big in sizetype_normalize ");
    REITERATE(0);
  }
}

// sizetype_inc(x,y) **************************************************
// Increment x by y
// Arguments x and y must differ!
// The resulting value may be a bit larger than the exact value,
// The resulting value may never be smaller than the exact value
// ********************************************************************

inline void sizetype_inc(sizetype& x,const sizetype& y) 
{ 
  if ( y.exponent >  x.exponent) {
  x.mantissa=scale(x.mantissa,(y.exponent-x.exponent))
            +y.mantissa +1 ;
  x.exponent=y.exponent;
  } else {
  x.mantissa +=
            scale(y.mantissa,(x.exponent-y.exponent)) +1 ;
  }
  sizetype_normalize(x);
}

// sizetype_add_one(x,y) **************************************************
// Let x = y  + 1*2^z
// Arguments x and y must differ!
// The resulting value may be a bit larger than the exact value,
// The resulting value may never be smaller than the exact value
// ********************************************************************

inline void sizetype_add_one(sizetype& x,const sizetype& y,const SIZETYPEEXPONENT zexp) 
{
// replace 1*2^(-zexp) by 1048576*2^(-zexp-20)
// this is still the same numerical value!
// if zexp is larger than y.exponent, the result is closer to the intended result
  SIZETYPEEXPONENT zexp_scale= zexp-20;
  if ( y.exponent >  zexp_scale) {
    x.exponent= y.exponent;
    x.mantissa= y.mantissa + 1048576 ;
  } else {
    x.exponent= zexp_scale;
    x.mantissa= (scale(y.mantissa,(zexp_scale-y.exponent))+1) + 1048576 ;
  }
  sizetype_normalize(x);
}

// sizetype_inc_one(x,y) **************************************************
// Let x = x  + 1*2^z
// The resulting value may be a bit larger than the exact value,
// The resulting value may never be smaller than the exact value
// ********************************************************************

inline void sizetype_inc_one(sizetype& x,const SIZETYPEEXPONENT zexp) 
{
  if ( x.exponent > zexp) {
    x.mantissa ++ ;
  } else {
    x.mantissa=scale(x.mantissa,(zexp-x.exponent)) + 2 ;
    x.exponent= zexp;
  }
  sizetype_normalize(x);
}


// sizetype_inc2(x,y,z) **************************************************
// Increment x by y and z
// Arguments x, y and z must differ!
// The resulting value may be a bit larger than the exact value,
// The resulting value may never be smaller than the exact value
// ********************************************************************

inline void sizetype_inc2(sizetype& x,const sizetype& y,const sizetype& z) 
{ SIZETYPEEXPONENT exponent;
  exponent=max3(x.exponent,y.exponent,z.exponent);
  x.mantissa=scale(x.mantissa,(exponent-x.exponent))
            +scale(y.mantissa,(exponent-y.exponent)) 
            +scale(z.mantissa,(exponent-z.exponent)) +2 ;
  x.exponent=exponent;
  sizetype_normalize(x);
}


// sizetype_add_wo_norm(x,y,z) **************************************************
// Add y and z yielding x 
// Argument x must be different from y and z!
// The resulting value may be a bit larger than the exact value,
// The resulting value may never be smaller than the exact value
// ********************************************************************

inline void sizetype_add_wo_norm(sizetype& x,const sizetype& y,const sizetype& z) 
{
  if ( y.exponent >  z.exponent) {
    x.exponent= y.exponent;
    x.mantissa= y.mantissa + scale(z.mantissa,(x.exponent-z.exponent))  + 1 ;
  } else {
    x.exponent= z.exponent;
    x.mantissa= z.mantissa + scale(y.mantissa,(x.exponent-y.exponent))  + 1 ;
  }
}


// sizetype_add(x,y,z) **************************************************
// Add y and z yielding x 
// Argument x must be different from y and z!
// The resulting value may be a bit larger than the exact value,
// The resulting value may never be smaller than the exact value
// ********************************************************************

inline void sizetype_add(sizetype& x,const sizetype& y,const sizetype& z) 
{
  sizetype_add_wo_norm(x,y,z);
  sizetype_normalize(x);
}


inline void sizetype_copy(sizetype& x,const sizetype& y) 
{
  x.exponent=y.exponent;
  x.mantissa=y.mantissa;
}

// sizetype_shift(x,y,s) **************************************************
// Shift y and s yielding x 
// Arguments x,y may be identical!
// The resulting value may be a bit larger than the exact value,
// The resulting value may never be smaller than the exact value
// ********************************************************************

inline void sizetype_shift(sizetype& x,const sizetype& y,const int s) 
{
  x.exponent=y.exponent+s;
  x.mantissa=y.mantissa;

  if (iRRAM_unlikely( x.exponent < MP_min ) ) {
    iRRAM_DEBUG1(1,"warning: small exponent found in sizetype_shift\n");
    x.exponent = min_exponent;
//    x.mantissa = 1;
  } else  if ( iRRAM_unlikely(x.exponent >= MP_max) ) 
  {
    iRRAM_DEBUG1(1,"exponent too big in sizetype_shift ");
    REITERATE(0);
  }
}

// sizetype_mult(x,y,z) **************************************************
// Multiply y and z yielding x 
// Argument x must be different from y and z!
// The resulting value may be a bit larger than the exact value,
// The resulting value may never be smaller than the exact value
// ********************************************************************

inline void sizetype_mult(sizetype& x,const sizetype& y,const sizetype& z) 
{ unsigned long long lmantissa=
     ((unsigned long long)(y.mantissa))*z.mantissa;
  x.exponent=y.exponent+z.exponent;

  while (iRRAM_unlikely(lmantissa >= max_mantissa) ) 
       { lmantissa=lmantissa>>BIT_RANGE2;x.exponent+=BIT_RANGE2;}

  x.mantissa=lmantissa+1;
  sizetype_normalize(x);
}

// sizetype_max(x,y,z) **************************************************
// Compute maximum of y and z in x 
// Arguments x,y, and z may all be identical!
// The resulting value is exactly the maximum
// ********************************************************************

inline void sizetype_max(sizetype& x,const sizetype& y,const sizetype& z) 
{ if (y.exponent>z.exponent) 
  {
    if (scale(z.mantissa,y.exponent-z.exponent)>=y.mantissa) x=z; else x=y;
  } else {
    if (scale(y.mantissa,z.exponent-y.exponent)>=z.mantissa) x=y; else x=z;
  }
}

// sizetype_set(x,m,e) **************************************************
// Construct a sizetype value x from mantissa m and exponent e 
// The resulting value is allowed to be larger than the exact value
// ********************************************************************

inline void sizetype_set(sizetype& x,const int mantissa,const int exponent) 
{
  x.exponent=exponent;
  x.mantissa=mantissa;
  sizetype_normalize(x);
}

inline void sizetype_exact(sizetype& x) 
{
  x.exponent=min_exponent;
  x.mantissa=0;
}

/*Test whether  y<z; for y=z the result is allowed to be true OR false!  */
inline int sizetype_less(const sizetype& y,const sizetype& z) 
{ unsigned int mantissa;
  if (iRRAM_unlikely(y.mantissa==0)) return 1;
  if (iRRAM_unlikely(z.mantissa==0)) return 0;
  if (y.exponent>z.exponent) 
  {
    mantissa=scale(z.mantissa,y.exponent-z.exponent);
    return (mantissa>=y.mantissa);
  } else {
    mantissa=scale(y.mantissa,z.exponent-y.exponent);
    return (mantissa<z.mantissa);
  }
}

inline void sizetype_half(sizetype& x,const sizetype& y) 
{
  x.exponent=y.exponent-1;
  x.mantissa=y.mantissa;
}

/************************ the following functions are unchecked for over/underflow ***************/
/* also the exact semantics has still to be defined and compared to the applications ************/


inline void sizetype_dec(sizetype& x, const sizetype& y ) 
{ x.mantissa=x.mantissa - scale(y.mantissa,(x.exponent-y.exponent)) -1 ;
  sizetype_normalize(x);
}

inline void sizetype_dec(sizetype& x) 
{ x.mantissa -= 1 ;
}

inline void sizetype_sqrt(sizetype& x,const sizetype& y) 
{
  if (y.exponent&1) {
    x.exponent=(y.exponent-1)>>1; 
    x.mantissa=y.mantissa <<1;
  }  else {
    x.exponent=y.exponent>>1; 
    x.mantissa=y.mantissa;
  }
  x.mantissa=int(std::sqrt(double(x.mantissa)))+1;
}

inline void sizetype_div(sizetype& x,const sizetype& y,const sizetype& z) 
{ unsigned long long lmantissa=
         (((unsigned long long)(y.mantissa))<<GUARD_BITS)/z.mantissa;
  x.exponent=y.exponent-z.exponent-GUARD_BITS;

  while ( lmantissa >= max_mantissa ) 
       { lmantissa=lmantissa>>BIT_RANGE2;x.exponent+=BIT_RANGE2;}

  x.mantissa=lmantissa+1;
  sizetype_normalize(x);
}




/*****************************************/
// iRRAM_exec template

template <class RESULT,class... ARGUMENT>
RESULT iRRAM_exec(
	RESULT (*iRRAM_compute)(const ARGUMENT&...), const ARGUMENT&... x
) {
iRRAM_thread_data_address= new iRRAM_thread_data_class;

ITERATION_STACK SAVED_STACK;

ACTUAL_STACK.prec_step=iRRAM_prec_start;
ACTUAL_STACK.actual_prec=iRRAM_prec_array[ACTUAL_STACK.prec_step];
iRRAM_highlevel = (ACTUAL_STACK.prec_step > 1);
fesetround(FE_DOWNWARD);
// set the correct rounding mode for REAL using double intervals):

cache_active = new cachelist;

if ( iRRAM_unlikely(iRRAM_debug>0) ) {
	cerr <<"\niRRAM (version "<<iRRAM_VERSION_rt
		<<", backend "<<iRRAM_BACKENDS<<") starting...\n";
	iRRAM_max_prec=ACTUAL_STACK.prec_step;
}


RESULT result;

while (true) {

  iRRAM::cout.rewind();
  for (int n=0;n<max_active;n++){cache_active->id[n]->rewind();}
  inReiterate = false;
  ACTUAL_STACK.inlimit=0;

  iRRAM_highlevel = (ACTUAL_STACK.prec_step > 1);

  int p_end=0;
  try { result=iRRAM_compute(x...);   if ( iRRAM_likely(!iRRAM_infinite) ) break;}
  catch ( Iteration it)  { p_end=ACTUAL_STACK.actual_prec+it.prec_diff; }
  catch ( const iRRAM_Numerical_Exception exc)
    {
      cerr << "iRRAM exception: "<<iRRAM_error_msg[exc.type]<<"\n";
      throw iRRAM_Numerical_Exception (exc);
    }

	int prec_skip=0;
    	do {
		prec_skip++;
		ACTUAL_STACK.prec_step +=4;
		ACTUAL_STACK.actual_prec=iRRAM_prec_array[ACTUAL_STACK.prec_step];
    	} while ( (ACTUAL_STACK.actual_prec > p_end) && (prec_skip != iRRAM_prec_skip) ); 

  ACTUAL_STACK.inlimit=0;    	
  if ( iRRAM_unlikely(iRRAM_debug>0) ) {
	show_statistics();
         if (iRRAM_max_prec <= ACTUAL_STACK.prec_step) 
		iRRAM_max_prec  = ACTUAL_STACK.prec_step;
	cerr << "increasing precision bound to "<<ACTUAL_STACK.actual_prec<<"["<<ACTUAL_STACK.prec_step<<"]\n";
    	}
  }

iRRAM::cout.reset();
for (int n=0;n<max_active;n++){cache_active->id[n]->clear();}

max_active=0;
ACTUAL_STACK.inlimit=-1;
delete cache_active;
delete iRRAM_thread_data_address;

if ( iRRAM_unlikely (iRRAM_debug>0) ) {
	show_statistics();
	cerr << "iRRAM ending \n";
}

return result;
}


/*****************************************/
// templates needed for iRRAM_thread_exec 
extern unsigned int iRRAM_thread_maxid;

template <class ARGUMENT, class RESULT> class iRRAM_thread_data{public:
ARGUMENT argument;
RESULT result;
RESULT (*f) (const ARGUMENT&); 
pthread_t thread;
bool finished;
unsigned int id;
};

template <class ARGUMENT, class RESULT> void* iRRAM_thread_wrapper (iRRAM_thread_data<ARGUMENT,RESULT> *_data){

iRRAM_thread_data_address= new iRRAM_thread_data_class;

ITERATION_STACK SAVED_STACK;

ACTUAL_STACK.prec_step=iRRAM_prec_start;
ACTUAL_STACK.actual_prec=iRRAM_prec_array[ACTUAL_STACK.prec_step];
iRRAM_highlevel = (ACTUAL_STACK.prec_step > 1);
fesetround(FE_DOWNWARD);
// set the correct rounding mode for REAL using double intervals):

cache_active = new cachelist;

if ( iRRAM_unlikely(iRRAM_debug>0) ) {
	cerr <<"\niRRAM (version "<<iRRAM_VERSION_rt
		<<", backend "<<iRRAM_BACKENDS
		<<", thread "<< _data->id<<") starting...\n";
	iRRAM_max_prec=ACTUAL_STACK.prec_step;
}


while (true) {

  iRRAM::cout.rewind();
  for (int n=0;n<max_active;n++){cache_active->id[n]->rewind();}
  inReiterate = false;
  ACTUAL_STACK.inlimit=0;

  iRRAM_highlevel = (ACTUAL_STACK.prec_step > 1);

  int p_end=0;

  try {_data->result = _data-> f(_data->argument); 
       if ( iRRAM_likely(!iRRAM_infinite) ) break;}
  catch ( Iteration it)  { p_end=ACTUAL_STACK.actual_prec+it.prec_diff; }
  catch ( const iRRAM_Numerical_Exception exc)
    {
      cerr << "iRRAM exception: "<<iRRAM_error_msg[exc.type]<<"\n";
      throw iRRAM_Numerical_Exception (exc);
    }
	int prec_skip=0;
    	do {
		prec_skip++;
		ACTUAL_STACK.prec_step +=4;
		ACTUAL_STACK.actual_prec=iRRAM_prec_array[ACTUAL_STACK.prec_step];
    	} while ( (ACTUAL_STACK.actual_prec > p_end) && (prec_skip != iRRAM_prec_skip) ); 

  ACTUAL_STACK.inlimit=0;    	
  iRRAM_prec_start=ACTUAL_STACK.prec_step;
  if ( iRRAM_unlikely(iRRAM_debug>0) ) {
	show_statistics();
         if (iRRAM_max_prec <= ACTUAL_STACK.prec_step) 
		iRRAM_max_prec  = ACTUAL_STACK.prec_step;
	cerr << "increasing precision bound to "<<ACTUAL_STACK.actual_prec<<"["<<ACTUAL_STACK.prec_step<<"]\n";
	cerr << "iRRAM iterating in thread "<< _data->id <<  "\n";
    	}
  }

iRRAM::cout.reset();
for (int n=0;n<max_active;n++){cache_active->id[n]->clear();}

max_active=0;
ACTUAL_STACK.inlimit=-1;
delete cache_active;
delete iRRAM_thread_data_address;

if ( iRRAM_unlikely (iRRAM_debug>0) ) {
	show_statistics();
	cerr << "iRRAM thread "<< _data->id <<  " ending \n";
}

_data->finished=true;

return NULL;
}


template <class ARGUMENT, class RESULT> 
  void iRRAM_thread_exec  
(RESULT iRRAM_compute(const ARGUMENT&), const ARGUMENT& x, iRRAM_thread_data<ARGUMENT,RESULT>& data)
{
typedef void*(*testf)(void*) ;
data.argument  = x;
data.f         = iRRAM_compute;
data.id        = ++iRRAM_thread_maxid;
data.finished  = false;

void* (*my_f)(iRRAM_thread_data<ARGUMENT,RESULT>*) = &iRRAM_thread_wrapper<ARGUMENT,RESULT>;
void*(*my_function)(void*) = reinterpret_cast<testf> (my_f);
if (int errcode=pthread_create(&(data.thread),/* thread struct             */
		       NULL,                    /* default thread attributes */
		       my_function,                    /* start routine             */
		       &data))         /* arg to routine            */
	{ cerr << "Error " << errcode << " in pthread_create"; }

return;
}


template <class ARGUMENT, class RESULT> 
RESULT iRRAM_thread_wait(const iRRAM_thread_data<ARGUMENT,RESULT>& data){
  if ( iRRAM_unlikely (iRRAM_debug>0) ) {
      cerr << "Waiting for termination of thread "<< data.id << "\n";
  }
  pthread_join(data.thread,NULL);
  if ( iRRAM_unlikely (iRRAM_debug>0) ) {
      cerr << "Thread "<< data.id << " now terminated.\n";
  }
  return data.result;
}



template <class ARGUMENT, class RESULT> 
bool iRRAM_thread_finished(const iRRAM_thread_data<ARGUMENT,RESULT>& data){
  return data.finished;
}


} /* ! namespace iRRAM */

//*************************************************************************************/
// predefined instantiations of some iRRAM_exec routines can be found in stack.cc
//*************************************************************************************/
#endif /* ! iRRAM_CORE_H  */
