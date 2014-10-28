/*

iRRAM_INTEGER.h -- header file for the INTEGER part of the iRRAM library
 
Copyright (C) 2001-2003 Norbert Mueller
 
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

#ifndef iRRAM_INTEGER_H
#define iRRAM_INTEGER_H

namespace iRRAM {

class INTEGER 
{
public:

friend class REAL;
friend class DYADIC;
friend class RATIONAL;

/****** Constructors ******/

INTEGER(MP_int_type y);

INTEGER();
INTEGER(const int i);
INTEGER(const std::string s);
INTEGER(const char* s);
INTEGER(const INTEGER& y); 
INTEGER(const double d);

/****** Copy constructor ******/

INTEGER& operator = (const int y);
INTEGER& operator = (const INTEGER& y);

	/****** Destructor ******/
	
~INTEGER();

	/****** Standard arithmetic ******/

friend INTEGER 	operator +  (const INTEGER& x, const INTEGER& y);
friend INTEGER 	operator +  (const INTEGER& x, const int      y);
friend INTEGER 	operator +  (const int      x, const INTEGER& y);

friend INTEGER 	operator -  (const INTEGER& x, const INTEGER& y);
friend INTEGER 	operator -  (const INTEGER& x, const int      y);
friend INTEGER 	operator -  (const int      x, const INTEGER& y);

friend INTEGER 	operator -  (const INTEGER& x);

friend INTEGER 	operator *  (const INTEGER& x, const INTEGER& y);
friend INTEGER	operator *  (const INTEGER& x, const int      y);
friend INTEGER	operator *  (const int      x, const INTEGER& y);

friend INTEGER 	operator /  (const INTEGER& x, const INTEGER& y);
friend INTEGER 	operator /  (const INTEGER& x, const int      y);
friend INTEGER 	operator /  (const int      x, const INTEGER& y);

friend INTEGER 	operator ^  (const INTEGER& x, const INTEGER& y);
friend INTEGER  operator ^  (const INTEGER& x, const unsigned int y);
friend INTEGER 	operator << (const INTEGER& x, const int y);
friend INTEGER 	operator >> (const INTEGER& x, const int y);
friend INTEGER	operator %  (const INTEGER& x, const INTEGER& y);

friend INTEGER&	operator += (	   INTEGER& x, const INTEGER& y);
friend INTEGER&	operator += (	   INTEGER& x, const int      y);

friend INTEGER&	operator -= (	   INTEGER& x, const INTEGER& y);
friend INTEGER&	operator -= (	   INTEGER& x, const int      y);

friend INTEGER&	operator *= (	   INTEGER& x, const INTEGER& y);
friend INTEGER&	operator *= (	   INTEGER& x, const int      y);

friend INTEGER&	operator /= (	   INTEGER& x, const INTEGER& y);
friend INTEGER&	operator /= (	   INTEGER& x, const int      y);



friend INTEGER 	sqrt	(const INTEGER& x);
friend INTEGER		scale	(const INTEGER& x, const int k);
friend INTEGER		abs	(const INTEGER& x);

/****** Comparisons ******/

friend bool operator <  (const INTEGER& x, const INTEGER& y);
friend bool operator <  (const INTEGER& x, const int      y);
friend bool operator <  (const int      x, const INTEGER& y);

friend bool operator <= (const INTEGER& x, const INTEGER& y);
friend bool operator <= (const INTEGER& x, const int      y);
friend bool operator <= (const int      x, const INTEGER& y);

friend bool operator >  (const INTEGER& x, const INTEGER& y);
friend bool operator >  (const INTEGER& x, const int      y);
friend bool operator >  (const int      x, const INTEGER& y);

friend bool operator >= (const INTEGER& x, const INTEGER& y);
friend bool operator >= (const INTEGER& x, const int      y);
friend bool operator >= (const int      x, const INTEGER& y);

friend bool operator == (const INTEGER& x, const INTEGER& y);
friend bool operator == (const INTEGER& x, const int      y);
friend bool operator == (const int      x, const INTEGER& y);

