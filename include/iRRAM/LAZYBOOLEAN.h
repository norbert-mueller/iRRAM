/*

iRRAM_LAZYBOOLEAN.h -- header file for LAZYBOOLEAN class of the iRRAM library
 
Copyright (C) 2001-2012 Norbert Mueller
 
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
/****************************************************************************/
// Definition of class LAZY_BOOLEAN 
/****************************************************************************/

#ifndef iRRAM_LAZYBOOLEAN_H
#define iRRAM_LAZYBOOLEAN_H

namespace iRRAM {

class LAZY_BOOLEAN
{


public:  
LAZY_BOOLEAN(){value=false;};
LAZY_BOOLEAN(const LAZY_BOOLEAN& y){value=y.value;};
LAZY_BOOLEAN& operator = (const LAZY_BOOLEAN& y){value=y.value; return *this;};
LAZY_BOOLEAN(bool b){value=b;};


friend LAZY_BOOLEAN operator && (const LAZY_BOOLEAN& x, const LAZY_BOOLEAN& y);
friend LAZY_BOOLEAN operator || (const LAZY_BOOLEAN& x, const LAZY_BOOLEAN& y);
friend LAZY_BOOLEAN operator ! ( const LAZY_BOOLEAN& x);
friend int choose(const LAZY_BOOLEAN& x1,
                  const LAZY_BOOLEAN& x2,
                  const LAZY_BOOLEAN& x3,
                  const LAZY_BOOLEAN& x4,
                  const LAZY_BOOLEAN& x5,
                  const LAZY_BOOLEAN& x6 );

operator bool() const;
friend int check (const LAZY_BOOLEAN& x);

friend REAL maximum ( const REAL& x, const REAL& y);
friend REAL minimum ( const REAL& x, const REAL& y);

friend LAZY_BOOLEAN operator < (const REAL&, const REAL&);
friend LAZY_BOOLEAN positive (const REAL& x, int k);
friend LAZY_BOOLEAN bound (const REAL& x, const int k);

friend class REAL;

private:

LAZY_BOOLEAN(int b){value=b;};
int value;
const static int BOTTOM = -1; 
};

int choose(const LAZY_BOOLEAN& x1= false,
           const LAZY_BOOLEAN& x2= false,
           const LAZY_BOOLEAN& x3= false,
           const LAZY_BOOLEAN& x4= false,
           const LAZY_BOOLEAN& x5= false,
           const LAZY_BOOLEAN& x6= false );



} // namespace iRRAM

#endif
