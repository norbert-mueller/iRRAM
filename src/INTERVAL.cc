/*

INTERVAL.cc -- interface implementation of real intervals for the iRRAM library
 
Copyright (C) 2003 Norbert Mueller, Shao Qi
 
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

INTERVAL::INTERVAL()
{
  low=0;
  upp=0;
}

INTERVAL::INTERVAL(const REAL& x)
{
  low=x;
  upp=x;
}

INTERVAL::INTERVAL(const REAL& x, const REAL& y)
{
  low=minimum(x,y);
  upp=maximum(x,y);
}

INTERVAL operator + (const INTERVAL & x, const INTERVAL & y){
	return INTERVAL( x.low+y.low, x.upp+y.upp, true);
}

INTERVAL operator - (const INTERVAL  & x, const INTERVAL  & y){
	return INTERVAL( x.low-y.upp, x.upp-y.low, true);
}

INTERVAL operator - (const INTERVAL  & x){
	return INTERVAL( -x.upp, -x.low, true);
}

INTERVAL operator * (const INTERVAL  & x, const INTERVAL  & y){
	REAL aa= x.low*y.low;
	REAL ab= x.upp*y.low;
	REAL ba= x.low*y.upp;
	REAL bb= x.upp*y.upp ;
	REAL mina=minimum(aa,ab);
	REAL minb=minimum(ba,bb);
	REAL maxa=maximum(aa,ab);
	REAL maxb=maximum(bb,ba);

	return INTERVAL(minimum(mina,minb),maximum(maxa,maxb), true);

}

INTERVAL operator / (const INTERVAL  & x, const INTERVAL  & y){
  if ( y.low< REAL(0) && REAL(0) < y.upp ) {
    throw iRRAM_Numerical_Exception(iRRAM_interval_divide_by_zero);
  }
  return  INTERVAL(1/y.low,1/y.upp)*x;
}


REAL wid(const INTERVAL& x){
return  x.upp-x.low ;
}

REAL inf(const INTERVAL& x){
return x.low;
}

REAL sup(const INTERVAL& x){
return x.upp;
}

REAL mid(const INTERVAL& x){
return (x.upp+x.low)/2;
}

REAL mag(const INTERVAL& x){
return maximum(abs(x.upp),abs(x.low));
}

REAL mig(const INTERVAL& x){
return maximum(REAL(0),x.low)-minimum(REAL(0),x.upp);
}

INTERVAL fabs(const INTERVAL& x){
return INTERVAL(maximum(x.low,REAL(0))-minimum(x.upp,REAL(0)),
                maximum(abs(x.low),abs(x.upp)), true);
}

INTERVAL square(const INTERVAL& x){
return INTERVAL(x.low*maximum(x.low,REAL(0)) + x.upp*minimum(x.upp,REAL(0)),
                square(maximum(abs(x.low),abs(x.upp))), true);
}

INTERVAL power(const INTERVAL& x,int n){
  if (n==0)  return 1;
  if (n==1)  return x;
  if (n==2)  return square(x);
  if (n<0)   return power(1/x,-n);
  if (n%2==1)return INTERVAL(power(x.low,n),power(x.upp,n), true);
  REAL l=power(x.low,n-1);
  REAL u=power(x.upp,n-1);
  return INTERVAL(l*maximum(x.low,REAL(0)) + u*minimum(x.upp,REAL(0)),
                maximum(abs(l*x.low),abs(u*x.upp)), true);
}

INTERVAL exp(const INTERVAL& x){
// exp is monotonic increasing, so the following is sufficient:
return INTERVAL(exp(x.low),exp(x.upp), true);
}

INTERVAL log(const INTERVAL& x){
// log is monotonic increasing, so the following is sufficient:
return INTERVAL(log(x.low),log(x.upp), true);
}

INTERVAL sin(const INTERVAL& x){
// first, using lazy booleans, we do a multivalued test
// whether the interval width is definitely at least 2*pi.
 
  int test=choose( x.upp-x.low >= 2*pi(), 
                   x.upp-x.low <  3*pi());

// if the width is at least 2*pi, return the closed interval [-1,1]
	       
  if ( test == 1 ) return INTERVAL(REAL(-1), REAL(1));

// now we have a width of at most 3*pi, 
// and we do a range reduction
  
  REAL inf_reduced=modulo(x.low,2*pi());
  REAL sup_reduced=x.upp- (x.low-inf_reduced);
  
// now we have -2*pi <= inf_reduced<= sup_reduced <= 5*pi
// with a difference between the values of at most 3*pi

// we could estimate which odd multiples of pi/2 lie between the
// borders of the reduced interval, but for simplicity here I try all
// 7 possibilities (it was easier using copy and paste...)

// the following values are either 
//   inf_reduced, or 
//   sup_reduced, or
//   the multiple of pi/2, in case it lies between the two values
REAL p1= maximum(inf_reduced,minimum(sup_reduced,-3*pi()/2)); 
REAL p2= maximum(inf_reduced,minimum(sup_reduced,  -pi()/2)); 
REAL p3= maximum(inf_reduced,minimum(sup_reduced,   pi()/2)); 
REAL p4= maximum(inf_reduced,minimum(sup_reduced, 3*pi()/2)); 
REAL p5= maximum(inf_reduced,minimum(sup_reduced, 5*pi()/2)); 
REAL p6= maximum(inf_reduced,minimum(sup_reduced, 7*pi()/2)); 
REAL p7= maximum(inf_reduced,minimum(sup_reduced, 9*pi()/2)); 
REAL s1=sin(p1);
REAL s2=sin(p2);
REAL s3=sin(p3);
REAL s4=sin(p4);
REAL s5=sin(p5);
REAL s6=sin(p6);
REAL s7=sin(p7);

REAL inf_min=minimum(sin(inf_reduced),sin(sup_reduced));
inf_min=minimum(inf_min,s1);
inf_min=minimum(inf_min,s2);
inf_min=minimum(inf_min,s3);
inf_min=minimum(inf_min,s4);
inf_min=minimum(inf_min,s5);
inf_min=minimum(inf_min,s6);
inf_min=minimum(inf_min,s7);

REAL sup_min=maximum(sin(inf_reduced),sin(sup_reduced));
sup_min=maximum(sup_min,s1);
sup_min=maximum(sup_min,s2);
sup_min=maximum(sup_min,s3);
sup_min=maximum(sup_min,s4);
sup_min=maximum(sup_min,s5);
sup_min=maximum(sup_min,s6);
sup_min=maximum(sup_min,s7);
return INTERVAL(inf_min,sup_min);
}

INTERVAL cos(const INTERVAL& x){

// using lazy booleans, we do a multivalued test whether
// the interval width is definitely at least 2*pi.
 
  int test=choose( x.upp-x.low >= 2*pi(), 
               x.upp-x.low <  3*pi());

// if the width is at least 2*pi, return the closed interval [-1,1]
	       
  if ( test == 1 ) return INTERVAL(REAL(-1), REAL(1));

// now we have a width of at most 3*pi
// we do a range reduction
  
  REAL inf_reduced=modulo(x.low,2*pi());
  REAL sup_reduced=x.upp- (x.low-inf_reduced);
  
// now we have -2*pi <= inf_reduced<= sup_reduced <= 5*pi
// with a difference between the values of at most 3*pi

// now we could estimate which multiples of pi lie between the
// borders of the reduced interval, but for simplicity here I try all
// 8 possibilities (it was easier using copy and paste...)

// the following values are either 
//   inf_reduced, or 
//   sup_reduced, or
//   a multiple of pi, in case it lies between the two values
REAL p1= maximum(inf_reduced,minimum(sup_reduced,-2*pi())); 
REAL p2= maximum(inf_reduced,minimum(sup_reduced,  -pi())); 
REAL p3= maximum(inf_reduced,minimum(sup_reduced,REAL(0.0))); 
REAL p4= maximum(inf_reduced,minimum(sup_reduced,   pi())); 
REAL p5= maximum(inf_reduced,minimum(sup_reduced, 2*pi())); 
REAL p6= maximum(inf_reduced,minimum(sup_reduced, 3*pi())); 
REAL p7= maximum(inf_reduced,minimum(sup_reduced, 4*pi())); 
REAL s1=cos(p1);
REAL s2=cos(p2);
REAL s3=cos(p3);
REAL s4=cos(p4);
REAL s5=cos(p5);
REAL s6=cos(p6);
REAL s7=cos(p7);

REAL inf_min=minimum(cos(inf_reduced),cos(sup_reduced));
inf_min=minimum(inf_min,s1);
inf_min=minimum(inf_min,s2);
inf_min=minimum(inf_min,s3);
inf_min=minimum(inf_min,s4);
inf_min=minimum(inf_min,s5);
inf_min=minimum(inf_min,s6);
inf_min=minimum(inf_min,s7);

REAL sup_min=maximum(cos(inf_reduced),cos(sup_reduced));
sup_min=maximum(sup_min,s1);
sup_min=maximum(sup_min,s2);
sup_min=maximum(sup_min,s3);
sup_min=maximum(sup_min,s4);
sup_min=maximum(sup_min,s5);
sup_min=maximum(sup_min,s6);
sup_min=maximum(sup_min,s7);
return INTERVAL(inf_min,sup_min);
}

INTERVAL tan(const INTERVAL& x){
// If there is no odd multiple of pi/2 between the borders of
// the interval x, then the monotonicity of tan() is sufficient
// for the following formula to give the correct result:
return INTERVAL(tan(x.low),tan(x.upp));
// If on the other hand, there is such a multiple, then
// tan(x) is no proper interval.
// We choose to stay with the upper formula in this case.
// Another possibility would be to throw an exception.
}

INTERVAL asin(const INTERVAL& x){
return INTERVAL (asin(maximum(x.low,REAL(-1))),
                 asin(minimum(x.upp,REAL(1))));
}

INTERVAL acos(const INTERVAL& x){
return INTERVAL (acos(minimum(x.upp,REAL(1))),
                 acos(maximum(x.low,REAL(-1))));
}

INTERVAL atan(const INTERVAL& x){
return INTERVAL (atan(x.low),atan(x.upp));
}


LAZY_BOOLEAN superset (const INTERVAL& x,
                             const INTERVAL& y){
return ( (x.low <= y.low)&&(x.upp >= y.upp) );
}


LAZY_BOOLEAN proper_superset (const INTERVAL& x, const INTERVAL& y){
return superset(x,y); }

LAZY_BOOLEAN subset (const INTERVAL& x, const INTERVAL& y){
return superset(y,x); }

LAZY_BOOLEAN proper_subset (const INTERVAL& x, const INTERVAL& y){
return superset(y,x); }

LAZY_BOOLEAN in_interior (const INTERVAL& x, const INTERVAL& y){
return superset(y,x); }

LAZY_BOOLEAN disjoint (const INTERVAL& x, const INTERVAL& y){
return ( (x.low > y.upp) || (y.low > x.upp));}

LAZY_BOOLEAN in (const REAL& x, const INTERVAL& y){
return ( (y.low <= x) && (x <= y.upp) );}

INTERVAL interval_hull (const INTERVAL& x, const INTERVAL& y){
return INTERVAL(minimum(x.low,y.low),maximum(x.upp,x.upp));}

INTERVAL intersect (const INTERVAL& x, const INTERVAL& y){
return INTERVAL(maximum(x.low,y.low),minimum(x.upp,x.upp));}

} // namespace iRRAM