friend bool operator != (const INTEGER& x, const INTEGER& y);
friend bool operator != (const INTEGER& x, const int      y);
friend bool operator != (const int      x, const INTEGER& y);
	
friend int size(const INTEGER& x);
friend int sign(const INTEGER& x);

/* conversion */
friend std::string    swrite  (const INTEGER& x, const int w);
friend std::string    swrite  (const INTEGER& x);

operator int()  const ;


/****** Private ******/
//private:

MP_int_type value;

};

inline INTEGER::~INTEGER(){  MP_int_clear(value);}

inline INTEGER::INTEGER::INTEGER(MP_int_type y){ value = y; ;}

inline INTEGER::INTEGER(const int i){
	MP_int_init(value);
	MP_int_to_INTEGER(i,value);
}

inline INTEGER::INTEGER(){
	MP_int_init(value);
	MP_int_to_INTEGER(0,value);
}


inline INTEGER& INTEGER::operator = (const INTEGER& y){
  MP_int_duplicate_wo_init(y.value, value);
  return (*this);
}

inline INTEGER& INTEGER::operator = (const int y){
  MP_int_to_INTEGER(y,value);
  return (*this);
}


#ifdef MP_shift

inline INTEGER scale(const INTEGER& x, const int n)
{
	MP_int_type zvalue;
	MP_int_init(zvalue);
	MP_int_shift(x.value,zvalue,n);
	return zvalue;
}

#else

