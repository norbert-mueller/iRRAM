/*

MPFR_ext.h -- extension to the MPFR float functions for the iRRAM library
 
Copyright (C) 2002/2003 Norbert Mueller
 
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

#ifndef MPFR_ext_h
#define MPFR_ext_h

#include <stdlib.h>
#include <stdio.h>

#include "iRRAM/MPFR_interface.h"

//const unsigned int BITS_PER_MP_LIMB  = 8*sizeof (mp_limb_t);

//#include "gmp-mparam.h"
//#include "mpfr-impl.h"
#define MPFR_PREC(x)      ((x)->_mpfr_prec)
#define MPFR_EXP(x)       ((x)->_mpfr_exp)
#define MPFR_MANT(x)      ((x)->_mpfr_d)
#define MPFR_LAST_LIMB(x) ((MPFR_PREC (x) - 1) / GMP_NUMB_BITS)
#define MPFR_LIMB_SIZE(x) (MPFR_LAST_LIMB (x) + 1)
#define MPFR_NOTZERO(x)  !( mpfr_sgn(x)==0)


#include "mpfr.h"

#define MAX_OF(h,i) ((h) >= (i) ? (h) : (i))


#define GMP_min -1000000000
#define GMP_max 1000000000

#define iRRAM_mpfr_rounding_mode GMP_RNDN

int ext_mpfr_size(ext_mpfr_type z);

void ext_mpfr_add(ext_mpfr_type z1,ext_mpfr_type z2,ext_mpfr_type z,int p);
void ext_mpfr_sub(ext_mpfr_type z1,ext_mpfr_type z2,ext_mpfr_type z,int p);
void ext_mpfr_mul(ext_mpfr_type z1,ext_mpfr_type z2,ext_mpfr_type z,int p);
void ext_mpfr_div(ext_mpfr_type z1,ext_mpfr_type z2,ext_mpfr_type z,int p);
void ext_mpfr_abs(ext_mpfr_type z1,ext_mpfr_type z);
void ext_mpfr_truncate(ext_mpfr_type z1,ext_mpfr_type z);

void ext_mpfr_set_z(ext_mpfr_type r, int_mpfr_type i);
void ext_mpfr_set_f2i(ext_mpfr_type r, int_mpfr_type i);

void ext_mpfr_add_i(ext_mpfr_type z1, int z2, ext_mpfr_type z, int p);
void ext_mpfr_sub_i(ext_mpfr_type z1, int z2, ext_mpfr_type z, int p);
void ext_mpfr_i_sub(int z1, ext_mpfr_type z2, ext_mpfr_type z, int p);
void ext_mpfr_mul_i(ext_mpfr_type z1, int z2, ext_mpfr_type z, int p);
void ext_mpfr_div_i(ext_mpfr_type z1, int z2, ext_mpfr_type z, int p);
void ext_mpfr_i_div(int z1, ext_mpfr_type z2, ext_mpfr_type z, int p);

void ext_mpfr_writee(ext_mpfr_type z,int w);
char* ext_mpfr_swritee(ext_mpfr_type z,int w);
void ext_mpfr_write(ext_mpfr_type z,int w);

void ext_mpfr_duplicate_w_init(ext_mpfr_type z1,ext_mpfr_type *z2);
void ext_mpfr_duplicate_wo_init(ext_mpfr_type z1,ext_mpfr_type z2);

void ext_mpfr_sqrt(ext_mpfr_type z1,ext_mpfr_type z,int p);
void ext_mpfr_shift(ext_mpfr_type z1,ext_mpfr_type z,int p);

inline void ext_mpfr_remove_trailing_zeroes (mpfr_t x)
{
   unsigned int xn = MPFR_MSW_INDEX(x);
   unsigned int i;
   unsigned int j=(xn+1)*BITS_PER_MP_LIMB;
   mp_limb_t *xp = MPFR_MANT(x);
 
   if ( xp[0] != 0 ) return;
   /* first determine last non-zero limb */
   for (i=0; i<=xn && xp[i]==0; (i++) ) j-=BITS_PER_MP_LIMB;
 
   if (i > xn) return;
   mpfr_round_prec (x, GMP_RNDN, j); 
}

