/*

COMPLEX.cc -- implementation of complex numbers for the iRRAM library
 
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
#include <iRRAM/core.h>

namespace iRRAM {

COMPLEX::COMPLEX()
{}

COMPLEX::COMPLEX(const int x)
{
  _real = x;
}

COMPLEX::COMPLEX(const double x)
{
  _real = x;
}

COMPLEX::COMPLEX(const INTEGER& x)
{
  _real = x;
}

COMPLEX::COMPLEX(const DYADIC& x)
{
  _real = x;
}

COMPLEX::COMPLEX(const RATIONAL& x)
{
  _real = x;
}

COMPLEX::COMPLEX(const REAL& x)
{
  _real = x;
}

COMPLEX::COMPLEX(const REAL& x, const REAL& y)
{
  _real = x;
  _imag = y;
}

COMPLEX::COMPLEX(const COMPLEX& z)
{
  _real = z._real;
  _imag = z._imag;
}

COMPLEX& COMPLEX::operator = (const COMPLEX& z) {
  _real=z._real;
  _imag=z._imag;
  return(*this);
}



void COMPLEX::adderror (sizetype error)
{
  this->_real.adderror(error);
  this->_imag.adderror(error);
}

void COMPLEX::seterror (sizetype error)
{
  this->_real.seterror(error);
  this->_imag.seterror(error);
}

void COMPLEX::geterror (sizetype& error) const
{
  sizetype lerror;
  this->_real.geterror(error);
  this->_imag.geterror(lerror);
  sizetype_max(error,error,lerror);
}


COMPLEX operator + (const COMPLEX& x, const COMPLEX& y)
{
  return COMPLEX(x._real+y._real,
                 x._imag+y._imag);
}

COMPLEX operator - (const COMPLEX& x, const COMPLEX& y)
{
  return COMPLEX(x._real-y._real,
                 x._imag-y._imag);
}

COMPLEX operator - (const COMPLEX& x)
{
  return COMPLEX(-x._real,
                 -x._imag);
}

COMPLEX operator * (const COMPLEX& x, const COMPLEX& y)
{
  return COMPLEX(x._real*y._real-
                 x._imag*y._imag,
                 x._real*y._imag+
                 x._imag*y._real);
}

COMPLEX operator / (const COMPLEX& x, const COMPLEX& y)
{ 
  REAL d;
  d=y._real*y._real +y._imag*y._imag;
  return COMPLEX( (x._real*y._real+
                   x._imag*y._imag)/d,
                  (x._imag*y._real-
                   x._real*y._imag)/d);
}

REAL real(const COMPLEX& z)
{ return z._real; }

REAL imag(const COMPLEX& z)
{ return z._imag; }

REAL abs(const COMPLEX& z)
{ return sqrt(z._real*z._real+z._imag*z._imag); }


COMPLEX c_sqrt_approx(int p, int* choice, const COMPLEX& z)
{
  REAL a,b,c,d,r;
  COMPLEX y;
  r= abs (z);
  a= real(z);
  b= imag(z);
  c= sqrt( (r+a) / 2 );
  d= sqrt( (r-a) / 2 );
  if ( *choice == 0 ) {
    if ( !bound(r, 2*p) ) {
      if ( ! bound(b,p-10) ) {
        if ( b > REAL(0) )  *choice=1; 
                 else  *choice=2; 
      } else {
        if ( a > REAL(0) )  *choice=3; 
                 else  *choice=4; 
      }
    }
  }

  switch ( *choice ) {
    case 0:    y = COMPLEX (  0      ,  0     );    break;
    case 1:    y = COMPLEX (  c      ,  d     );    break;
    case 2:    y = COMPLEX (  c      , -d     );    break;
    case 3:    y = COMPLEX (  c      ,  b/2/c );    break;
    case 4:    y = COMPLEX (  b/2/d  ,  d     );    break;
  }
  return y;
}

COMPLEX  sqrt(const COMPLEX& z)
{
  return limit_mv(c_sqrt_approx,z);
}

COMPLEX csqrt(const COMPLEX& z)
{
  REAL a,b,c,d,r,s;
  COMPLEX y;
  int choice=0;
 
  r= abs (z);
  a= real(z);
  b= imag(z);
  c= sqrt( (r+a) / 2 );
  d= sqrt( (r-a) / 2 );

  if ( ! bound(b,size(r)-3) ) {
    if ( b > REAL(0) )  choice=1;  else  choice=2; 
  } else {
    if ( a > REAL(0) )  choice=3;  else  choice=4; 
  }

  switch ( choice ) {
  case 1:    y = COMPLEX ( c      ,   d    ); break;
  case 2:    y = COMPLEX ( c      ,  -d    ); break;
  case 3:    y = COMPLEX ( -c     , -b/2/c ); break;
  case 4:    y = COMPLEX ( b/2/d ,   d    ); break;
  }
  return y;
}

REAL arg(const COMPLEX& z){
  REAL z_abs=abs(z);
  COMPLEX z_norm= z/COMPLEX(z_abs);
  REAL border=0.5;
  REAL phi;
  switch (choose(real(z_norm)>border,
                 imag(z_norm)>border,
		 imag(z_norm)<-border,
		 real(z_norm)<-border)
	){
	case 1:phi= asin(imag(z_norm)); break;;
	case 2:phi= acos(real(z_norm)); break;;
	case 3:phi= -acos(real(z_norm)); break;;
	case 4:phi= pi()-asin(imag(z_norm)); break;;
  }
  if (positive(phi-pi(),-10)) phi=phi-2*pi();
  return phi;
}

COMPLEX log(const COMPLEX& z){
  REAL z_abs=abs(z);
  return COMPLEX(log(abs(z)),arg(z));
}

//**************************************************************************************
// added: 28.01.2001
// Exponential-Funktion
// exponential function
// exp(z)=e^re(z)*cos(im(z)+i*(e^re(z)*sin(im(z)))
// *************************************************************************************

COMPLEX exp (const COMPLEX& z)
{
	REAL scalar = exp(real(z));

	return COMPLEX (scalar * cos( imag(z) ), scalar * sin( imag(z) ) );

}



//**************************************************************************************
// added: 28.01.2001
// sinus-Funktion
// sin(z)=(exp(i*z)-exp(-i*z))/(2*i)
//**************************************************************************************

COMPLEX sin (const COMPLEX& z)
{
	REAL scalar, iz_re, iz_im, re1, re2, im1, im2;

	iz_re = -imag(z);
	iz_im = real(z);
	scalar = exp(iz_re);
	re1 = scalar * cos(iz_im);
	im1 = scalar * sin(iz_im);
	scalar = exp(-iz_re);
	re2 = scalar * cos(-iz_im);
	im2 = scalar * sin(-iz_im);
	re1 = re1 - re2;
	im1 = im1 - im2;

	return COMPLEX(REAL(0.5) * im1, REAL(-0.5) * re1);
}

//***************************************************************************************
// added: 28.01.2001
// cosinus-Funktion
// cos(z)=(exp(i*z)+exp(-i*z))/2
//***************************************************************************************

COMPLEX cos (const COMPLEX& z)
{
	REAL scalar, iz_re, iz_im, re1, re2, im1, im2;
	COMPLEX y;

	iz_re = -imag(z);
	iz_im = real(z);
	scalar = exp(iz_re);
	re1 = scalar * cos(iz_im);
	im1 = scalar * sin(iz_im);
	scalar = exp(-iz_re);
	re2 = scalar * cos(-iz_im);
	im2 = scalar * sin(-iz_im);
	re1 = re1 + re2;
	im1 = im1 + im2;

	return COMPLEX(REAL(0.5) * re1, REAL(0.5) * im1);
}

//***************************************************************************************
// added: 28.01.2001
// tangens-Funktion
// tan(z)=(sin(z)/cos(z))
//***************************************************************************************

COMPLEX tan (const COMPLEX& z)
{
	return sin(z) / cos(z);
}

//***************************************************************************************
// added: 28.01.2001
// cotangens-Funktion
// cot(z)=1/tan(z)
//***************************************************************************************

COMPLEX cotan (const COMPLEX& z)
{
	return (COMPLEX(1.0,0.0) / tan(z));
}

//***************************************************************************************
// added: 28.01.2001
// cosec-Funktion
// cosec(z)=1/sin(z)
//***************************************************************************************

COMPLEX cosec (const COMPLEX& z)
{
	return (COMPLEX(1.0,0.0) / sin(z));
}

//***************************************************************************************
// added: 28.01.2001
// sec-Funktion
// sec(z)=1/cos(z)
//***************************************************************************************

COMPLEX sec (const COMPLEX& z)
{
	return (COMPLEX(1.0,0.0) / cos(z));
}

//***************************************************************************************
// added: 28.01.2001
// sinus-hyperbolicus-Funktion
// sinh(z)=(exp(z)-exp(-z))/2
//***************************************************************************************

COMPLEX sinh (const COMPLEX& z)
{
	REAL re, im, re1, im1, re2, im2, scalar;
	
	re = real(z);
	im = imag(z);
	// exp(z)
	scalar = exp(re);
	re1 = scalar * cos(im);
	im1 = scalar * sin(im);
	//exp(-z)
	scalar = exp(-re);
	re2 = scalar * cos(-im);
	im2 = scalar * sin(-im);
	//exp(z)-exp(-z)
	re1 = re1 - re2;
	im1 = im1 - im2;
	
	return COMPLEX(REAL(0.5) * re1, REAL(0.5) * im1);
}

//***************************************************************************************
// added: 28.01.2001
// cosinus-hyperbolicus-Funktion
// cosh(z)=(exp(z)+exp(-z))/2
//***************************************************************************************

COMPLEX cosh (const COMPLEX& z)
{
	REAL re, im, re1, im1, re2, im2, scalar;
	
	re = real(z);
	im = imag(z);
	// exp(z)
	scalar = exp(re);
	re1 = scalar * cos(im);
	im1 = scalar * sin(im);
	//exp(-z)
	scalar = exp(-re);
	re2 = scalar * cos(-im);
	im2 = scalar * sin(-im);
	//exp(z)+exp(-z)
	re1 = re1 + re2;
	im1 = im1 + im2;
	
	return COMPLEX(REAL(0.5) * re1, REAL(0.5) * im1);
}

//***************************************************************************************
// added: 28.01.2001
// tangens-hyperbolicus-Funktion
// tanh(z)=sinh(z)/cosh(z)
//***************************************************************************************

COMPLEX tanh(const COMPLEX& z)
{
	return sinh(z) / cosh(z);
}

//***************************************************************************************
// added: 28.01.2001
// cotangens-hyperbolicus-Funktion
// hyperbolic cotangent function
// coth(z)=(exp(z)+exp(-z))/(exp(z)-exp(-z))
//***************************************************************************************

COMPLEX coth (const COMPLEX& z)
{
	return (exp(z)+exp(-z))/(exp(z)-exp(-z));
}

//***************************************************************************************
// added: 28.01.2001
// sec-hyperbolicus-Funktion
// hyperbolic sec function
// sech(z)=2/(exp(z)+exp(-z))
//***************************************************************************************

COMPLEX sech (const COMPLEX& z)
{
	return COMPLEX(2.0,0.0)/(exp(z)+exp(-z));
}

//***************************************************************************************
// added: 28.01.2001
// cosec-hyperbolicus-Funktion
// hyperbolic cosec function
// cosech(z)=2/(exp(z)-exp(-z))
//***************************************************************************************

COMPLEX cosech (const COMPLEX& z)
{
	return COMPLEX(2.0,0.0)/(exp(z)-exp(-z));
}

//***************************************************************************************
// added: 28.01.2001
// arcus-sinus-Funktion
// arc-sine-function
// asin(z)=-i*log(i*z+sqrt(1-z*z))
//***************************************************************************************

COMPLEX asin (const COMPLEX& z)
{
	REAL re, im;
	COMPLEX scalar;

	// sqrt(1-z*z)
	scalar = sqrt(COMPLEX(1.0,0.0)-z*z);
	
	//i*z+.....
	re = real(scalar) - imag(z);
	im = imag(scalar) + real(z);

	//log(....)*(-i)
	return COMPLEX(im, -log(abs(re)));

}

//***************************************************************************************
// added: 28.01.2001
// arcus-cosinus-Funktion
// arc-cosine-function
// acos(z)=-i*log(z+i*sqrt(1-z*z))
//***************************************************************************************

COMPLEX acos (const COMPLEX& z)
{
	REAL re, im;
	COMPLEX scalar;

	//sqrt(1-z*z)
	scalar = sqrt(COMPLEX(1.0,0.0)-z*z);

	//(....*i)+z
	re = real(z)-imag(scalar);
	im = imag(z)+real(scalar);

	//log(....)*(-i)
	return COMPLEX(im, -log(abs(re)));
}

//***************************************************************************************
// added: 28.01.2001
// arcus-tangens-Funktion
// arc-tangent-function
// atan(z)=-i/2*log((i-z)/i+z))
//***************************************************************************************

COMPLEX atan (const COMPLEX& z)
{
	COMPLEX scalar, i=COMPLEX(0.0,1.0);

	//log((i-z)/(i+z))/2
	scalar = log((i-z)/(i+z))/REAL(2);

	//....*(-i)
	return COMPLEX(imag(scalar), -real(scalar));
}

//***************************************************************************************
// added:
// arcus-cotangens-Funktion
// arc cotangent function
// acotan(z)=
// Information: do not know function yet, returns 0+0i
//***************************************************************************************

COMPLEX acotan(const COMPLEX& z)
{
	return atan(COMPLEX(1.0,0.0)/z);
}

//***************************************************************************************
// added: 28.01.2001
// arcus-sec-Funktion
// arc-sec function
// asec(z)=acos(1/z)
//***************************************************************************************

COMPLEX asec(const COMPLEX& z)
{
	return acos(COMPLEX(1.0,0.0)/z);
}

//***************************************************************************************
// added: 28.01.2001
// arcus-cosec-Funktion
// arc cosec function
// acosec(z)=asin(1/z)
//***************************************************************************************

COMPLEX acosec(const COMPLEX& z)
{
	return asin(COMPLEX(1.0,0.0)/z);
}

//***************************************************************************************
// added: 28.01.2001
// arcus-sinus-hyperbolicus-Funktion
// hyperbolic arc-sine function
// asinh(z)=log(z+sqrt(z*z+1))
//***************************************************************************************

COMPLEX asinh (const COMPLEX& z)
{
	return log(z+sqrt(z*z+COMPLEX(1.0,0.0)));
}

//***************************************************************************************
// added: 28.01.2001
// arcus-cosinus-hyperbolicus-Funktion
// hyperbolic arc-cosine function
// acosh(z)=log(z+sqrt(z*z-1))
//***************************************************************************************

COMPLEX acosh (const COMPLEX& z)
{
	return log(z+sqrt(z*z-COMPLEX(1.0,0.0)));
}

//***************************************************************************************
// added: 28.01.2001
// arcus-tangens-hyperbolicus-Funktion
// hyperbolic arc-tangent function
// atanh(z)=1/2*log((1+z)/1-z))
//***************************************************************************************

COMPLEX atanh (const COMPLEX& z)
{
	return COMPLEX(0.5,0.0)*log((COMPLEX(1.0,0.0)+z)/(COMPLEX(1.0,0.0)-z));
}

//***************************************************************************************
// added: 28.01.2001
// arcus-cotangens-hyperbolicus-Funktion
// hyperbolic arc-cotangent function
// acoth(z)=1/2*log((1-z)/(1+z))
//***************************************************************************************

COMPLEX acoth (const COMPLEX& z)
{
	return COMPLEX(0.5,0.0) * log( (COMPLEX(1.0,0.0)-z) / (COMPLEX(1.0,0.0)+z) );
}

//***************************************************************************************
// added: 28.01.2001
// arcus-sec-hyperbolicus-Funktion
// hyperbolic arc-sec function
// asech(z)=acosh(1/z)
//***************************************************************************************

COMPLEX asech (const COMPLEX& z)
{
	return acosh(COMPLEX(1.0,0.0) / z);
}

//***************************************************************************************
// added: 28.01.2001
// arcus-cosec-hyperbolicus-Funktion
// hyperbolic arc-cosec function
// acosech(z)=asinh(1/z)
//***************************************************************************************

COMPLEX acosech (const COMPLEX& z)
{
	return asinh(COMPLEX(1.0,0.0) / z);
}

} // namespace iRRAM
