/*

LAZY_BOOLEAN.cc -- implementation of lazy booleans for the iRRAM library
 
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

#include <cstdlib>
#include <cstring>

#include <iRRAM/core.h>

namespace iRRAM {

LAZY_BOOLEAN::operator bool() const {
  bool result;
  if ( ACTUAL_STACK.inlimit !=0 || ! iRRAM_thread_data_address->cache_b.get(result)){

    if ( value <= LAZY_BOOLEAN::BOTTOM ){
      iRRAM_DEBUG1(1,"lazy boolean values BOTTOM leading to iteration\n");
      REITERATE(0);
    }

    result=value;
    if ( ACTUAL_STACK.inlimit==0 ) iRRAM_thread_data_address->cache_b.put(result);
  }
  return result;
}

int check( const LAZY_BOOLEAN& lb) {
  int result;
  if ( ACTUAL_STACK.inlimit !=0 || ! iRRAM_thread_data_address->cache_i.get(result)) {
    result=lb.value;
    if ( ACTUAL_STACK.inlimit==0 ) iRRAM_thread_data_address->cache_i.put(result);
  }   
  return result;
}

int choose(const LAZY_BOOLEAN& x1,
           const LAZY_BOOLEAN& x2,
           const LAZY_BOOLEAN& x3,
           const LAZY_BOOLEAN& x4,
           const LAZY_BOOLEAN& x5,
           const LAZY_BOOLEAN& x6)
{
  int result=0;
  if ( (ACTUAL_STACK.inlimit==0) && iRRAM_thread_data_address->cache_i.get(result)) return result;

  int minvalue=false;
  if (x1.value == 1 )result=1; else
   if (x2.value == 1 )result=2; else
    if (x3.value == 1 )result=3; else
     if (x4.value == 1 )result=4; else
      if (x5.value == 1 )result=5; else
       if (x6.value == 1 )result=6; else
        if (x1.value == LAZY_BOOLEAN::BOTTOM )minvalue=LAZY_BOOLEAN::BOTTOM; else
         if (x2.value == LAZY_BOOLEAN::BOTTOM )minvalue=LAZY_BOOLEAN::BOTTOM; else
          if (x3.value == LAZY_BOOLEAN::BOTTOM )minvalue=LAZY_BOOLEAN::BOTTOM; else
           if (x4.value == LAZY_BOOLEAN::BOTTOM )minvalue=LAZY_BOOLEAN::BOTTOM; else
            if (x5.value == LAZY_BOOLEAN::BOTTOM )minvalue=LAZY_BOOLEAN::BOTTOM; else
             if (x6.value == LAZY_BOOLEAN::BOTTOM )minvalue=LAZY_BOOLEAN::BOTTOM;

  if ( minvalue == LAZY_BOOLEAN::BOTTOM ){
    iRRAM_DEBUG1(1,"lazy boolean value BOTTOM leading to iteration\n");
    REITERATE(0);
  }

  if ( ACTUAL_STACK.inlimit==0 ) iRRAM_thread_data_address->cache_i.put(result);
  return result;
}

LAZY_BOOLEAN operator && 
   (const LAZY_BOOLEAN& x, const LAZY_BOOLEAN& y)
   {
     if (x.value==1 && y.value==1) return 1;
     if (x.value==0 || y.value==0) return 0;
     return LAZY_BOOLEAN::BOTTOM;
   } 

LAZY_BOOLEAN operator || 
   (const LAZY_BOOLEAN& x, const LAZY_BOOLEAN& y)
   {
     if (x.value==1 || y.value==1) return 1;
     if (x.value==0 && y.value==0) return 0;
     return LAZY_BOOLEAN::BOTTOM;
   } 

LAZY_BOOLEAN operator !
   (const LAZY_BOOLEAN& x)
   {
     if (x.value==1) return 0;
     if (x.value==0) return 1;
     return LAZY_BOOLEAN::BOTTOM;
   } 

} // namespace iRRAM
