/*

convert.cc -- a few conversion functions for the iRRAM library
 
Copyright (C) 2002/2006 Norbert Mueller / Tom van Diessen
 
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
#include <cstdlib>
#include <cstdarg>
#include <cstring>

#include <iRRAM/core.h>

#if iRRAM_BACKEND_MPFR
	#include "MPFR_ext.h"
#else
	#error "Currently no further backends define!"
#endif

namespace iRRAM {

INTEGER DYADIC::as_INTEGER()
{
	INTEGER z;
	MP_mp_to_INTEGER(this->value,z.value);
	return z;
}

//Conversion from INTEGER to smaller types
INTEGER::operator int() const {;
  return MP_INTEGER_to_int(value);
}

//Conversions from REAL to DYADIC with absolute precision
DYADIC REAL::as_DYADIC(const int p) const {
  return approx(*this,p);
}

DYADIC REAL::as_DYADIC () const { return approx(*this,DYADIC::getprec());}


//Conversion from REAL to double with a relative precision of p bits
double REAL::as_double(const int p) const {
  if ( bound(*this,-1150) ) return 0.0;
  int s=size(*this);
  DYADIC d=approx(*this,s-p-2);
  return MP_mp_to_double(d.value);
}

} // namespace iRRAM
