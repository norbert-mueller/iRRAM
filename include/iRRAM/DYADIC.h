/*

iRRAM_DYADIC.h -- header file for the DYADIC part of the iRRAM library
 
Copyright (C) 2001-2004 Norbert Mueller
 
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

#ifndef iRRAM_DYADIC_H
#define iRRAM_DYADIC_H

namespace iRRAM {

  extern __thread  int iRRAM_DYADIC_precision;

class DYADIC
{
public:

friend class DYADIC_precision;
static int getprec() {return iRRAM_DYADIC_precision;};
// Constructors: -------------------------------

DYADIC ();
DYADIC (const int i);
DYADIC (const double y);
DYADIC (const INTEGER& y);
DYADIC (const DYADIC& y);

// Copy Constructor: ---------------------------

DYADIC&   operator = (const DYADIC& y);

// Destructor: ---------------------------------

~DYADIC();

// Standard Arithmetic: ------------------------

friend DYADIC  ADD  (const DYADIC&  x, const DYADIC& y, int p);
friend DYADIC  operator +  (const DYADIC&  x, const DYADIC& y);
friend DYADIC  operator +  (const int      x, const DYADIC& y);
friend DYADIC  operator +  (const DYADIC&  x, const int y);
friend DYADIC  operator +  (const double   x, const DYADIC& y);
friend DYADIC  operator +  (const DYADIC&  x, const double y);
friend DYADIC  operator +  (const INTEGER& x, const DYADIC& y);
friend DYADIC  operator +  (const DYADIC&  x, const INTEGER& y);

friend DYADIC  SUB  (const DYADIC&  x, const DYADIC& y, int p);
friend DYADIC  operator -  (const DYADIC&  x, const DYADIC& y);
friend DYADIC  operator -  (const int      x, const DYADIC& y);
friend DYADIC  operator -  (const DYADIC&  x, const int y);
friend DYADIC  operator -  (const double   x, const DYADIC& y);
friend DYADIC  operator -  (const DYADIC&  x, const double y);
friend DYADIC  operator -  (const INTEGER& x, const DYADIC& y);
friend DYADIC  operator -  (const DYADIC&  x, const INTEGER& y);

friend DYADIC  operator -  (const DYADIC& x);

friend DYADIC  MULT (const DYADIC&  x, const DYADIC& y, int p);
friend DYADIC  operator *  (const DYADIC&  x, const DYADIC& y);
friend DYADIC  operator *  (const int      x, const DYADIC& y);
friend DYADIC  operator *  (const DYADIC&  x, const int y);
friend DYADIC  operator *  (const double   x, const DYADIC& y);
friend DYADIC  operator *  (const DYADIC&  x, const double y);
friend DYADIC  operator *  (const INTEGER& x, const DYADIC& y);
friend DYADIC  operator *  (const DYADIC&  x, const INTEGER& y);

friend DYADIC  DIV  (const DYADIC&  x, const DYADIC& y, int p);
friend DYADIC  operator /  (const DYADIC&  x, const DYADIC&  y);
friend DYADIC  operator /  (const int      x, const DYADIC&  y);
friend DYADIC  operator /  (const DYADIC&  x, const int      y);
friend DYADIC  operator /  (const double   x, const DYADIC&  y);
friend DYADIC  operator /  (const DYADIC&  x, const double   y);
friend DYADIC  operator /  (const INTEGER& x, const DYADIC&  y);
friend DYADIC  operator /  (const DYADIC&  x, const INTEGER& y);

// Comparisons: --------------------------------

friend bool    operator <  (const DYADIC&  x, const DYADIC&  y);
friend bool    operator <  (const DYADIC&  x, const int      y);
friend bool    operator <  (const int      x, const DYADIC&  y);
friend bool    operator <  (const DYADIC&  x, const double   y);
friend bool    operator <  (const double   x, const DYADIC&  y);
friend bool    operator <  (const DYADIC&  x, const INTEGER& y);
friend bool    operator <  (const INTEGER& x, const DYADIC&  y);


friend bool    operator >  (const DYADIC&  x, const DYADIC&  y);
friend bool    operator >  (const DYADIC&  x, const int      y);
friend bool    operator >  (const int      x, const DYADIC&  y);
friend bool    operator >  (const DYADIC&  x, const double   y);
friend bool    operator >  (const double   x, const DYADIC&  y);
friend bool    operator >  (const DYADIC&  x, const INTEGER& y);
friend bool    operator >  (const INTEGER& x, const DYADIC&  y);


friend bool    operator <= (const DYADIC&  x, const DYADIC&  y);
friend bool    operator <= (const DYADIC&  x, const int      y);
friend bool    operator <= (const int      x, const DYADIC&  y);
friend bool    operator <= (const DYADIC&  x, const double   y);
friend bool    operator <= (const double   x, const DYADIC&  y);
friend bool    operator <= (const DYADIC&  x, const INTEGER& y);
friend bool    operator <= (const INTEGER& x, const DYADIC&  y);


friend bool    operator >= (const DYADIC&  x, const DYADIC&  y);
friend bool    operator >= (const DYADIC&  x, const int      y);
friend bool    operator >= (const int      x, const DYADIC&  y);
friend bool    operator >= (const DYADIC&  x, const double   y);
friend bool    operator >= (const double   x, const DYADIC&  y);
friend bool    operator >= (const DYADIC&  x, const INTEGER& y);
friend bool    operator >= (const INTEGER& x, const DYADIC&  y);


friend bool    operator == (const DYADIC&  x, const DYADIC&  y);
friend bool    operator == (const DYADIC&  x, const int      y);
friend bool    operator == (const int      x, const DYADIC&  y);
friend bool    operator == (const DYADIC&  x, const double   y);
friend bool    operator == (const double   x, const DYADIC&  y);
friend bool    operator == (const DYADIC&  x, const INTEGER& y);
friend bool    operator == (const INTEGER& x, const DYADIC&  y);


friend bool    operator != (const DYADIC&  x, const DYADIC&  y);
friend bool    operator != (const DYADIC&  x, const int      y);
friend bool    operator != (const int      x, const DYADIC&  y);
friend bool    operator != (const DYADIC&  x, const double   y);
friend bool    operator != (const double   x, const DYADIC&  y);
friend bool    operator != (const DYADIC&  x, const INTEGER& y);
friend bool    operator != (const INTEGER& x, const DYADIC&  y);

// Output: -------------------------------------

friend std::string  swrite      (const DYADIC& x, const int w);

// miscellaneous: ------------------------------

friend int     size        (const DYADIC& x); 
friend DYADIC  abs         (const DYADIC& x); 
friend DYADIC  scale       (const DYADIC& x, const int k);

// coexistence with other classes: -------------

friend class REAL;
friend class INTEGER;
friend class RATIONAL;

friend DYADIC  approx      (const REAL& x, const int p);

INTEGER as_INTEGER();

// implementational issues: --------------------

MP_type    value; 


private:

DYADIC(MP_type  y);

};

class DYADIC_precision{
public:
DYADIC_precision(int p)
  {precision=iRRAM_DYADIC_precision;iRRAM_DYADIC_precision=p;}
~DYADIC_precision()
  {iRRAM_DYADIC_precision=precision;};
private:
int precision;
};

} /* ! namespace iRRAM */

#endif
