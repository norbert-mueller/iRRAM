/*

RATIONAL.cc -- routines for RATIONAL data type
 
Copyright (C) 2001-2003 Norbert Mueller, Tom van Diessen
 
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
Changelog: (initial version by Tom, all modifications by Norbert)

  2003-08-29  Addition of functions x+i, x-i, x*i, x/i and i+x etc
              for i of simple type "int"

  2001-07-19  Initial version from Tom's diploma thesis

*/

#include <stdlib.h>
#include <stdarg.h>
#include <string.h>

#include "iRRAM/core.h"

namespace iRRAM {

#define maxsint 2147483647
#define maxint 2*(maxsint-1) //****** must be unsigned ******


//****************************************************************************************
// CONSTRUCTORS
// IMPORTANT: Objects MUST be initialized before being used.
//****************************************************************************************

inline RATIONAL::RATIONAL(MP_rat_type y){
  value = y;
}

//****************************************************************************************
// Constructing RATIONAL from scratch
//****************************************************************************************

RATIONAL::RATIONAL(){
  MP_rat_init(value);
  MP_int_to_RATIONAL(0,value);
}

RATIONAL::RATIONAL(const INTEGER& i,const INTEGER& j){
  MP_rat_init(value);
  MP_INTINTEGER_to_RATIONAL(i.value,j.value,value);
}

RATIONAL::RATIONAL(const int i, const int j){
  MP_rat_init(value);
  if ( j >= 0) {
	MP_intint_to_RATIONAL(i,(unsigned int)(j),value);
	} else {
	INTEGER ii(i);
	INTEGER jj(j);
	MP_INTINTEGER_to_RATIONAL(ii.value,jj.value,value);
	}
}

RATIONAL::RATIONAL(const INTEGER& i){
  MP_rat_init(value);
  MP_INTEGER_to_RATIONAL(i.value,value);
}

RATIONAL::RATIONAL(const int i) {
  MP_rat_init(value);
  MP_int_to_RATIONAL(i,value);
}

RATIONAL::RATIONAL(const RATIONAL& y){
  MP_rat_init(value);
  MP_rat_duplicate_wo_init(y.value, value);
}


//****************************************************************************************
// Constructing RATIONAL from double, the result is NOT rounded
//****************************************************************************************

RATIONAL::RATIONAL(const double d){
  MP_rat_init(value);
  MP_double_to_RATIONAL(d,value);
}

//****************************************************************************************
// Constructing RATIONAL from string, the string must be in decimal base
//****************************************************************************************

RATIONAL::RATIONAL(const char* s){
  MP_rat_init(value);
  MP_string_to_RATIONAL(s,value);
}

//****************************************************************************************
// Copy constructor
// Types on both sides must be RATIONAL, left side must already be initialized.
//****************************************************************************************

RATIONAL& RATIONAL::operator = (const RATIONAL& y){
  MP_rat_duplicate_wo_init(y.value, value);
  return (*this);
}

RATIONAL& RATIONAL::operator = (const INTEGER& y){
  return (*this)=RATIONAL(y);
}

RATIONAL& RATIONAL::operator = (const int y){
  MP_int_to_RATIONAL(y,value);
  return (*this)=y;
}

//**************************************************************/
// Destructor
// Destroying the object and freeing space occupied by it
//******************************************************************/

RATIONAL::~RATIONAL(){
  MP_rat_clear(value);
}

//****************************************************************************************
// Returns the sign of RATIONAL objects (-1/0/1): 
// sign(i)= -1, iff i<0,   sign(i)=1 iff i>0,  and sign(0)=0
//****************************************************************************************
int sign(const RATIONAL& x)
{
  return MP_rat_sign(x.value);
}

//****************************************************************************************
// Shift
// defines shifting function if none exists in the backend
//****************************************************************************************

#ifdef MP_shift

RATIONAL scale(const RATIONAL& x, int n)
{
	MP_rat_type zvalue;
	MP_rat_init(zvalue);
	MP_rat_shift(x.value,zvalue,n);
	return zvalue;
}

#else

RATIONAL scale(const RATIONAL& x, int n)
{
	if (n==0) return x;
	if (n==1) return 2*x;
	RATIONAL y=1;
	RATIONAL xc=2;
	if (n<0) {xc=y/2;n=-n;}
	if (n==1) return xc*x;
	for (int k=n;k>0;k=k/2)
	{
		if (k%2==1) y*=xc;
		if (k==1) break;
		xc*=xc;
	}
	return y*x;
}

#endif


//*********************************************************************/
// Addition: returns x + y
//**********************************************************************/

RATIONAL operator + (const RATIONAL& x, const RATIONAL& y){
  MP_rat_type zvalue;
  MP_rat_init(zvalue);
  MP_rat_add(x.value,y.value,zvalue);
  return zvalue;
}

RATIONAL operator + (const RATIONAL& x, const INTEGER&  y) {return x+RATIONAL(y); }
RATIONAL operator + (const INTEGER&  x, const RATIONAL& y) {return RATIONAL(x)+y; }

RATIONAL operator + (const RATIONAL& x, const int y){
  MP_rat_type zvalue;
  MP_rat_init(zvalue);
  if(y<0) MP_rat_sub_ui(x.value,-y,zvalue);
  else MP_rat_add_ui(x.value,y,zvalue);
  return zvalue;
}

RATIONAL operator + (const int x, const RATIONAL& y){ return y+x; }

//****************************************************************************************
// Addition: returns x += y
// Simple infix addition of 2 RATIONAL-objects using left-hand argument 
// as second right-hand argument
//****************************************************************************************

RATIONAL& operator += (RATIONAL& x, const RATIONAL& y)
{
  MP_rat_add(x.value,y.value,x.value);
  return x;
}

RATIONAL& operator += (RATIONAL& x, const INTEGER& n){ return x+=RATIONAL(n);}
RATIONAL& operator += (RATIONAL& x, const int n){ return x+=RATIONAL(n);}

//****************************************************************************************
// Subtraction: returns x - y
// Simple infix difference of 2 RATIONAL-objects
//****************************************************************************************

RATIONAL operator - (const RATIONAL& x, const RATIONAL& y){
  MP_rat_type zvalue;
  MP_rat_init(zvalue);
  MP_rat_sub(x.value,y.value,zvalue);
  return zvalue;
}

RATIONAL operator - (const RATIONAL& x, const INTEGER&  y){ return x-RATIONAL(y);}
RATIONAL operator - (const INTEGER&  x, const RATIONAL& y){ return RATIONAL(x)-y;}

RATIONAL operator - (const RATIONAL& x, int y){
  MP_rat_type zvalue;
  MP_rat_init(zvalue);
  if(y<0) MP_rat_add_ui(x.value,-y,zvalue);
  else MP_rat_sub_ui(x.value,y,zvalue);
  return zvalue;
}

RATIONAL operator - (const int x, const RATIONAL& y){ return -(y-x); }

RATIONAL& operator -= (RATIONAL& x, const RATIONAL& y){
  MP_rat_sub(x.value,y.value,x.value);
  return x;
}

RATIONAL& operator -= (RATIONAL& x, const INTEGER& n){ return x-=RATIONAL(n);}
RATIONAL& operator -= (RATIONAL& x, const int n){ return x-=RATIONAL(n);}

//****************************************************************************************
// Negative: returns -x
// changes sign of RATIONAL
//****************************************************************************************

RATIONAL operator - (const RATIONAL& x){
  MP_rat_type zvalue;
  RATIONAL y=RATIONAL(0);
  MP_rat_init(zvalue);
  MP_rat_sub(y.value,x.value,zvalue);
  return zvalue;
}

//****************************************************************************************
// Multiplication: returns x * y
//****************************************************************************************

RATIONAL operator * (const RATIONAL& x, const RATIONAL& y){
  MP_rat_type zvalue;
  MP_rat_init(zvalue);
  MP_rat_mul(x.value,y.value,zvalue);
  return zvalue;
}

RATIONAL operator * (const RATIONAL& x, const INTEGER&  y){ return x*RATIONAL(y);}
RATIONAL operator * (const INTEGER&  x, const RATIONAL& y){ return RATIONAL(x)*y;}

RATIONAL operator * (const RATIONAL& x, const int y){
  MP_rat_type zvalue;
  MP_rat_init(zvalue);
  MP_rat_mul_si(x.value,y,zvalue);
  return zvalue;
}

RATIONAL operator * (const int      x, const RATIONAL& y){ return y*x;}

RATIONAL& operator *= (RATIONAL& x, const RATIONAL& y){
  MP_rat_mul(x.value,y.value,x.value);
  return x;
}

RATIONAL& operator *= (RATIONAL& x, const INTEGER& n){ return x*=RATIONAL(n);}
RATIONAL& operator *= (RATIONAL& x, const int n){ return x*=RATIONAL(n);}


//****************************************************************************************
// Division: returns x / y
// Simple infix division of 2 RATIONAL-objects
//****************************************************************************************

RATIONAL operator / (const RATIONAL& x, const RATIONAL& y)
{
	MP_rat_type zvalue;
	MP_rat_init(zvalue);
	MP_rat_div(x.value,y.value,zvalue);
	return zvalue;
}

RATIONAL operator / (const RATIONAL& x, const INTEGER&  y){ return x/RATIONAL(y);}
RATIONAL operator / (const INTEGER&  x, const RATIONAL& y){ return RATIONAL(x)/y;}

RATIONAL operator / (const RATIONAL& x, int y){
  MP_rat_type zvalue;
  MP_rat_init(zvalue);
  MP_rat_div_si(x.value,y,zvalue);
  return zvalue;
}

RATIONAL operator / (int x, const RATIONAL& y){
  MP_rat_type zvalue;
  MP_rat_init(zvalue);
  MP_rat_si_div(x,y.value,zvalue);
  return zvalue;
}

RATIONAL& operator /= (RATIONAL& x, const RATIONAL& y){
  MP_rat_div(x.value,y.value,x.value);
  return x;
}

RATIONAL& operator /= (RATIONAL& x, const INTEGER& n){ return x/=RATIONAL(n);}
RATIONAL& operator /= (RATIONAL& x, const int n){ return x/=RATIONAL(n);}


//****************************************************************************************
// Absolute value: |x|
//****************************************************************************************

RATIONAL abs (const RATIONAL& x){
  MP_rat_type zvalue;
  MP_rat_init(zvalue);
  MP_rat_abs(x.value,zvalue);
  return zvalue;
}

INTEGER denominator (const RATIONAL& x){
  MP_int_type zvalue;
  MP_int_init(zvalue);
  MP_rat_get_denominator(x.value,zvalue);
  return zvalue;
}

INTEGER numerator (const RATIONAL& x){
  MP_int_type zvalue;
  MP_int_init(zvalue);
  MP_rat_get_numerator(x.value,zvalue);
  return zvalue;
}

//****************************************************************************************
// Less than: x < y
// returns boolean value 1 if x<y, 0 otherwise
//****************************************************************************************

bool operator < (const RATIONAL& x, const RATIONAL& y){
  return (MP_rat_compare(x.value,y.value) < 0 );
}
bool operator < (const RATIONAL& x, const INTEGER&  y){ return x<RATIONAL(y);};
bool operator < (const RATIONAL& x, const int       y){ return x<RATIONAL(y);};
bool operator < (const INTEGER&  x, const RATIONAL& y){ return RATIONAL(x)<y;};
bool operator < (const int       x, const RATIONAL& y){ return RATIONAL(x)<y;};

//****************************************************************************************
// Less or equal than: x <= y
// returns boolean value 1 if x<=y, 0 otherwise
//****************************************************************************************
bool operator <= (const RATIONAL& x, const RATIONAL& y){ return !(y<x); };
bool operator <= (const RATIONAL& x, const INTEGER&  y){ return !(y<x); };
bool operator <= (const RATIONAL& x, const int       y){ return !(y<x); };
bool operator <= (const INTEGER&  x, const RATIONAL& y){ return !(y<x); };
bool operator <= (const int       x, const RATIONAL& y){ return !(y<x); };

//****************************************************************************************
// Greater than: x > y
// returns boolean value 1 if x>y, 0 otherwise
//****************************************************************************************
bool operator > (const RATIONAL& x, const RATIONAL& y){ return (y<x); };
bool operator > (const RATIONAL& x, const INTEGER&  y){ return (y<x); };
bool operator > (const RATIONAL& x, const int       y){ return (y<x); };
bool operator > (const INTEGER&  x, const RATIONAL& y){ return (y<x); };
bool operator > (const int       x, const RATIONAL& y){ return (y<x); };


//****************************************************************************************
// Greater or equal than: x >= y
// returns boolean value 1 if x>=y, 0 otherwise
//****************************************************************************************
bool operator >= (const RATIONAL& x, const RATIONAL& y){ return !(x<y); };
bool operator >= (const RATIONAL& x, const INTEGER&  y){ return !(x<y); };
bool operator >= (const RATIONAL& x, const int       y){ return !(x<y); };
bool operator >= (const INTEGER&  x, const RATIONAL& y){ return !(x<y); };
bool operator >= (const int       x, const RATIONAL& y){ return !(x<y); };

//****************************************************************************************
// Equal to: x == y
// returns boolean value 1 if x==y, 0 otherwise
//****************************************************************************************

bool operator == (const RATIONAL& x, const RATIONAL& y){
  return (MP_rat_equal(x.value,y.value));
}
bool operator == (const RATIONAL& x, const INTEGER&  y){ return x==RATIONAL(y);};
bool operator == (const RATIONAL& x, const int       y){ return x==RATIONAL(y);};
bool operator == (const INTEGER&  x, const RATIONAL& y){ return y==RATIONAL(x);};
bool operator == (const int       x, const RATIONAL& y){ return y==RATIONAL(x);};

//****************************************************************************************
// Not equal to: x != y
// returns boolean value 1 if x!=y, 0 otherwise
//****************************************************************************************

bool operator != (const RATIONAL& x, const RATIONAL& y){ return !(x==y); };
bool operator != (const RATIONAL& x, const INTEGER&  y){ return !(x==y); };
bool operator != (const RATIONAL& x, const int       y){ return !(x==y); };
bool operator != (const INTEGER& y, const RATIONAL& x ){ return !(y==x); };
bool operator != (const int      y, const RATIONAL& x ){ return !(y==x); };


//*****************************************************************************
// Function swrite
// string function for RATIONAL
// writes rational into string
//*****************************************************************************

std::string swrite(const RATIONAL& x, const int w){
  	return MP_rat_swritee(x.value,w);
}

//****************************************************************************************
// swrite: writes RATIONAL to string
// 1. argument: RATIONAL
//****************************************************************************************

std::string swrite(const RATIONAL& x){
	return MP_rat_sprintf(x.value);
}

} // namespace iRRAM