inline void ext_mpfr_initialize()
{
mpfr_set_default_prec(32);
}


inline int ext_mpfr_size(ext_mpfr_type z)
{
if ( MPFR_NOTZERO(z) ) return MPFR_EXP(z);
else return GMP_min;
}

inline void ext_mpfr_getsize(ext_mpfr_type z,ext_mpfr_sizetype* s)
{
  int zn=MPFR_MSW_INDEX(z);
  if ( MPFR_NOTZERO(z) ) {
#if BITS_PER_MP_LIMB == 32
    s->mantissa=( ((z->_mpfr_d)[zn]) >> 1 ) + 1;
    s->exponent=MPFR_EXP(z)-BITS_PER_MP_LIMB+1;
#else  
    s->mantissa=( ((z->_mpfr_d)[zn]) >> (BITS_PER_MP_LIMB/2+1) ) + 1;
    s->exponent=MPFR_EXP(z)- (BITS_PER_MP_LIMB/2) +1;
#endif
  } else {
    s->mantissa=0;
    s->exponent=GMP_min;
  }
}

inline void ext_mpfr_add(ext_mpfr_type z1,ext_mpfr_type z2,ext_mpfr_type z,int p)
{ int q,s1,s2;
  s1=ext_mpfr_size(z1);
  s2=ext_mpfr_size(z2);
  q=MAX_OF(s1,s2)-p+1;
  q=MAX_OF(q,10);
  mpfr_set_prec(z,q);
  mpfr_add(z,z1,z2,iRRAM_mpfr_rounding_mode);
  ext_mpfr_remove_trailing_zeroes (z); 
  return;
}

inline void ext_mpfr_add_i(ext_mpfr_type z1,int n,ext_mpfr_type z,int p)
{ 
	int q,s1,s2=BITS_PER_MP_LIMB ;
  	s1=ext_mpfr_size(z1);
	q=MAX_OF(s1,s2)-p+1;
	q=MAX_OF(q,10);
	mpfr_set_prec(z,q);
	if(n<0)mpfr_sub_ui(z,z1,-n,iRRAM_mpfr_rounding_mode);
		else mpfr_add_ui(z,z1,n,iRRAM_mpfr_rounding_mode);
	return;
}


inline void ext_mpfr_sub(ext_mpfr_type z1,ext_mpfr_type z2,ext_mpfr_type z,int p)
{ int q,s1,s2;
  s1=ext_mpfr_size(z1);
  s2=ext_mpfr_size(z2);
  q=MAX_OF(s1,s2)-p+1;
  q=MAX_OF(q,10);
  mpfr_set_prec(z,q);
  mpfr_sub(z,z1,z2,iRRAM_mpfr_rounding_mode);
  ext_mpfr_remove_trailing_zeroes (z); 
 return;
}

inline void ext_mpfr_sub_i(ext_mpfr_type z1,int n,ext_mpfr_type z,int p)
{ 
	int q,s1 ;
  	s1=ext_mpfr_size(z1);
	q=MAX_OF(s1,BITS_PER_MP_LIMB)-p+1;
	q=MAX_OF(q,10);
	mpfr_set_prec(z,q);
	if(n<0)mpfr_add_ui(z,z1,-n,iRRAM_mpfr_rounding_mode);
		else mpfr_sub_ui(z,z1,n,iRRAM_mpfr_rounding_mode);
	return;
}

