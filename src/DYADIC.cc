/*

DYADIC.cc -- interface implementation of dyadic numbers for the iRRAM library
 
Copyright (C) 2003 Norbert Mueller
 
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

#include <iRRAM/core.h>

#if iRRAM_BACKEND_MPFR
	#include "MPFR_ext.h"
#else
	#error "Currently no further backends defined!"
#endif

namespace iRRAM {

DYADIC::DYADIC() {
   MP_init(value);
   MP_int_to_mp(0,value);
}

DYADIC::DYADIC(MP_type  y) {
  value=y;
}

DYADIC::DYADIC(const int i) {
   MP_init(value);MP_int_to_mp(i,value);
}

DYADIC::DYADIC(const INTEGER& i) {
   MP_init(value);MP_INTEGER_to_mp(i.value,value);
}

DYADIC::DYADIC(const DYADIC& y) { 
  MP_duplicate_w_init(y.value,value);
}

DYADIC::DYADIC(const double x) {
   MP_init(value);MP_double_to_mp(x,value);
}

DYADIC::~DYADIC() { 
  MP_clear(value);
}

DYADIC ADD (const DYADIC& x, const DYADIC& y, const int p) {
  MP_type zvalue;
  MP_init(zvalue);
  MP_add(x.value,y.value,zvalue,p);
  return zvalue;
}

DYADIC operator + (const DYADIC& x, const DYADIC& y) {
  MP_type zvalue;
  MP_init(zvalue);
  MP_add(x.value,y.value,zvalue,iRRAM_DYADIC_precision);
  return zvalue;
}

DYADIC  operator +  (const int      x, const DYADIC&  y){return DYADIC(x)+y;}
DYADIC  operator +  (const DYADIC&  x, const int      y){return x+DYADIC(y);}
DYADIC  operator +  (const INTEGER& x, const DYADIC&  y){return DYADIC(x)+y;}
DYADIC  operator +  (const DYADIC&  x, const INTEGER& y){return x+DYADIC(y);}
DYADIC  operator +  (const double   x, const DYADIC&  y){return DYADIC(x)+y;}
DYADIC  operator +  (const DYADIC&  x, const double   y){return x+DYADIC(y);}


DYADIC SUB (const DYADIC& x, const DYADIC& y,const int p) {
  MP_type zvalue;
  MP_init(zvalue);
  MP_sub(x.value,y.value,zvalue,p);
  return zvalue;
}

DYADIC operator - (const DYADIC& x, const DYADIC& y) {
  MP_type zvalue;
  MP_init(zvalue);
  MP_sub(x.value,y.value,zvalue,iRRAM_DYADIC_precision);
  return zvalue;
}

DYADIC  operator -  (const int      x, const DYADIC&  y){return DYADIC(x)-y;}
DYADIC  operator -  (const DYADIC&  x, const int      y){return x-DYADIC(y);}
DYADIC  operator -  (const INTEGER& x, const DYADIC&  y){return DYADIC(x)-y;}
DYADIC  operator -  (const DYADIC&  x, const INTEGER& y){return x-DYADIC(y);}
DYADIC  operator -  (const double   x, const DYADIC&  y){return DYADIC(x)-y;}
DYADIC  operator -  (const DYADIC&  x, const double   y){return x-DYADIC(y);}


DYADIC operator - (const DYADIC& x) {
  DYADIC y;
  MP_type zvalue;
  MP_init(zvalue);
  MP_sub(y.value,x.value,zvalue,iRRAM_DYADIC_precision);
  return zvalue;
}


DYADIC MULT (const DYADIC& x, const DYADIC& y, const int p) {
  MP_type zvalue;
  MP_init(zvalue);
  MP_mul(x.value,y.value,zvalue,p);
  return zvalue;
}

DYADIC operator * (const DYADIC& x, const DYADIC& y) {
  MP_type zvalue;
  MP_init(zvalue);
  MP_mul(x.value,y.value,zvalue,iRRAM_DYADIC_precision);
  return zvalue;
}

DYADIC  operator *  (const int      x, const DYADIC&  y){return DYADIC(x)*y;}
DYADIC  operator *  (const DYADIC&  x, const int      y){return x*DYADIC(y);}
DYADIC  operator *  (const INTEGER& x, const DYADIC&  y){return DYADIC(x)*y;}
DYADIC  operator *  (const DYADIC&  x, const INTEGER& y){return x*DYADIC(y);}
DYADIC  operator *  (const double   x, const DYADIC&  y){return DYADIC(x)*y;}
DYADIC  operator *  (const DYADIC&  x, const double   y){return x*DYADIC(y);}


DYADIC DIV (const DYADIC& x, const DYADIC& y,const int p) {
  MP_type zvalue;
  MP_init(zvalue);
  MP_div(x.value,y.value,zvalue,p);
  return zvalue;
}

DYADIC operator / (const DYADIC& x, const DYADIC& y) {
  MP_type zvalue;
  MP_init(zvalue);
  MP_div(x.value,y.value,zvalue,iRRAM_DYADIC_precision);
  return zvalue;
}

DYADIC  operator /  (const int      x, const DYADIC&  y){return DYADIC(x)/y;}
DYADIC  operator /  (const DYADIC&  x, const int      y){return x/DYADIC(y);}
DYADIC  operator /  (const INTEGER& x, const DYADIC&  y){return DYADIC(x)/y;}
DYADIC  operator /  (const DYADIC&  x, const INTEGER& y){return x/DYADIC(y);}
DYADIC  operator /  (const double   x, const DYADIC&  y){return DYADIC(x)/y;}
DYADIC  operator /  (const DYADIC&  x, const double   y){return x/DYADIC(y);}


#ifdef MP_shift
DYADIC scale(const DYADIC& x, const int n) {
  MP_type zvalue;
  MP_init(zvalue);
  MP_shift(x.value,zvalue,n);
  return DYADIC(zvalue);
}
#else
DYADIC scale(const DYADIC& x, int n) {
 exit(1);
//#error this can not be compiled
//#error the implementation is missing for this backend
}
#endif


DYADIC& DYADIC::operator = (const DYADIC& y) {
  MP_duplicate_wo_init(y.value,value);
  return (*this);
}

bool operator <  (const DYADIC& x, const DYADIC& y) {
  int erg=MP_compare((x.value),(y.value));
  return  (erg==-1);
}

bool operator <  (const int      x, const DYADIC&  y) {return DYADIC(x)<y;}
bool operator <  (const DYADIC&  x, const int      y) {return x<DYADIC(y);}
bool operator <  (const double   x, const DYADIC&  y) {return DYADIC(x)<y;}
bool operator <  (const DYADIC&  x, const double   y) {return x<DYADIC(y);}
bool operator <  (const INTEGER& x, const DYADIC&  y) {return DYADIC(x)<y;}
bool operator <  (const DYADIC&  x, const INTEGER  y) {return x<DYADIC(y);}

bool operator >  (const DYADIC&  x, const DYADIC&  y) {return (y<x);}
bool operator >  (const int      x, const DYADIC&  y) {return y<DYADIC(x);}
bool operator >  (const DYADIC&  x, const int      y) {return DYADIC(y)<x;}
bool operator >  (const double   x, const DYADIC&  y) {return y<DYADIC(x);}
bool operator >  (const DYADIC&  x, const double   y) {return DYADIC(y)<x;}
bool operator >  (const INTEGER& x, const DYADIC&  y) {return y<DYADIC(x);}
bool operator >  (const DYADIC&  x, const INTEGER  y) {return DYADIC(y)<x;}

bool operator <= (const DYADIC&  x, const DYADIC&  y) {return !(y<x);}
bool operator <= (const int      x, const DYADIC&  y) {return !(y<DYADIC(x));}
bool operator <= (const DYADIC&  x, const int      y) {return !(DYADIC(y)<x);}
bool operator <= (const double   x, const DYADIC&  y) {return !(y<DYADIC(x));}
bool operator <= (const DYADIC&  x, const double   y) {return !(DYADIC(y)<x);}
bool operator <= (const INTEGER& x, const DYADIC&  y) {return !(y<DYADIC(x));}
bool operator <= (const DYADIC&  x, const INTEGER  y) {return !(DYADIC(y)<x);}

bool operator >= (const DYADIC&  x, const DYADIC&  y) {return !(x<y);}
bool operator >= (const int      x, const DYADIC&  y) {return !(DYADIC(x)<y);}
bool operator >= (const DYADIC&  x, const int      y) {return !(x<DYADIC(y));}
bool operator >= (const double   x, const DYADIC&  y) {return !(DYADIC(x)<y);}
bool operator >= (const DYADIC&  x, const double   y) {return !(x<DYADIC(y));}
bool operator >= (const INTEGER& x, const DYADIC&  y) {return !(DYADIC(x)<y);}
bool operator >= (const DYADIC&  x, const INTEGER  y) {return !(x<DYADIC(y));}

bool operator == (const DYADIC& x, const DYADIC& y) {
  int erg=MP_compare((x.value),(y.value));
  return  (erg==0);
}
bool operator == (const int      x, const DYADIC&  y) {return DYADIC(x)==y;}
bool operator == (const DYADIC&  x, const int      y) {return x==DYADIC(y);}
bool operator == (const double   x, const DYADIC&  y) {return DYADIC(x)==y;}
bool operator == (const DYADIC&  x, const double   y) {return x==DYADIC(y);}
bool operator == (const INTEGER& x, const DYADIC&  y) {return DYADIC(x)==y;}
bool operator == (const DYADIC&  x, const INTEGER  y) {return x==DYADIC(y);}

bool operator != (const DYADIC&  x, const DYADIC&  y) {return !(x==y);}
bool operator != (const int      x, const DYADIC&  y) {return !(DYADIC(x)==y);}
bool operator != (const DYADIC&  x, const int      y) {return !(x==DYADIC(y));}
bool operator != (const double   x, const DYADIC&  y) {return !(DYADIC(x)==y);}
bool operator != (const DYADIC&  x, const double   y) {return !(x==DYADIC(y));}
bool operator != (const INTEGER& x, const DYADIC&  y) {return !(DYADIC(x)==y);}
bool operator != (const DYADIC&  x, const INTEGER  y) {return !(x==DYADIC(y));}

std::string swrite (const DYADIC& x,const int w){
  int width=w;
  if (width<9) width=9;
  char * s=MP_swrite(x.value,width);
  std::string erg=s;
  free(s);
  return erg;
}

int size (const DYADIC& x) {
  int s=MP_size((x.value));
  if ( s <= MP_min) throw iRRAM_Numerical_Exception(iRRAM_underflow_error);
  return s;
}

DYADIC abs (const DYADIC& x) {
  MP_type zvalue;
  MP_init(zvalue);
  MP_abs(x.value,zvalue);
  return zvalue;
}

} // namespace iRRAM
