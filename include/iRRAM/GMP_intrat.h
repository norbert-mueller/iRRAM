/*

GMP_intrat.h -- extension to the GMP integer/rationals for the iRRAM library
 
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

/*
Changelog: (initial version by Tom van Diessen)  

  2001-09-10 removal of several unneeded functions by Norbert
  2004-12-20 transformation to inline by Norbert
*/

#ifndef iRRAM_GMP_INT_RAT_H
#define iRRAM_GMP_INT_RAT_H

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include <gmp.h>
#ifdef __cplusplus
extern "C" {
#endif

/*GMP_min and GMP_max should go to the interface definitions!*/

#define GMP_min -1000000000
#define GMP_max 1000000000

/********** MP integer type definition **********/
typedef mpz_ptr int_gmp_type;
typedef struct {unsigned int body;} int_gmp_sizetype;

/********** counting vars **********/
#define MaxFreeVars 1000
extern __thread int_gmp_type gmp_FreeVarsi[MaxFreeVars];
extern __thread int gmp_FreeVarCounti;
extern __thread int int_gmp_var_count;


/********** initialization function **********/

static inline int_gmp_type int_gmp_init()
{
	int_gmp_type z;
	if (gmp_FreeVarCounti>0) {
		gmp_FreeVarCounti--;
		z = gmp_FreeVarsi[gmp_FreeVarCounti];
	}else{
		z = (int_gmp_type) malloc(sizeof(__mpz_struct));
		mpz_init(z);
	}
	int_gmp_var_count++;
	return z;
}

static inline void int_gmp_free(int_gmp_type z)
{
  if (gmp_FreeVarCounti < MaxFreeVars){
  		gmp_FreeVarsi[gmp_FreeVarCounti]=z;
  		gmp_FreeVarCounti++;
  }else{
  	mpz_clear(z);
  	free(z);
  }
  int_gmp_var_count--;
}

/********** Conversion functions **********/

static inline void int_gmp_int2integer(const int i, int_gmp_type z){mpz_set_si(z,i);}
static inline void int_gmp_double2int(const double d, int_gmp_type z){mpz_set_d(z,d);}
static inline void int_gmp_string2int(const char* s, int_gmp_type z, int b){mpz_set_str(z,s,b);}
static inline int int_gmp_integer2int(const int_gmp_type z){return mpz_get_si(z);}

/********** standard arithmetic functions for MP integer **********/

static inline void int_gmp_add(const int_gmp_type z1, const int_gmp_type z2, int_gmp_type z){mpz_add(z,z1,z2);}
static inline void int_gmp_sub(const int_gmp_type z1, const int_gmp_type z2, int_gmp_type z){mpz_sub(z,z1,z2);}
static inline void int_gmp_mul(const int_gmp_type z1, const int_gmp_type z2, int_gmp_type z){mpz_mul(z,z1,z2);}
static inline void int_gmp_div(const int_gmp_type z1, const int_gmp_type z2, int_gmp_type z){mpz_tdiv_q(z,z1,z2);}

static inline void int_gmp_add_ui(const int_gmp_type z1, const unsigned int z2, int_gmp_type z){mpz_add_ui(z,z1,z2);}
static inline void int_gmp_sub_ui(const int_gmp_type z1, const unsigned int z2,int_gmp_type z){mpz_sub_ui(z,z1,z2);}
static inline void int_gmp_mul_si(const int_gmp_type z1, const int z2, int_gmp_type z){mpz_mul_si(z,z1,z2);}
static inline void int_gmp_div_ui(const int_gmp_type z1, const unsigned int z2, int_gmp_type z){mpz_tdiv_q_ui(z,z1,z2);}

static inline void int_gmp_abs(const int_gmp_type z1, int_gmp_type z){mpz_abs(z, z1);}
static inline void int_gmp_neg(const int_gmp_type z1, int_gmp_type z){mpz_neg(z, z1);}



/********** more MP integer functions *********/

void int_gmp_root(int_gmp_type z1, unsigned int z2, int_gmp_type z);
void int_gmp_power_i(int_gmp_type z1, unsigned int z2, int_gmp_type z);
void int_gmp_power_ii(unsigned int z1, unsigned int z2, int_gmp_type z);
void int_gmp_fac(unsigned int z1, int_gmp_type z);
void int_gmp_modulo(int_gmp_type z1, int_gmp_type z2, int_gmp_type z3);
void int_gmp_sqrt(int_gmp_type z1, int_gmp_type z);
void int_gmp_shift(int_gmp_type z1, int_gmp_type z, int p);
int int_gmp_log(int_gmp_type z);


/********** output functions for MP integers **********/

void int_gmp_writee(int_gmp_type z, int w);
char* int_gmp_swritee(int_gmp_type z, int w);
void int_gmp_write(int_gmp_type z, int w);
void int_gmp_outstr(int_gmp_type z, int w);
void int_gmp_printf(int_gmp_type z);
char* int_gmp_sprintf(int_gmp_type z);


/********** copying MP integers with/without initializing **********/

static inline void int_gmp_duplicate_w_init(const int_gmp_type z1, int_gmp_type *z2){
  *z2=int_gmp_init();
  mpz_set(*z2,z1);
}

static inline void int_gmp_duplicate_wo_init(const int_gmp_type z1, int_gmp_type z2){mpz_set(z2,z1);}

/********* sign, size, and comparison of integer*/
static inline int int_gmp_sgn(const int_gmp_type z){return mpz_sgn(z);}
static inline int int_gmp_cmp(const int_gmp_type z1,const  int_gmp_type z2){return mpz_cmp(z1,z2);}
static inline int int_gmp_size(const int_gmp_type z){
	if (mpz_sgn(z) == 0 )  return 0; else   return mpz_sizeinbase(z,2);}

/********** MP rational type definition **********/
typedef mpq_ptr rat_gmp_type;

/********** counting vars **********/
#define rat_MaxFreeVars 1000
extern __thread int rat_gmp_var_count;
extern __thread rat_gmp_type rat_gmp_FreeVarsi[rat_MaxFreeVars];
extern __thread int rat_gmp_FreeVarCount;

/********** initialization function **********/

static inline rat_gmp_type rat_gmp_init(){
	rat_gmp_type z;
	if (rat_gmp_FreeVarCount>0){
		rat_gmp_FreeVarCount--;
		z = rat_gmp_FreeVarsi[rat_gmp_FreeVarCount];
	}else{
		z = (rat_gmp_type) malloc(sizeof(__mpq_struct));
		mpq_init(z);
	}
	rat_gmp_var_count++;
	return z;
}

static inline void rat_gmp_free(rat_gmp_type z){
	if(rat_gmp_FreeVarCount < rat_MaxFreeVars){
		rat_gmp_FreeVarsi[rat_gmp_FreeVarCount]=z;
		rat_gmp_FreeVarCount++;
	}else{
		mpq_clear(z);
		free(z);
	}
	rat_gmp_var_count--;
}

/* canonicalize the given rational, used for used defined rationals */
static inline void rat_gmp_canon(rat_gmp_type z){mpq_canonicalize(z);}


/********** standard arithmetic functions for MP rational **********/
/********** + - * / *********/

static inline void rat_gmp_add(const rat_gmp_type z1, const rat_gmp_type z2, rat_gmp_type z){mpq_add(z,z1,z2);}
static inline void rat_gmp_sub(const rat_gmp_type z1, const rat_gmp_type z2, rat_gmp_type z){mpq_sub(z,z1,z2);}
static inline void rat_gmp_mul(const rat_gmp_type z1, const rat_gmp_type z2, rat_gmp_type z){mpq_mul(z,z1,z2);}
static inline void rat_gmp_div(const rat_gmp_type z1, const rat_gmp_type z2, rat_gmp_type z){mpq_div(z,z1,z2);}
void rat_gmp_add_ui(const rat_gmp_type z1, const unsigned int z2, rat_gmp_type z);
void rat_gmp_sub_ui(const rat_gmp_type z1, const unsigned int z2, rat_gmp_type z);
void rat_gmp_mul_si(const rat_gmp_type z1, const int z2, rat_gmp_type z);
void rat_gmp_div_si(const rat_gmp_type z1, const int z2, rat_gmp_type z);
void rat_gmp_si_div(const int z1, const rat_gmp_type z2, rat_gmp_type z);
static inline void rat_gmp_abs(const rat_gmp_type z1, rat_gmp_type z){mpq_abs(z,z1);}


/********** more MP rational functions *********/
/********** ^!%>>... **********/

void rat_gmp_power(rat_gmp_type z1, unsigned int z2, rat_gmp_type z);
void rat_gmp_powerr(rat_gmp_type z1, rat_gmp_type z2, rat_gmp_type z);
void rat_gmp_shift(rat_gmp_type z1, rat_gmp_type z, int p);


/********** output functions for MP rationals ************/

char* rat_gmp_swritee(const rat_gmp_type z, const int w);
char* rat_gmp_sprintf(const rat_gmp_type z);


void rat_gmp_string_2_rat(rat_gmp_type z, const char* s);
void rat_gmp_double_2_rat(rat_gmp_type z, const double d);

static inline void rat_gmp_get_numerator(int_gmp_type z, const rat_gmp_type z1){mpq_get_num(z,z1);}
static inline void rat_gmp_get_denominator(int_gmp_type z, const rat_gmp_type z1){mpq_get_den(z,z1);}


/********** copying MP rationals with/without initializing ***********/
static inline void rat_gmp_duplicate_w_init(const rat_gmp_type z1, rat_gmp_type *z2){
  *z2=rat_gmp_init(); mpq_set(*z2,z1);}
static inline void rat_gmp_duplicate_wo_init(const rat_gmp_type z1, rat_gmp_type z2){mpq_set(z2,z1);}

/********* sign and comparison of integer and rational */
static inline int rat_gmp_sgn(const rat_gmp_type z){return mpq_sgn(z);}
static inline int rat_gmp_cmp(const rat_gmp_type z1,const  rat_gmp_type z2){return mpq_cmp(z1,z2);}

#ifdef __cplusplus
}
#endif

#endif /* ifndef iRRAM_GMP_INT_RAT_H */