inline void ext_mpfr_i_sub(int z1,ext_mpfr_type z2,ext_mpfr_type z,int p)
{ 
	int q,s2;
  	s2=ext_mpfr_size(z2);
	q=MAX_OF(BITS_PER_MP_LIMB,s2)-p+1;
  	q=MAX_OF(q,10);
  	mpfr_set_prec(z,q);
  	if(z1<0){mpfr_add_ui(z,z2,-z1,iRRAM_mpfr_rounding_mode);mpfr_neg(z,z,iRRAM_mpfr_rounding_mode);} 
	else mpfr_ui_sub(z,z1,z2,iRRAM_mpfr_rounding_mode);
  	return;
}

inline void ext_mpfr_mul(ext_mpfr_type z1,ext_mpfr_type z2,ext_mpfr_type z,int p)
{ int q,s1,s2;
  s1=ext_mpfr_size(z1);
  s2=ext_mpfr_size(z2);
  q=s1+s2-p+1;
  q=MAX_OF(q,10);
  mpfr_set_prec(z,q);
  mpfr_mul(z,z1,z2,iRRAM_mpfr_rounding_mode);
  ext_mpfr_remove_trailing_zeroes (z); 
  return;
}

inline void ext_mpfr_mul_i(ext_mpfr_type z1, int z2, ext_mpfr_type z, int p)
{
int q,s1,zz;
s1=ext_mpfr_size(z1);
q=s1+BITS_PER_MP_LIMB-p+10;if(q<10)q=10;
int maxsize_ifexact=MPFR_PREC(z1)+32;
if (q>maxsize_ifexact)q= maxsize_ifexact;
mpfr_set_prec(z,q);
if (z2<0) zz=-z2; else zz=z2;
mpfr_mul_ui(z,z1,zz,iRRAM_mpfr_rounding_mode);
if (z2<0)mpfr_neg(z,z,iRRAM_mpfr_rounding_mode);
return;
}

inline void ext_mpfr_div(ext_mpfr_type z1,ext_mpfr_type z2,ext_mpfr_type z,int p)
{ int q,s1,s2;
  s1=ext_mpfr_size(z1);
  s2=ext_mpfr_size(z2);
  q=s1-s2-p+1;
  q=MAX_OF(q,10);
  mpfr_set_prec(z,q);
  mpfr_div(z,z1,z2,iRRAM_mpfr_rounding_mode);
  ext_mpfr_remove_trailing_zeroes (z); 
  return;
}

inline void ext_mpfr_div_i(ext_mpfr_type z1,int z2,ext_mpfr_type z,int p)
{ 
	int q,s1,zz,s2=1;
  	s1=ext_mpfr_size(z1);
  	q=s1-s2-p+10;
  	q=MAX_OF(q,10);
  	mpfr_set_prec(z,q);
	if(z2<0)zz=-z2;else zz=z2;
  	mpfr_div_ui(z,z1,zz,iRRAM_mpfr_rounding_mode);
	if(z2<0)mpfr_neg(z,z,iRRAM_mpfr_rounding_mode);
  	return;
}

inline void ext_mpfr_i_div(int z1,ext_mpfr_type z2,ext_mpfr_type z,int p)
{ 
	int q,zz,s2;
  	s2=ext_mpfr_size(z2);
  	q=BITS_PER_MP_LIMB-s2-p+10;
  	q=MAX_OF(q,10);
  	mpfr_set_prec(z,q);
	if(z1<0)zz=-z1;else zz=z1;
  	mpfr_ui_div(z,zz,z2,iRRAM_mpfr_rounding_mode);
	if(z1<0)mpfr_neg(z,z,iRRAM_mpfr_rounding_mode);
  	return;
}

inline void ext_mpfr_abs(ext_mpfr_type z1,ext_mpfr_type z)
{
  int q1=mpfr_get_prec(z1);
  mpfr_set_prec(z,q1);
  mpfr_abs(z,z1,iRRAM_mpfr_rounding_mode);
  return;
}

inline void ext_mpfr_truncate(ext_mpfr_type z1,ext_mpfr_type z)
{
  int q1=mpfr_get_prec(z1);
  mpfr_set_prec(z,q1);
  mpfr_trunc(z,z1);
  return;
}


