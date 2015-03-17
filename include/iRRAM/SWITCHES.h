/*

iRRAM/SWITCHES.h -- header file for SWITCHES class of the iRRAM library
 
Copyright (C) 2001-2013 Norbert Mueller
 
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
// Definition of switches
//
// The iRRAM uses "switches" that start with a default behaviour.
// Their value can be changed by declaring a variable of the corresponding type.
// This change is effective until an other variable of the type is declared or the
// variable is destroyed.
// With the end of the lifetime of a variable, the previous behaviour will be restored.
//
// Such switches exist for:
// -  explicit declaration of single_valued behaviour of a code section despite multi-valued operations
// -  switching the precision policy to absolute or relative precision
// -  temporary increase or decrease of the working precision for REAL
// -  declaration of the working precision of DYADIC operators (see DYACIC.h)
//
// The switches are thread-specific.
//
/****************************************************************************/

#ifndef iRRAM_SWITCHES_H
#define iRRAM_SWITCHES_H

#include <iRRAM/core.h>

namespace iRRAM {

  extern int iRRAM_prec_array[];
  extern const int iRRAM_prec_steps;

// explicit declaration of single_valued behaviour of a code section despite multi-valued operations
//
// single_valued code  --> all possible computation paths again lead to single-valued function
//
// obsoletes continous_begin(), continous_end
class single_valued
{
int saved;
public:
  inline  single_valued(){ saved=ACTUAL_STACK.inlimit++; }
  inline ~single_valued(){ ACTUAL_STACK.inlimit=saved; }
};


// switching the precision policy to absolute or relative precision
//
// precision_mode code(0)  --> work with error control based on absolute precision
// precision_mode code(1)  --> work with error control based on relative precision
//
// obsoletes precision_policy()
class precision_mode
{
int saved;
public:
  inline precision_mode(int policy){
    saved=ACTUAL_STACK.prec_policy;
    ACTUAL_STACK.prec_policy=policy; }
  inline ~precision_mode(){ACTUAL_STACK.prec_policy=saved; }
};

// temporary increase of the working precision
//
// stiff code    --> temporarily work with next precision step 
// stiff code(n) --> temporarily work with precision n steps higher (n>0) or lower (n<0)
//
// obsoletes stiff_begin(), stiff_end()
class stiff
{
	int saved;

	static inline void set_prec_step(int n)
	{
		if (n<1) n=1;
		if (iRRAM_prec_steps <= n) n = iRRAM_prec_steps-1;
		ACTUAL_STACK.prec_step = n;
		ACTUAL_STACK.actual_prec = iRRAM_prec_array[ACTUAL_STACK.prec_step];
		iRRAM_highlevel = (ACTUAL_STACK.prec_step > 1);
	}
public:
	explicit inline stiff(int n = 1) {
		saved = ACTUAL_STACK.prec_step;
		set_prec_step(ACTUAL_STACK.prec_step + n);
	}
	inline ~stiff() {
		set_prec_step(saved);
	}
	stiff(const stiff &) = delete;
	stiff operator=(const stiff &) = delete;
};

} // namespace iRRAM

#endif