inline INTEGER scale(const INTEGER& x, const int n)
{
	if (n==0) return x;
	if (n==1) return 2*x;
	INTEGER y=1;
	INTEGER xc=2;
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
//****************************************************************************************
// Returns the sign of INTEGER objects (-1/0/1): 
// sign(i)= -1, iff i<0,   sign(i)=1 iff i>0,  and sign(0)=0
//****************************************************************************************
inline int sign(const INTEGER& x){  return MP_int_sign(x.value); }

//****************************************************************************************
// Returns the size of INTEGER objects: The smallest i>0 such that |x| < 2^i,
// e.g. size(0)=size(1)=1;size(2)=size(3)=2,size(4)=...size(7)=3
//****************************************************************************************

inline int size(const INTEGER& x){ return MP_int_size(x.value);}

//****************************************************************************************
// Addition: returns x + y
//****************************************************************************************

inline INTEGER operator + (const INTEGER& x, const INTEGER& y){
  MP_int_type zvalue;
  MP_int_init(zvalue);
  MP_int_add(x.value,y.value,zvalue);
  return zvalue;
}

inline INTEGER operator + (const INTEGER& x, const int y){
  MP_int_type zvalue;
  MP_int_init(zvalue);
  if (y<0) MP_int_sub_ui(x.value,-y,zvalue);
  else     MP_int_add_ui(x.value,y,zvalue);
  return zvalue;
}

inline INTEGER operator + (const int     x, const INTEGER& y) {return y+x;}

inline INTEGER::INTEGER(const double d){
  MP_int_init(value);
  MP_double_to_INTEGER(d,value);
}

inline INTEGER::INTEGER(const INTEGER& i){
  MP_int_init(value);
  MP_int_duplicate_wo_init(i.value, value);
}


//****************************************************************************************
// Constructing INTEGER from character string
// The base is set to 10 (decimal)
//****************************************************************************************

inline INTEGER::INTEGER(const char* s){
  MP_int_init(value);
  MP_string_to_INTEGER(s,value,10);
}

inline INTEGER::INTEGER(const std::string s){
  MP_int_init(value);
  MP_string_to_INTEGER(s.c_str(),value,10);
}





//****************************************************************************************
// Power: return  = x ^ n
//****************************************************************************************

inline INTEGER operator ^ (const INTEGER& x, const int n)
{
  MP_int_type zvalue;
  MP_int_init(zvalue);
  MP_int_power_i(x.value,n,zvalue);
  return zvalue;
}

//****************************************************************************************
// Power: return = x ^ y, where 0<=y<= (1u<<32)
//****************************************************************************************

inline INTEGER operator ^ (const INTEGER& x, const INTEGER& y)
{
   if (y==0 || x==1) return 1;
   if (y==1) return x;
   if( y> (int((1u<<31)-1 )) ) throw iRRAM_Numerical_Exception(iRRAM_integer_error);
   if(sign(y)<0 ) throw iRRAM_Numerical_Exception(iRRAM_integer_error);
   return x^(unsigned int)(MP_INTEGER_to_int(y.value));
}


//****************************************************************************************
// Shifter: returns x << n
// shifts n bits to the left
//****************************************************************************************

inline INTEGER operator << (const INTEGER& x, const int n){
  return scale(x,n);
}

//****************************************************************************************
// Shifter: returns x >> n
// shifts n bits to the right
//****************************************************************************************

inline INTEGER operator >> (const INTEGER& x, const int n){
  return scale(x,-n);
}




//****************************************************************************************
// Addition: returns x += y
//****************************************************************************************

inline INTEGER& operator += (INTEGER& x, const INTEGER& y){
  MP_int_add(x.value,y.value,x.value);
  return x;
}

inline INTEGER& operator += (INTEGER& x, const int n ){ return x=x+n; }

//****************************************************************************************
// Subtraction: returns x - y
//****************************************************************************************

inline INTEGER operator - (const INTEGER& x, const INTEGER& y){
  MP_int_type zvalue;
  MP_int_init(zvalue);
  MP_int_sub(x.value,y.value,zvalue);
  return zvalue;
}

inline INTEGER operator - (const INTEGER& x, const int y){
  MP_int_type zvalue;
  MP_int_init(zvalue);
  if(y<0) MP_int_add_ui(x.value,-y,zvalue);
    else  MP_int_sub_ui(x.value,y,zvalue);
  return zvalue;
}

inline INTEGER operator - (const int x, const INTEGER& y){
  return -(y-x);
}

//****************************************************************************************
// Subtraction: returns x -= y
//****************************************************************************************

inline INTEGER& operator -= (INTEGER& x, const INTEGER& y){
  MP_int_sub(x.value,y.value,x.value);
  return x;
}

inline INTEGER& operator -= (INTEGER& x, const int  n){ return x=x-n; }

//****************************************************************************************
// Negative: returns -x
//****************************************************************************************

inline INTEGER operator - (const INTEGER& x){
  MP_int_type zvalue;
  INTEGER y=INTEGER(0);
  MP_int_init(zvalue);
  MP_int_sub(y.value,x.value,zvalue);
  return zvalue;
}

//****************************************************************************************
// Multiplication: returns x * y
//****************************************************************************************

inline INTEGER operator * (const INTEGER& x, const INTEGER& y){
  MP_int_type zvalue;
  MP_int_init(zvalue);
  MP_int_mul(x.value,y.value,zvalue);
  return zvalue;
}

inline INTEGER operator * (const INTEGER& x, const int y){
  MP_int_type zvalue;
  MP_int_init(zvalue);
  MP_int_mul_si(x.value,y,zvalue);
  return(zvalue);
}

inline INTEGER operator * (const int x, const INTEGER& y){
  MP_int_type zvalue;
  MP_int_init(zvalue);
  MP_int_mul_si(y.value,x,zvalue);
  return(zvalue);
}

//****************************************************************************************
// Multiplication: returns x *= y
//****************************************************************************************

inline INTEGER& operator *= (INTEGER& x, const INTEGER& y){
  MP_int_mul(x.value,y.value,x.value);
  return x;
}

inline INTEGER& operator *= (INTEGER& x, const int n ){
  MP_int_mul_si(x.value,n,x.value);
  return x; }

//****************************************************************************************
// Division: returns x / y
//****************************************************************************************


inline INTEGER operator / (const INTEGER& x, const INTEGER& y){
  MP_int_type zvalue;
  MP_int_init(zvalue);
  MP_int_div(x.value,y.value,zvalue);
  return zvalue;
}


inline INTEGER operator / (const INTEGER& x, const int n){
  MP_int_type zvalue;
  MP_int_init(zvalue);
  if(n<0){
    MP_int_div_ui(x.value,-n,zvalue);
    MP_int_mul_si(zvalue,-1,zvalue);
  }
  else MP_int_div_ui(x.value,n,zvalue);
  return zvalue;
}

inline INTEGER operator / (const int      x, const INTEGER& y) {return INTEGER(x)/y;}

//****************************************************************************************
// Division: returns x /= y
//****************************************************************************************

inline INTEGER& operator /= (INTEGER& x, const INTEGER& y){
  MP_int_div(x.value,y.value,x.value);
  return x;
}

inline INTEGER& operator /= (INTEGER& x, const int  n){ return x=x/n; }

//****************************************************************************************
// Modulo: returns x % y
//****************************************************************************************

inline INTEGER operator % (const INTEGER& x, const INTEGER& y){
  MP_int_type zvalue;
  MP_int_init(zvalue);
  MP_int_modulo(x.value,y.value,zvalue);
  return zvalue;
}


//****************************************************************************************
// Square-root: returns SQRT(x)
// Returns the truncated square-root of the given argument
//****************************************************************************************

inline INTEGER sqrt (const INTEGER& x)
{
	MP_int_type zvalue;
	MP_int_init(zvalue);
	MP_int_sqrt(x.value, zvalue);
	return zvalue;
}

//****************************************************************************************
// Absolute value: |x|
// returns the positive part of the object
// 1. argument: INTEGER
// return value: INTEGER
//****************************************************************************************

inline INTEGER abs (const INTEGER& x)
{
  MP_int_type zvalue;
  MP_int_init(zvalue);
  MP_int_abs(x.value,zvalue);
  return zvalue;
}


//****************************************************************************************
// boolean operators
// Last change: 2004-12-20
//****************************************************************************************

//****************************************************************************************
// Less than: x < y
//****************************************************************************************

inline bool operator < (const INTEGER& x, const INTEGER& y){
  return (MP_int_compare(x.value,y.value) < 0 );
}
inline bool operator <  (const INTEGER& x, const int      y){ return x<INTEGER(y) ;}
inline bool operator <  (const int      x, const INTEGER& y){ return INTEGER(x)<y ;}

inline bool operator <= (const INTEGER& x, const INTEGER& y){ return !(y<x) ;}
inline bool operator <= (const INTEGER& x, const int      y){ return !(y<x) ;}
inline bool operator <= (const int      x, const INTEGER& y){ return !(y<x) ;}

inline bool operator >  (const INTEGER& x, const INTEGER& y){ return y<x ;}
inline bool operator >  (const INTEGER& x, const int      y){ return y<x ;}
inline bool operator >  (const int      x, const INTEGER& y){ return y<x ;}

inline bool operator >= (const INTEGER& x, const INTEGER& y){ return !(x<y) ;}
inline bool operator >= (const INTEGER& x, const int      y){ return !(x<y) ;}
inline bool operator >= (const int      x, const INTEGER& y){ return !(x<y) ;}



inline bool operator == (const INTEGER& x, const INTEGER& y){
  return (MP_int_compare(x.value,y.value)==0);
}

inline bool operator == (const INTEGER& x, const int  i){ return x==INTEGER(i);}
inline bool operator == (const int      x, const INTEGER& y){ return (y==x) ;}

inline bool operator != (const INTEGER& x, const INTEGER& y){ return !(x==y) ;} 
inline bool operator != (const INTEGER& x, const int      y){ return !(x==y) ;}
inline bool operator != (const int      x, const INTEGER& y){ return !(x==y) ;}


//*****************************************************************************
// Function swrite
// output function for integer 
// 1. argument: INTEGER
// 2. argument: int integer : width
//*****************************************************************************

inline std::string swrite(const INTEGER& x, const int w){
     char* erg= MP_int_swritee(x.value,w);
     std::string result=erg;
     free(erg);
     return result;
}

//*****************************************************************************
// Function swrite
// output function for integer 
// 1. argument: INTEGER
//*****************************************************************************

inline std::string swrite(const INTEGER& x){
     char* erg= MP_int_sprintf(x.value);
     std::string result=erg;
     free(erg);
     return result;
}

} /* ! namespace iRRAM */

#endif /* ! iRRAM_INTEGER_H */
