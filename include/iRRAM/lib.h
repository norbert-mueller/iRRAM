/*

iRRAM_lib.h -- central header file for the iRRAM library
 
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


/*
Authors:  all by Norbert, except: 

  2003-07 INTERVAL extensions partly by Shao Qi

  2001-07 INTEGER and RATIONAL partly by Tom van Diessen


*/

#ifndef iRRAM_LIB_H
#define iRRAM_LIB_H

#include <string>

#if defined(__GNUC__) || defined(__clang__) || defined(__builtin_expect)
/* Don't need to check version of compilers, they all support __builtin_expect()
 * since the time they know of C++11 */
# define iRRAM_expect(e,n)	__builtin_expect((e),(n))
#else
# define iRRAM_expect(e,n)	(e)
#endif
#define iRRAM_likely(x)		iRRAM_expect(!!(x), 1)
#define iRRAM_unlikely(x)	iRRAM_expect(!!(x), 0)

#include <iRRAM/version.h>

#ifndef iRRAM_BACKENDS
# error error: no usable backend, defined iRRAM_BACKENDS
#endif

#if iRRAM_BACKEND_MPFR
# include <iRRAM/MPFR_interface.h>
#else
# error "Currently no additional backend!"
#endif

extern "C" void iRRAM_initialize(int argc,char** argv);

namespace iRRAM{

template <class RESULT,class... ARGUMENT> 
 RESULT iRRAM_exec (RESULT (*f)(const ARGUMENT&...), const ARGUMENT&...);


// forward declaration of some classes

class INTEGER;
class RATIONAL;
class DYADIC;
class LAZY_BOOLEAN;
class REAL;
class COMPLEX;

extern char* iRRAM_error_msg[];
class iRRAM_Numerical_Exception
{
public:
iRRAM_Numerical_Exception(const int msg){type=msg;};
//private:
int type;
};
#define ERRORDEFINE(x,y) x,
enum iRRAM_exception_list {
#include <iRRAM/errno.h>
};



typedef unsigned int SIZETYPEMANTISSA;
typedef int SIZETYPEEXPONENT;

struct sizetype { SIZETYPEMANTISSA  mantissa; SIZETYPEEXPONENT exponent; } ;

struct ITERATION_DATA {
  int prec_policy;
  int inlimit;
  int actual_prec;
  int prec_step;
};

class ITERATION_STACK {
public:
  ITERATION_STACK();
  ~ITERATION_STACK();
  ITERATION_DATA data;
};


extern __thread ITERATION_DATA ACTUAL_STACK;
extern __thread bool iRRAM_highlevel;
extern __thread bool inReiterate;

inline  ITERATION_STACK::ITERATION_STACK(){
	this->data=ACTUAL_STACK;
	}
inline  ITERATION_STACK::~ITERATION_STACK(){
	ACTUAL_STACK= this->data;
	iRRAM_highlevel = (ACTUAL_STACK.prec_step > 1);
	}

} // namespace iRRAM

#include <iRRAM/LAZYBOOLEAN.h>
#include <iRRAM/REAL.h>
#include <iRRAM/DYADIC.h>
#include <iRRAM/INTEGER.h>
#include <iRRAM/RATIONAL.h>
#include <iRRAM/REALMATRIX.h>
#include <iRRAM/SPARSEREALMATRIX.h>
#include <iRRAM/COMPLEX.h>
#include <iRRAM/INTERVAL.h>
#include <iRRAM/STREAMS.h>
#include <iRRAM/SWITCHES.h>
#include <iRRAM/FUNCTION.h>


namespace iRRAM {

#define iRRAM_ABSOLUTE 0
#define iRRAM_RELATIVE 1
void precision_policy (int policy);

REAL atoREAL(const char* s);


/****************************************************************************/
// arithmetic functions
/****************************************************************************/
REAL power(const REAL& x, const REAL& y);
REAL power(const REAL& x, int n);
REAL modulo  (const REAL& x, const REAL& y);
REAL maximum (const REAL& x, const REAL& y);
REAL minimum (const REAL& x, const REAL& y);

/****************************************************************************/
// roots
/****************************************************************************/
REAL sqrt    (const REAL& x);
REAL root    (const REAL& x,int n);

/****************************************************************************/
// trigonometric functions
/****************************************************************************/
REAL sin     (const REAL& x);
REAL cos     (const REAL& x);
REAL tan     (const REAL& x);
REAL cotan   (const REAL& x);
REAL sec     (const REAL& x);
REAL cosec   (const REAL& x);

/****************************************************************************/
// inverse trigonometric functions
/****************************************************************************/
REAL atan    (const REAL& x);
REAL asin    (const REAL& x);
REAL acos    (const REAL& x);
REAL acotan  (const REAL& x);
REAL asec    (const REAL& x);
REAL acosec  (const REAL& x);

/****************************************************************************/
//hyperbolic functions
/****************************************************************************/
REAL sinh    (const REAL& x);
REAL cosh    (const REAL& x);
REAL tanh    (const REAL& x);
REAL coth    (const REAL& x);
REAL sech    (const REAL& x);
REAL cosech  (const REAL& x);

/****************************************************************************/
// inverse hyperbolic functions 
/****************************************************************************/
REAL asinh   (const REAL& x);
REAL acosh   (const REAL& x);
REAL atanh   (const REAL& x);
REAL acoth   (const REAL& x);
REAL asech   (const REAL& x);
REAL acosech (const REAL& x);

/****************************************************************************/
// exponentiation + logarithm
/****************************************************************************/
REAL exp     (const REAL& x);
REAL log     (const REAL& x);

/****************************************************************************/
// special constants values                                                 
/****************************************************************************/
REAL pi      ();   // = 3.141592653...
REAL euler   ();   // = 2.718281828...
REAL ln2     ();   // = 0.693147180...

/****************************************************************************/
//  a few vector functions
/****************************************************************************/
REAL abs    (const std::vector<REAL>& x);

/****************************************************************************/
//  matrix functions
/****************************************************************************/
REALMATRIX exp    (const REALMATRIX& x);

REALMATRIX steady_state (const REALMATRIX& x);

REALMATRIX steady_state (const SPARSEREALMATRIX& x);
REALMATRIX equilib (const SPARSEREALMATRIX& x);
void equilib_del (SPARSEREALMATRIX& x,REALMATRIX& z);

/****************************************************************************/
//  a few inlined functions
/****************************************************************************/
inline REAL   round2     (const REAL& x){ return REAL(x.as_INTEGER()); }
inline int    round      (const REAL& x){ return int (x.as_INTEGER()); }



/****************************************************************************/
// templates for limit operators
/****************************************************************************/

template <class ARGUMENT, class RESULT> 
RESULT limit_mv (RESULT f(int prec,
                                         int* choice,
                                         const ARGUMENT&),
                           const ARGUMENT& x);

template <class ARGUMENT, class RESULT> 
RESULT limit (RESULT f(int prec,const ARGUMENT&),
                           const ARGUMENT& x);


template <class ARGUMENT, class DISCRETE, class RESULT> 
RESULT limit (RESULT f(int prec,const ARGUMENT&, DISCRETE),
                           const ARGUMENT& x, DISCRETE param);

} // namespace iRRAM

#endif /* ! iRRAM_LIB_H */
