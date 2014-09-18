/*

GMP_int_ext.c -- extension to the GMP integer functions for the iRRAM library 
 
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

#include "iRRAM/GMP_intrat.h"

#include <strings.h>

#ifndef DEBUG			// debugging information
#define DEBUG 0
#endif
#if DEBUG > 0
  int DEBUG_OP = 1;
  unsigned int op_count=0;
  int minshowop = DEBUG ;
#endif

//**************************************************************************/
//* Variables for counting free space */
//**************************************************************************/

__thread int_gmp_type gmp_FreeVarsi[MaxFreeVars];
__thread int gmp_FreeVarCounti=0L;
__thread int int_gmp_var_count=0;


//**************************************************************************/
// nth root of integer in GMP
// added: 07.02.2001
// Last change: 07.02.2001
// Comment: this root-function truncates the result to the integer part
// Arguments: 1. GMP_integer
// 		  2. unsigned int
// 		  3. GMP_integer
//**************************************************************************/

void int_gmp_root(int_gmp_type z1, unsigned int z2, int_gmp_type z)
{
  mpz_root(z, z1, z2);
  return;
}

//**************************************************************************/
// power function in GMP
// calculates z = z1 ^ z2
// added: 07.02.2001
// Last change: 07.02.2001
// Comment: base=MP exponent=int
// Arguments: 1. GMP_integer
// 		  2. unsigned integer
// 		  3. GMP_integer
//**************************************************************************/

void int_gmp_power_i(int_gmp_type z1, unsigned int z2, int_gmp_type z)
{
 mpz_pow_ui(z, z1, z2);
 return;
}

//**************************************************************************/
// power function in GMP
// calculates z = z1 ^ z2
// added: 07.02.2001
// Last change: 07.02.2001
// Comment: base=int exponent=int
// Arguments: 1. unsigned int
// 		  2. unsigned int
// 		  3. GMP_integer
//**************************************************************************/

void int_gmp_power_ii(unsigned int z1, unsigned int z2, int_gmp_type z)
{
 mpz_ui_pow_ui(z, z1, z2);
 return;
}

//**************************************************************************/
// factorial function in GMP
// calculates z = z1!
// added: 07.02.2001
// Last change: 07.02.2001
// Comment: base is int
// Arguments: 1. unsigned int
// 		  2. GMP_integer
//**************************************************************************/

void int_gmp_fac(unsigned int z1, int_gmp_type z)
{
 mpz_fac_ui(z, z1);
 return;
}

//**************************************************************************/
// modulo function in GMP
// calculates z = z1 % z2
// added: 07.02.2001
// Last change: 07.02.2001
// Arguments: 1. GMP_integer
// 		  2. GMP_integer
// 		  3. GMP_integer
//**************************************************************************/

void int_gmp_modulo(int_gmp_type z1, int_gmp_type z2, int_gmp_type z)
{
 mpz_mod(z, z1, z2);
 return;
}





//**************************************************************************/
// writes integer to a string
// writes z to string with length w
// Arguments: 1. GMP_integer
// 		  2. int integer
//	return: char*
//**************************************************************************/


char* int_gmp_swritee(int_gmp_type z, int w)
{
  if (w<1)w=1;
  char *n, *s=malloc(w+1);
  int l;
  n=mpz_get_str(NULL, 10, z);
  l=strlen(n);
  if (l>w){
    strncpy(s,n,w);
    s[w-1]='*';
    s[w]='\0';
  }else{
    memset(s,' ',w-l);
    strncpy(&s[w-l],n,l);
    s[w]='\0';
  } 
  free(n); 
  return s;
}



//**************************************************************************/
// writes z to string with length calculated
// added: 18.07.2001
// Last change: 18.07.2001
// Arguments: 1. GMP_integer
// 	return: char*
//**************************************************************************/

char* int_gmp_sprintf(int_gmp_type z)
{
  return mpz_get_str(NULL,10,z);
}


//**************************************************************************/
// square root of GMP_integer
// calculates z = squareroot(z1)
// added: 01.02.2001
// Last change: 01.02.2001
// Arguments: 1. GMP_integer
// 		  2. GMP_integer
//**************************************************************************/

void int_gmp_sqrt(int_gmp_type z1, int_gmp_type z)
{
 mpz_sqrt(z,z1);
 return;
}


//**************************************************************************/
// shifting GMP_integer by n bits
// calculates z = z1 <<(>>) z2
// added: 01.02.2001
// Last change: 01.02.2001
// Arguments: 1. GMP_integer
// 		  2. GMP_integer
// 		  3. int integer n
//**************************************************************************/

void int_gmp_shift(int_gmp_type z1, int_gmp_type z, int n)
{
  if (n>=0) mpz_mul_2exp(z, z1, n);
  else mpz_tdiv_q_2exp(z, z1, -n);  /** truncate-div **/
}
