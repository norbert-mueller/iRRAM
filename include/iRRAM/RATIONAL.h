/*

iRRAM_RATIONAL.h -- declaration of the interface to rational numbers for the iRRAM library
 
Copyright (C) 2005 Norbert Mueller
 
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

#ifndef iRRAM_RATIONAL_H
#define iRRAM_RATIONAL_H

namespace iRRAM {

class RATIONAL
{
public:

friend class INTEGER;
friend class DYADIC;
friend class REAL;

/****** Constructors ******/

RATIONAL(MP_rat_type y);

RATIONAL();
RATIONAL(const int i);
RATIONAL(const double d);
RATIONAL(const char* s);
RATIONAL(const INTEGER& y);
//RATIONAL(const DYADIC& y); 
RATIONAL(const RATIONAL& y); 

RATIONAL(const int i, const int j);
RATIONAL(const INTEGER& x, const INTEGER& y);

/****** Copy constructor ******/

RATIONAL& operator = (const RATIONAL& y);
RATIONAL& operator = (const INTEGER& y);
RATIONAL& operator = (const int y);

/****** Destructor ******/

~RATIONAL();

/****** Standard arithmetic ******/

friend RATIONAL  operator +  (const RATIONAL& x, const RATIONAL& y);
friend RATIONAL  operator +  (const RATIONAL& x, const int	 y);
friend RATIONAL  operator +  (const RATIONAL& x, const INTEGER&  y);
friend RATIONAL  operator +  (const int       x, const RATIONAL& y);
friend RATIONAL  operator +  (const INTEGER&  x, const RATIONAL& y);

friend RATIONAL& operator += (      RATIONAL& x, const RATIONAL& y);
friend RATIONAL& operator += (      RATIONAL& x, const INTEGER&  y);
friend RATIONAL& operator += (      RATIONAL& x, const int	 y);


friend RATIONAL  operator -  (const RATIONAL& x, const RATIONAL& y);
friend RATIONAL  operator -  (const RATIONAL& x, const int	 y);
friend RATIONAL  operator -  (const RATIONAL& x, const INTEGER&  y);
friend RATIONAL  operator -  (const int       x, const RATIONAL& y);
friend RATIONAL  operator -  (const INTEGER&  x, const RATIONAL& y);

friend RATIONAL& operator -= (      RATIONAL& x, const RATIONAL& y);
friend RATIONAL& operator -= (      RATIONAL& x, const INTEGER&  y);
friend RATIONAL& operator -= (      RATIONAL& x, const int	 y);

friend RATIONAL  operator -  (const RATIONAL& x);

friend RATIONAL  operator *  (const RATIONAL& x, const RATIONAL& y);
friend RATIONAL  operator *  (const RATIONAL& x, const int	 y);
friend RATIONAL  operator *  (const RATIONAL& x, const INTEGER&  y);
friend RATIONAL  operator *  (const int       x, const RATIONAL& y);
friend RATIONAL  operator *  (const INTEGER&  x, const RATIONAL& y);

friend RATIONAL& operator *= (      RATIONAL& x, const RATIONAL& y);
friend RATIONAL& operator *= (      RATIONAL& x, const INTEGER&  y);
friend RATIONAL& operator *= (      RATIONAL& x, const int	 y);

friend RATIONAL  operator /  (const RATIONAL& x, const RATIONAL& y);
friend RATIONAL  operator /  (const RATIONAL& x, const int	 y);
friend RATIONAL  operator /  (const RATIONAL& x, const INTEGER&  y);
friend RATIONAL  operator /  (const int       x, const RATIONAL& y);
friend RATIONAL  operator /  (const INTEGER&  x, const RATIONAL& y);

friend RATIONAL& operator /= (      RATIONAL& x, const RATIONAL& y);
friend RATIONAL& operator /= (      RATIONAL& x, const INTEGER&  y);
friend RATIONAL& operator /= (      RATIONAL& x, const int	 y);

friend RATIONAL	 scale  	 (const RATIONAL& x, const int k);
friend RATIONAL	 abs		 (const RATIONAL& x);

friend INTEGER  numerator	 (const RATIONAL& x);
friend INTEGER  denominator	 (const RATIONAL& x);
friend int 	sign	 	 (const RATIONAL& x);

/****** Comparisons ******/

friend bool 	operator <      (const RATIONAL& x, const RATIONAL& y);
friend bool 	operator <      (const RATIONAL& x, const INTEGER&  y);
friend bool 	operator <      (const INTEGER&  x, const RATIONAL&  y);
friend bool 	operator <      (const RATIONAL& x, const int       y);
friend bool 	operator <      (const int       x, const INTEGER&  y);

friend bool 	operator <=     (const RATIONAL& x, const RATIONAL& y);
friend bool 	operator <=     (const RATIONAL& x, const INTEGER&  y);
friend bool 	operator <=     (const INTEGER&  x, const RATIONAL&  y);
friend bool 	operator <=     (const RATIONAL& x, const int       y);
friend bool 	operator <=     (const int       x, const INTEGER&  y);

friend bool 	operator >      (const RATIONAL& x, const RATIONAL& y);
friend bool 	operator >      (const RATIONAL& x, const INTEGER&  y);
friend bool 	operator >      (const INTEGER&  x, const RATIONAL&  y);
friend bool 	operator >      (const RATIONAL& x, const int       y);
friend bool 	operator >      (const int       x, const INTEGER&  y);

friend bool 	operator >=     (const RATIONAL& x, const RATIONAL& y);
friend bool 	operator >=     (const RATIONAL& x, const INTEGER&  y);
friend bool 	operator >=     (const INTEGER&  x, const RATIONAL&  y);
friend bool 	operator >=     (const RATIONAL& x, const int       y);
friend bool 	operator >=     (const int       x, const INTEGER&  y);

friend bool 	operator ==     (const RATIONAL& x, const RATIONAL& y);
friend bool 	operator ==     (const RATIONAL& x, const INTEGER&  y);
friend bool 	operator ==     (const INTEGER&  x, const RATIONAL&  y);
friend bool 	operator ==     (const RATIONAL& x, const int       y);
friend bool 	operator ==     (const int       x, const INTEGER&  y);

friend bool 	operator !=     (const RATIONAL& x, const RATIONAL& y);
friend bool 	operator !=     (const RATIONAL& x, const INTEGER&  y);
friend bool 	operator !=     (const INTEGER&  x, const RATIONAL&  y);
friend bool 	operator !=     (const RATIONAL& x, const int       y);
friend bool 	operator !=     (const int       x, const INTEGER&  y);

/****** String conversion ******/

friend std::string    swrite          (const RATIONAL& x);
friend std::string    swrite          (const RATIONAL& x, const int w);

/****** Private ******/
private:

MP_rat_type value;
RATIONAL(MP_int_type y);
};

} /* ! namespace iRRAM */

#endif