inline void ext_mpfr_writee(ext_mpfr_type z,int w)
{ char *s=(char *)malloc(w+1);

  long e; 
  int i;
  int flag=0;
  mpfr_get_str (s, &e, 10, w-7,z,iRRAM_mpfr_rounding_mode);
  for (i=0;i<w-7;i++) {
    if ( s[i]==0 ) flag=1 ;
    if ( flag ) s[i]='0';
  } 
  if (s[0]=='-') {s[w-7]=0;printf("-.%sE%+05ld",&s[1],e);}
  else {s[w-8]=0;printf("+.%sE%+05ld",&s[0],e);};
  free(s);
}

inline char* ext_mpfr_swritee(ext_mpfr_type z,int w)
{ 
  char* s=(char*)(malloc(w+1));
  char* r=&s[2];
  long e, e_abs,r_len; 
  int i;
  int flag=0;
  int sgn =  mpfr_sgn (z);

  mpfr_t z_copy;
  mpfr_init2(z_copy,mpfr_get_prec(z));
  mpfr_abs(z_copy,z,iRRAM_mpfr_rounding_mode);
  mpfr_get_str (r, &e, 10, w-6,z_copy,iRRAM_mpfr_rounding_mode);
  mpfr_clear(z_copy);

  /* in case trailing zeroes were discarded, we add them again... */
  for (i=0;i<w-6;i++) {
    if ( r[i]==0 ) flag=1 ;
    if ( flag ) r[i]='0';
  } 

  /*
  we want to have an error of less than 0.51ulp, so above 
  we took two additional digits giving an error of at most 0.01ulp
  (concerning the intended precision).
  The position of these digits is (w-8,w-7) for the 
  usual 4-decimal exponents and decreases for the larger cases.
  If the first of the two additional digits is 5 or greater,
  we round upwards (giving at most additional 0.5ulp)
  */ 

  r_len=e; if (r_len<0) r_len=-r_len;
  if (r_len<10000){r_len=4;}
  else if (r_len<100000){r_len=5;}
  else if (r_len<1000000){r_len=6;}
  else if (r_len<10000000){r_len=7;}
  else if (r_len<100000000){r_len=8;}
  else if (r_len<1000000000){r_len=9;}
  else {r_len=10;};

  /*
  r_len is the number of decimal places of the exponent prior to the rounding
  */


  if ( w < r_len+5) w=r_len+5;
  /*
  Produce at least one significant digit...
  */

  if ( r[w-r_len-4]>='5' ){
    for (i=w-r_len-5;i>=0;i--){
      if ( r[i] < '9' ) {
          r[i]++;
          break;
      }
      r[i]='0';
    }
    /* 
    In the rare case that we switch from 0.999...999 (length n) to 1.000...000 (length n+1), 
    we have to correct the exponent and the mantissa, but changing the mantissa is simple:
    (1) If the (decimal) length of the exponent does not change, result is 0.100...000 (length n) 
    (2) If the (decimal) length of the exponent increases, result is 0.100...000 (length n-1) 
    (3) If the (decimal) length of the exponent decreases, result is 0.100...000 (length n+1)
    So we simply set the last decimal to '0'. The 0.51ulp accuracy is true in all three cases!
    */ 
    if (i<0){
      r[0]='1';
      r[w-r_len-4]='0';
      e++;
    }
  }
  r[w-r_len-3] = 0;
  if (sgn== -1) s[0]='-';
  else s[0]='+';
  s[1]='.';

  e_abs=e; if (e_abs<0) e_abs=-e_abs;
  if (e_abs<10000){e_abs=4;}
  else if (e_abs<100000){e_abs=5;}
  else if (e_abs<1000000){e_abs=6;}
  else if (e_abs<10000000){e_abs=7;}
  else if (e_abs<100000000){e_abs=8;}
  else if (e_abs<1000000000){e_abs=9;}
  else {e_abs=10;};
  if (e_abs > w-2){sprintf(&s[w-6],"E*****");
  } else {
    if (e_abs==4){sprintf(&s[w-6],"E%+05ld",e);}
    else if (e_abs==5){sprintf(&s[w-7],"E%+06ld",e);}
    else if (e_abs==6){sprintf(&s[w-8],"E%+07ld",e);}
    else if (e_abs==7){sprintf(&s[w-9],"E%+08ld",e);}
    else if (e_abs==8){sprintf(&s[w-10],"E%+09ld",e);}
    else if (e_abs==9){sprintf(&s[w-11],"E%+010ld",e);}
    else {sprintf(&s[w-12],"E%+011ld",e);};
  }
  return s;
}


