/*

iRRAM_COMPLEX.h -- header file for the COMPLEX number part of the iRRAM library
 
Copyright (C) 2003/2006 Norbert Mueller
 
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

#ifndef iRRAM_COMPLEX_H
#define iRRAM_COMPLEX_H

namespace iRRAM {

class COMPLEX
{
public:

// Constructors: -------------------------------

COMPLEX();
COMPLEX(const int       real_part);
COMPLEX(const double    real_part);
COMPLEX(const INTEGER&  real_part);
COMPLEX(const DYADIC&   real_part);
COMPLEX(const RATIONAL& real_part);
COMPLEX(const REAL&     real_part);

COMPLEX(const REAL& real_part, const REAL& imag_part);
COMPLEX(const COMPLEX& z); 

// Copy Constructor: ---------------------------

COMPLEX&   operator = (const COMPLEX& y);

// Standard Arithmetic: ------------------------

friend COMPLEX  operator +  (const COMPLEX& x,
                             const COMPLEX& y);
friend COMPLEX  operator -  (const COMPLEX& x,
                             const COMPLEX& y);
friend COMPLEX  operator -  (const COMPLEX& x);
friend COMPLEX  operator *  (const COMPLEX& x,
                             const COMPLEX& y);
friend COMPLEX  operator /  (const COMPLEX& x,
                             const COMPLEX& y);

friend REAL real(const COMPLEX& z);
friend REAL imag(const COMPLEX& z);
friend REAL abs (const COMPLEX& z);

friend REAL arg (const COMPLEX& z);
// (1) arg(z) is not defined for z=0, 
// (2) arg(z) is multivalued giving values between -pi  and  pi + 2^(-10),
//     so for values z close to the negative half of the real axis,
//     arg(z) may give one of two possible values!  

void adderror (sizetype error);
void seterror (sizetype error);
void geterror (sizetype& error) const;
REAL _real,_imag;
};

/****************************************************************************/
// complex functions
/****************************************************************************/

COMPLEX  sqrt(const COMPLEX& z);
// defined for any complex number:

COMPLEX csqrt(const COMPLEX& z);
// only defined for |z| > 0 (faster if |z| >> 0 ) :

COMPLEX log(const COMPLEX& z);
// log is multivalued and gives the principal value,
// except for arguments z close to the negative half of the real axis
// (compare to arg(z) !)

COMPLEX exp(const COMPLEX& z);

/****************************/
/* trigonometric functions  */
/****************************/

COMPLEX sin(const COMPLEX& z);
COMPLEX cos(const COMPLEX& z);
COMPLEX tan(const COMPLEX& z);
COMPLEX cotan(const COMPLEX& z);
COMPLEX sec(const COMPLEX& z);
COMPLEX cosec(const COMPLEX& z);

COMPLEX sinh(const COMPLEX& z);
COMPLEX cosh(const COMPLEX& z);
COMPLEX tanh(const COMPLEX& z);
COMPLEX coth(const COMPLEX& z);
COMPLEX sech(const COMPLEX& z);
COMPLEX cosech(const COMPLEX& z);

COMPLEX asin(const COMPLEX& z);
COMPLEX acos(const COMPLEX& z);
COMPLEX atan(const COMPLEX& z);
COMPLEX acotan(const COMPLEX& z);
COMPLEX asec(const COMPLEX& z);
COMPLEX acosec(const COMPLEX& z);

COMPLEX asinh(const COMPLEX& z);
COMPLEX acosh(const COMPLEX& z);
COMPLEX atanh(const COMPLEX& z);
COMPLEX acoth(const COMPLEX& z);
COMPLEX asech(const COMPLEX& z);
COMPLEX acosech(const COMPLEX& z);


} // namespace iRRAM

#endif