inline void ext_mpfr_write(ext_mpfr_type z,int w)
{ char *s=(char *)malloc(w+1);
  long e; 
  int i;
  int flag=0;
  mpfr_get_str (s, &e, 10, w-7,z,iRRAM_mpfr_rounding_mode);
  for (i=0;i<w-7;i++) { if ( s[i]==0 ) flag=1 ;
    if ( flag ) s[i]='0';
  } 
  if (s[0]=='-') {s[w-7]=0;printf("-.%sE%+05ld",&s[1],e);}
  else {s[w-8]=0;printf("+.%sE%+05ld",&s[0],e);};
  if (MPFR_LIMB_SIZE(z) != 0) {
     printf("\n");
     for (i=MPFR_LIMB_SIZE(z)-1;i>=0; i-=1) 
       printf("%lu ",z->_mpfr_d[i]);
  }
  printf("\nExponent %ld, Limbs: %ld, Bits: %ld\n",
     z->_mpfr_exp,MPFR_LIMB_SIZE(z),mpfr_get_prec(z));
  free(s);
}

inline void ext_mpfr_duplicate_wo_init(ext_mpfr_type z1,ext_mpfr_type z2)
{
  int q1=mpfr_get_prec(z1);
  if (mpfr_get_prec(z2)< q1) mpfr_set_prec(z2,q1);
  mpfr_set(z2,z1,iRRAM_mpfr_rounding_mode);
}

inline void ext_mpfr_duplicate_w_init(ext_mpfr_type z1,ext_mpfr_type *z2)
{
//  *z2 = (ext_mpfr_type) malloc(sizeof (__mpfr_struct));
//  mpfr_init2(*z2,mpfr_get_prec(z1));
  int q1=mpfr_get_prec(z1);
  *z2=ext_mpfr_init();
  if (mpfr_get_prec(*z2)< q1) mpfr_set_prec(*z2,q1);
//  ext_mpfr_var_count +=1;
  mpfr_set(*z2,z1,iRRAM_mpfr_rounding_mode);
}

inline void ext_mpfr_sqrt(ext_mpfr_type z1,ext_mpfr_type z,int p)
{ int q,s1;
  s1=ext_mpfr_size(z1);
  q=s1/2-p+1;
  q=MAX_OF(q,10);
  mpfr_set_prec(z,q);
  mpfr_sqrt(z,z1,iRRAM_mpfr_rounding_mode);
  ext_mpfr_remove_trailing_zeroes (z); 
  return;
}

inline void ext_mpfr_shift(ext_mpfr_type z1,ext_mpfr_type z,int n)
{
  mpfr_set_prec(z,mpfr_get_prec(z1));
  if ( n>=  0 ) mpfr_mul_2exp(z,z1,n,iRRAM_mpfr_rounding_mode);
  else mpfr_div_2exp(z,z1,-n,iRRAM_mpfr_rounding_mode);
  return;
}

inline void ext_mpfr_set_z(ext_mpfr_type r,int_mpfr_type i)
{
  int sib=mpz_sizeinbase(i,2);
  mpfr_set_prec(r,MAX_OF(32,sib));
  mpfr_set_z(r,i,iRRAM_mpfr_rounding_mode);
} 

#endif
