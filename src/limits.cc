/*

limit.cc -- Templates for general limits, lipschitz limits ...
            for the iRRAM library

Copyright (C) 2001-2005 Norbert Mueller

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
Changelog: (initial version by Norbert)

  2001-09-11 correction of ordinary limits due to
             moving from longjump to exception as iteration mechanism

*/
#include <iRRAM/limit_templates.h>

namespace iRRAM {

REAL limit         (REAL f(int, const REAL&, const REAL&),
                           const REAL& x,
                           const REAL& y)
{
  ITERATION_STACK SAVED_STACK;
  ACTUAL_STACK.inlimit+=1;
  ACTUAL_STACK.prec_step++;
  ACTUAL_STACK.actual_prec=iRRAM_prec_array[ACTUAL_STACK.prec_step];
  iRRAM_highlevel = (ACTUAL_STACK.prec_step > 1);

  REAL lim,limnew;
  sizetype limnew_error,element_error;
  sizetype lim_error,x_error,y_error;

  int element=SAVED_STACK.data.actual_prec;
  int element_step=SAVED_STACK.data.prec_step;
  int firsttime=2;

  x.geterror(x_error);
  y.geterror(y_error);

  limit_debug("starting limit_gen2");

  while (1) {
   try {
    iRRAM_DEBUG2(2,"trying to compute limit_gen2 with precision %d...\n",element);
    limnew=f(element,x,y);
    sizetype_set(element_error,1,element);
    limnew.geterror(limnew_error);
    sizetype_inc(limnew_error,element_error);
    if (firsttime ==2 ) if ( limnew_error.exponent > iRRAM_prec_array[SAVED_STACK.data.prec_step-1]
    	&&  limnew_error.exponent > x_error.exponent -iRRAM_prec_array[SAVED_STACK.data.prec_step-1]
    	&&  limnew_error.exponent > y_error.exponent -iRRAM_prec_array[SAVED_STACK.data.prec_step-1]) {
    iRRAM_DEBUG0(2,{fprintf(stderr,"computation not precise enough (%d*2^%d), trying normal p-sequence\n",
                   limnew_error.mantissa,limnew_error.exponent);});
       element_step=1;
       element=4+iRRAM_prec_array[element_step];
       firsttime=1;
    }
    if ( firsttime != 0 || sizetype_less(limnew_error,lim_error) ){
      lim=limnew;
      lim_error=limnew_error;
      iRRAM_DEBUG2(2,"getting result with error %d*2^(%d)\n",
                   lim_error.mantissa,lim_error.exponent);
      } else {
      iRRAM_DEBUG1(2,"computation successful, but no improvement\n");
      }
    firsttime=0;
    if (element<=SAVED_STACK.data.actual_prec)break;
    element_step+=4;
    element=iRRAM_prec_array[element_step];
    }
    catch ( Iteration it)  {
      if ( firsttime==0) {
      iRRAM_DEBUG1(2,"computation failed, using best success\n");
      break;
      } else
      if ( firsttime==2) {
      iRRAM_DEBUG1(2,"computation failed, trying normal p-sequence\n");
      element_step=1;
      element=4+iRRAM_prec_array[element_step];
      firsttime=1;
      } else {
      iRRAM_DEBUG1(1,"computation of limit_gen1 failed totally\n");
      REITERATE(0);
      }}
  }
  lim.seterror(lim_error);
  iRRAM_DEBUG0(2,{fprintf(stderr,"end of limit_gen2 with error %d*2^(%d)\n",
                   lim_error.mantissa,lim_error.exponent);});
  return lim;
}


REAL limit (REAL f(int))
{
  ITERATION_STACK SAVED_STACK;
  ACTUAL_STACK.inlimit+=1;
  ACTUAL_STACK.prec_step++;
  ACTUAL_STACK.actual_prec=iRRAM_prec_array[ACTUAL_STACK.prec_step];
  iRRAM_highlevel = (ACTUAL_STACK.prec_step > 1);

  REAL lim;
  sizetype lim_error;

  limit_debug("starting limit_0");

  while (1) {
   try {
    iRRAM_DEBUG2(2,"trying to compute limit_0 with precision %d...\n",ACTUAL_STACK.actual_prec);
    lim=f(SAVED_STACK.data.actual_prec);
    iRRAM_DEBUG2(2,"getting result with local error %d*2^(%d)\n", lim.error.mantissa, lim.error.exponent);
    break;
  }
    catch ( Iteration it) {
      ACTUAL_STACK.prec_step+=2;
      ACTUAL_STACK.actual_prec=iRRAM_prec_array[ACTUAL_STACK.prec_step];
      limit_debug2("limit_0 failed");
   }}
  sizetype_set(lim_error,1,SAVED_STACK.data.actual_prec);
  lim.adderror(lim_error);
  iRRAM_DEBUG0(2,{lim.geterror(lim_error);
            fprintf(stderr,"end of limit_0 with error %d*2^(%d)\n",
              lim_error.mantissa,lim_error.exponent);});
  return lim;
}

REAL limit_lip (REAL f(int,const REAL&),
            int lip_value,
	    bool on_domain(const REAL&),
            const REAL& x)
{
  if ( on_domain(x) != true ) REITERATE(0);

  ITERATION_STACK SAVED_STACK;
  ACTUAL_STACK.inlimit+=1;
  ACTUAL_STACK.prec_step++;
  ACTUAL_STACK.actual_prec=iRRAM_prec_array[ACTUAL_STACK.prec_step];
  iRRAM_highlevel = (ACTUAL_STACK.prec_step > 1);

  REAL x_new,lim;
  sizetype lim_error,x_error;

  x_new=x;
  x_new.geterror(x_error);
  sizetype_exact(x_new.error);

  limit_debug("starting limit_lip1");

  while (1) {
    try{
    iRRAM_DEBUG2(2,"trying to compute limit_lip1 with precision %d...\n",ACTUAL_STACK.actual_prec);
    lim=f(SAVED_STACK.data.actual_prec,x_new);
    if (lim.error.exponent > SAVED_STACK.data.actual_prec ) {
      ACTUAL_STACK.prec_step+=2;
      ACTUAL_STACK.actual_prec=iRRAM_prec_array[ACTUAL_STACK.prec_step];
      iRRAM_DEBUG2(2,"limit_lip1 too imprecise, increasing precision locally to %d...\n",ACTUAL_STACK.actual_prec);
    } else {
      iRRAM_DEBUG2(2,"getting result with local error %d*2^(%d)\n",
             lim.error.mantissa, lim.error.exponent);
      break;
    }}
    catch ( Iteration it){
      ACTUAL_STACK.prec_step+=2;
      ACTUAL_STACK.actual_prec=iRRAM_prec_array[ACTUAL_STACK.prec_step];
      iRRAM_DEBUG2(2,"limit_lip1 failed, increasing precision locally to %d...\n",ACTUAL_STACK.actual_prec);
    } }
  sizetype_set(lim_error,1,SAVED_STACK.data.actual_prec);
  lim.adderror(lim_error);
  sizetype_shift(lim_error,x_error,lip_value);
  lim.adderror(lim_error);
  iRRAM_DEBUG0(2,{lim.geterror(lim_error);
            fprintf(stderr,"end of limit_lip1 with error %d*2^(%d)\n",
              lim_error.mantissa,lim_error.exponent);
            fprintf(stderr,"  error of argument: %d*2^(%d)\n",
              x_error.mantissa,x_error.exponent);});
  return lim;
}

REAL limit_lip (REAL f(int,const REAL&),
            int lip_bound(const REAL&),
            const REAL& x)
{
  int lip_value;
  {
    single_valued code;
    lip_value=lip_bound(x);
  }
  
  ITERATION_STACK SAVED_STACK;
  ACTUAL_STACK.inlimit+=1;
  ACTUAL_STACK.prec_step++;
  ACTUAL_STACK.actual_prec=iRRAM_prec_array[ACTUAL_STACK.prec_step];
  iRRAM_highlevel = (ACTUAL_STACK.prec_step > 1);

  REAL x_new;
  REAL lim;
  sizetype lim_error,x_error;

  x_new=x;
  x_new.geterror(x_error);
  sizetype_exact(x_new.error);

  limit_debug("starting limit_lip1");

  while (1) {
     try{
      iRRAM_DEBUG2(2,"trying to compute limit_lip1 with precision %d...\n",ACTUAL_STACK.actual_prec);
    lim=f(SAVED_STACK.data.actual_prec,x_new);
    if (lim.error.exponent > SAVED_STACK.data.actual_prec ) {
      ACTUAL_STACK.prec_step+=2;
      ACTUAL_STACK.actual_prec=iRRAM_prec_array[ACTUAL_STACK.prec_step];
      iRRAM_DEBUG2(2,"limit_lip1 too imprecise, increasing precision locally to %d...\n",ACTUAL_STACK.actual_prec);
    } else {
      iRRAM_DEBUG2(2,"getting result with local error %d*2^(%d)\n",
             lim.error.mantissa, lim.error.exponent);
      break;
    }}
    catch ( Iteration it){
      ACTUAL_STACK.prec_step+=2;
      ACTUAL_STACK.actual_prec=iRRAM_prec_array[ACTUAL_STACK.prec_step];
      iRRAM_DEBUG2(2,"limit_lip1 failed, increasing precision locally to %d...\n",ACTUAL_STACK.actual_prec);
    } }
  sizetype_set(lim_error,1,SAVED_STACK.data.actual_prec);
  lim.adderror(lim_error);
  sizetype_shift(lim_error,x_error,lip_value);
  lim.adderror(lim_error);
  iRRAM_DEBUG0(2,{lim.geterror(lim_error);
            fprintf(stderr,"end of limit_lip1 with error %d*2^(%d)\n",
              lim_error.mantissa,lim_error.exponent);
            fprintf(stderr,"  error of argument: %d*2^(%d)\n",
              x_error.mantissa,x_error.exponent);});
  return lim;
}

REAL limit_lip     (REAL f(int, const REAL&, const REAL&),
         int lip,
         bool on_domain(const REAL&,const REAL&),
         const REAL& x,
         const REAL& y)
{
  if ( on_domain(x,y) != true ) REITERATE(0);

  ITERATION_STACK SAVED_STACK;
  ACTUAL_STACK.inlimit+=1;
  ACTUAL_STACK.prec_step++;
  ACTUAL_STACK.actual_prec=iRRAM_prec_array[ACTUAL_STACK.prec_step];
  iRRAM_highlevel = (ACTUAL_STACK.prec_step > 1);

  REAL x_new,y_new,lim;
  sizetype lim_error,x_error,y_error;

  x_new=x; x_new.geterror(x_error);  sizetype_exact(x_new.error);
  y_new=y; y_new.geterror(y_error);  sizetype_exact(y_new.error);

  limit_debug("starting limit_lip2");

  while (1) {
    try {
      iRRAM_DEBUG2(2,"trying to compute limit_lip2 with precision %d...\n",ACTUAL_STACK.actual_prec);
      lim=f(SAVED_STACK.data.actual_prec,x_new,y_new);
     if (lim.error.exponent > SAVED_STACK.data.actual_prec ) {
      ACTUAL_STACK.prec_step+=2;
      ACTUAL_STACK.actual_prec=iRRAM_prec_array[ACTUAL_STACK.prec_step];
      iRRAM_DEBUG2(2,"limit_lip2 too imprecise, increasing precision locally to %d...\n",ACTUAL_STACK.actual_prec);
    } else {
      iRRAM_DEBUG2(2,"getting result with local error %d*2^(%d)\n",
             lim.error.mantissa, lim.error.exponent);
      break;
    }}
    catch ( Iteration it) {
      ACTUAL_STACK.prec_step+=2;
      ACTUAL_STACK.actual_prec=iRRAM_prec_array[ACTUAL_STACK.prec_step];
      iRRAM_DEBUG2(2,"limit_lip2 failed, increasing precision locally to %d...\n",ACTUAL_STACK.actual_prec);
    }}
  sizetype_set(lim_error,1,SAVED_STACK.data.actual_prec);
  lim.adderror(lim_error);
  sizetype_shift(lim_error,x_error,lip);
  lim.adderror(lim_error);
  sizetype_shift(lim_error,y_error,lip);
  lim.adderror(lim_error);
  iRRAM_DEBUG0(2,{lim.geterror(lim_error);
            fprintf(stderr,"end of limit_lip2 with error %d*2^(%d)\n",
              lim_error.mantissa,lim_error.exponent);
            fprintf(stderr,"  error of argument 1: %d*2^(%d)\n",
              x_error.mantissa,x_error.exponent);
            fprintf(stderr,"  error of argument 2: %d*2^(%d)\n",
              y_error.mantissa,y_error.exponent);});
  return lim;
}

REAL lipschitz (REAL f(const REAL&),
            int lip,
	    bool on_domain(const REAL&),
            const REAL& x)
{
  if ( on_domain(x) != true ) REITERATE(0);

  REAL x_new,lip_result;
  sizetype lip_error,x_error;

  x_new=x;
  x_new.geterror(x_error);
  sizetype_exact(x_new.error);
  {
    single_valued code;
  iRRAM_DEBUG1(2,"starting lipschitz1 ...\n");
  lip_result=f(x_new);
  iRRAM_DEBUG2(2,"getting result with local error %d*2^(%d)\n",
             lip_result.error.mantissa, lip_result.error.exponent);
  }
  sizetype_shift(lip_error,x_error,lip);
  lip_result.adderror(lip_error);
  iRRAM_DEBUG0(2,{lip_result.geterror(lip_error);
            fprintf(stderr,"end of lipschitz_1 with error %d*2^(%d)\n",
              lip_error.mantissa,lip_error.exponent);
            fprintf(stderr,"  for argument with error %d*2^(%d)\n",
              x_error.mantissa,x_error.exponent);});
  return lip_result;
}

REAL lipschitz (REAL f(const REAL&),
            REAL lip_f(const REAL&),
	    bool on_domain(const REAL&),
            const REAL& x)
{
  if ( on_domain(x) != true ) REITERATE(0);

  REAL x_new,lip_result,lip_bound;
  sizetype lip_error,lip_size,tmp_size,x_error;

  ITERATION_STACK SAVED_STACK;

  {
    single_valued code;
  iRRAM_DEBUG1(2,"starting lipschitz1b ...\n");

// for the computation of the Lipschitz bound, we work with
// reduced precision:
  ACTUAL_STACK.prec_step=(ACTUAL_STACK.prec_step+1)/2;
  ACTUAL_STACK.actual_prec=iRRAM_prec_array[ACTUAL_STACK.prec_step];
  iRRAM_highlevel = (ACTUAL_STACK.prec_step > 1);

  lip_bound=lip_f(x);

  ACTUAL_STACK.prec_step=SAVED_STACK.data.prec_step;
  ACTUAL_STACK.actual_prec=iRRAM_prec_array[ACTUAL_STACK.prec_step];
  iRRAM_highlevel = (ACTUAL_STACK.prec_step > 1);

  x_new=x;
  x_new.geterror(x_error);
  sizetype_exact(x_new.error);


  bool try_it=true;
  while (try_it) {
  try { try_it=false;
        lip_result=f(x_new); }
  catch ( Iteration it)  { try_it=true;
      ACTUAL_STACK.prec_step+=2;
      ACTUAL_STACK.actual_prec=iRRAM_prec_array[ACTUAL_STACK.prec_step];
      iRRAM_highlevel = (ACTUAL_STACK.prec_step > 1);
      iRRAM_DEBUG2(2,"limit_lip2 failed, increasing precision locally to %d...\n",ACTUAL_STACK.actual_prec);
    }
  }
  }
  
  iRRAM_DEBUG2(2,"getting result with local error %d*2^(%d)\n",
             lip_result.error.mantissa, lip_result.error.exponent);
  lip_bound.getsize(lip_size);
  lip_bound.geterror(tmp_size);
  sizetype_inc(lip_size,tmp_size);
  sizetype_mult(lip_error,lip_size,x_error);
  lip_result.adderror(lip_error);
  iRRAM_DEBUG0(2,{lip_result.geterror(lip_error);
            fprintf(stderr,"end of lipschitz_1b with error %d*2^(%d)\n",
              lip_error.mantissa,lip_error.exponent);
            fprintf(stderr,"  for argument with error %d*2^(%d)\n",
              x_error.mantissa,x_error.exponent);});
  return lip_result;
}

REAL lipschitz (REAL f(const REAL&),
            REAL lip_f(const REAL&),
            const REAL& x)
{
  REAL x_new,lip_result,lip_bound;
  sizetype lip_error,lip_size,tmp_size,x_error;

  x_new=x;
  x_new.geterror(x_error);
  sizetype_exact(x_new.error);
  
  ITERATION_STACK SAVED_STACK;

  {
    single_valued code;
  iRRAM_DEBUG1(2,"starting lipschitz1a ...\n");
  lip_result=f(x_new);

// for the computation of the Lipschitz bound, we work with
// reduced precision:
  int saved_step=ACTUAL_STACK.prec_step;
  ACTUAL_STACK.prec_step=(ACTUAL_STACK.prec_step+1)/2;
  ACTUAL_STACK.actual_prec=iRRAM_prec_array[ACTUAL_STACK.prec_step];
  iRRAM_highlevel = (ACTUAL_STACK.prec_step > 1);

  lip_bound=lip_f(x);

  ACTUAL_STACK.prec_step=saved_step;
  ACTUAL_STACK.actual_prec=iRRAM_prec_array[ACTUAL_STACK.prec_step];
  iRRAM_highlevel = (ACTUAL_STACK.prec_step > 1);

  iRRAM_DEBUG2(2,"getting result with local error %d*2^(%d)\n",
             lip_result.error.mantissa, lip_result.error.exponent);
  }
  lip_bound.getsize(lip_size);
  lip_bound.geterror(tmp_size);
  sizetype_inc(lip_size,tmp_size);
  sizetype_mult(lip_error,lip_size,x_error);
  lip_result.adderror(lip_error);
  iRRAM_DEBUG0(2,{lip_result.geterror(lip_error);
            fprintf(stderr,"end of lipschitz_1a with error %d*2^(%d)\n",
              lip_error.mantissa,lip_error.exponent);
            fprintf(stderr,"  for argument with error %d*2^(%d)\n",
              x_error.mantissa,x_error.exponent);});
  return lip_result;
}

REAL lipschitz (REAL f(int, const REAL&),
            int lip,
	    bool on_domain(int k,const REAL&),
            int k,
            const REAL& x)
{
  if ( on_domain(k,x) != true ) REITERATE(0);
  REAL x_new,lip_result;
  sizetype lip_error,x_error;

  x_new=x;
  x_new.geterror(x_error);
  sizetype_exact(x_new.error);
  {
    single_valued code;
  iRRAM_DEBUG1(2,"starting lipschitz1 ...\n");
  lip_result=f(k,x_new);
  iRRAM_DEBUG2(2,"getting result with local error %d*2^(%d)\n",
             lip_result.error.mantissa, lip_result.error.exponent);
  }
  sizetype_shift(lip_error,x_error,lip);
  lip_result.adderror(lip_error);
  iRRAM_DEBUG0(2,{lip_result.geterror(lip_error);
            fprintf(stderr,"end of lipschitz_1 with error %d*2^(%d)\n",
              lip_error.mantissa,lip_error.exponent);
            fprintf(stderr,"  error of argument: %d*2^(%d)\n",
              x_error.mantissa,x_error.exponent);});
  return lip_result;
}

REAL lipschitz (REAL f(const REAL&, const REAL&),
            int lip,
	    bool on_domain(const REAL&,const REAL&),
            const REAL& x,
            const REAL& y)
{
  if ( on_domain(x,y) != true ) REITERATE(0);
  REAL x_new,y_new,lip_result;
  sizetype lip_error,x_error,y_error;

  x_new=x;
  x_new.geterror(x_error);
  sizetype_exact(x_new.error);
  y_new=y;
  y_new.geterror(y_error);
  sizetype_exact(y_new.error);
  {
    single_valued code;
  iRRAM_DEBUG1(2,"starting lipschitz2 ...\n");
  lip_result=f(x_new,y_new);
  iRRAM_DEBUG2(2,"getting result with local error %d*2^(%d)\n",
             lip_result.error.mantissa, lip_result.error.exponent);
  }
  sizetype_shift(lip_error,x_error,lip);
  lip_result.adderror(lip_error);
  sizetype_shift(lip_error,y_error,lip);
  lip_result.adderror(lip_error);
  iRRAM_DEBUG0(2,{lip_result.geterror(lip_error);
            fprintf(stderr,"end of lipschitz_2 with error %d*2^(%d)\n",
              lip_error.mantissa,lip_error.exponent);
            fprintf(stderr,"  error of argument x: %d*2^(%d)\n",
              x_error.mantissa,x_error.exponent);
            fprintf(stderr,"  error of argument y: %d*2^(%d)\n",
              y_error.mantissa,y_error.exponent);});
  return lip_result;
}

REAL lipschitz (REAL f(int, const REAL&, const REAL&),
            int lip,
	    bool on_domain(int k,const REAL&,const REAL&),
            int k,
            const REAL& x,
            const REAL& y)
{
  if ( on_domain(k,x,y) != true ) REITERATE(0);
  REAL x_new,y_new,lip_result;
  sizetype x_error,y_error,lip_error;

  x_new=x;
  x_new.geterror(x_error);
  sizetype_exact(x_new.error);
  y_new=y;
  y_new.geterror(y_error);
  sizetype_exact(y_new.error);
  x_new.geterror(x_error);
  {
    single_valued code;
  iRRAM_DEBUG1(2,"starting lipschitz2 ...\n");
  lip_result=f(k,x_new,y_new);
  iRRAM_DEBUG2(2,"getting result with local error %d*2^(%d)\n",
             lip_result.error.mantissa, lip_result.error.exponent);
  }
  sizetype_shift(lip_error,x_error,lip);
  lip_result.adderror(lip_error);
  sizetype_shift(lip_error,y_error,lip);
  lip_result.adderror(lip_error);
  iRRAM_DEBUG0(2,{lip_result.geterror(lip_error);
            fprintf(stderr,"end of lipschitz_2 with error %d*2^(%d)\n",
              lip_error.mantissa,lip_error.exponent);
            fprintf(stderr,"  error of argument x: %d*2^(%d)\n",
              x_error.mantissa,x_error.exponent);
            fprintf(stderr,"  error of argument y: %d*2^(%d)\n",
              y_error.mantissa,y_error.exponent);});
  return lip_result;
}

REAL limit_hint    (REAL f(int, const REAL&),
                    int hint,
                    const REAL& x)
{
  ITERATION_STACK SAVED_STACK;
  ACTUAL_STACK.inlimit+=1;
  ACTUAL_STACK.prec_step++;
  ACTUAL_STACK.actual_prec=iRRAM_prec_array[ACTUAL_STACK.prec_step];
  iRRAM_highlevel = (ACTUAL_STACK.prec_step > 1);

  REAL lim,limnew;
  sizetype limnew_error,element_error;
  sizetype lim_error;
  int hintcopy=hint;
  int success=0;

  x.geterror(element_error);
  int element=max(element_error.exponent,SAVED_STACK.data.actual_prec);

  iRRAM_DEBUG1(2,"starting limit_hint1...\n");

  while (1) {
    try {
    iRRAM_DEBUG2(2,"trying to compute limit_hint1 with precicion 2^(%d)...\n",element);
    limnew=f(element,x);
    sizetype_set(element_error,1,element);
    limnew.geterror(limnew_error);
    sizetype_inc(limnew_error,element_error);
    if ( (! success) || sizetype_less(limnew_error,lim_error) ) {
      lim=limnew;
      lim_error=limnew_error;
      iRRAM_DEBUG2(2,"getting result with error %d*2^(%d)\n",
               lim_error.mantissa, lim_error.exponent);
      } else {
      iRRAM_DEBUG1(2,"computation successful, but no improvement\n");
      hintcopy=2*hintcopy;
      }
    success=1;
    if (element>iRRAM_prec_array[1])break;
    element=element+hint;
  }
    catch ( Iteration it) {
    if (element>iRRAM_prec_array[1])break;
    element=element+hintcopy;
    }}
  if ( ! success) {
    iRRAM_DEBUG1(1,"computation of limit_hint1 failed\n");
    REITERATE(0);
  }
  lim.seterror(lim_error);
  iRRAM_DEBUG0(2,{fprintf(stderr,"end of limit_hint1 with error %d*2^(%d)\n",
                   lim_error.mantissa,lim_error.exponent);});
  return lim;
}


REAL limit_hint    (REAL f(int, const REAL&, const REAL&),
                    int hint,
                    const REAL& x, const REAL&y)
{
  ITERATION_STACK SAVED_STACK;
  ACTUAL_STACK.inlimit+=1;
  ACTUAL_STACK.prec_step++;
  ACTUAL_STACK.actual_prec=iRRAM_prec_array[ACTUAL_STACK.prec_step];
  iRRAM_highlevel = (ACTUAL_STACK.prec_step > 1);

  REAL lim,limnew;
  sizetype limnew_error,element_error;
  sizetype lim_error;
  int hintcopy=hint;
  int success=0;

  x.geterror(element_error);
  int element=max(element_error.exponent,SAVED_STACK.data.actual_prec);

  limit_debug("starting limit_hint1");

  while (1) {
    try {
    iRRAM_DEBUG2(2,"trying to compute limit_hint1 with precicion 2^(%d)...\n",element);
    limnew=f(element,x,y);
    sizetype_set(element_error,1,element);
    limnew.geterror(limnew_error);
    sizetype_inc(limnew_error,element_error);
    if ( (! success) || sizetype_less(limnew_error,lim_error) ) {
      lim=limnew;
      lim_error=limnew_error;
      iRRAM_DEBUG2(2,"getting result with error %d*2^(%d)\n",
               lim_error.mantissa, lim_error.exponent);
      } else {
      iRRAM_DEBUG1(2,"computation successful, but no improvement\n");
      hintcopy=2*hintcopy;
      }
    success=1;
    if (element>iRRAM_prec_array[1])break;
    element=element+hint;
  }
    catch ( Iteration it) {
    if (element>iRRAM_prec_array[1])break;
    element=element+hintcopy;
    }}
  if ( ! success) {
    iRRAM_DEBUG1(1,"computation of limit_hint1 failed\n");
    REITERATE(0);
  }
  lim.seterror(lim_error);
  iRRAM_DEBUG0(2,{fprintf(stderr,"end of limit_hint1 with error %d*2^(%d)\n",
                   lim_error.mantissa,lim_error.exponent);});
  return lim;
}


REALMATRIX limit_lip (REALMATRIX f(int,const REALMATRIX&),
            int lip,
	    bool on_domain(const REALMATRIX&),
            const REALMATRIX& x)
{
  if ( on_domain(x) != true ) REITERATE(0);

  ITERATION_STACK SAVED_STACK;
  ACTUAL_STACK.inlimit+=1;
  ACTUAL_STACK.prec_step++;
  ACTUAL_STACK.actual_prec=iRRAM_prec_array[ACTUAL_STACK.prec_step];
  iRRAM_highlevel = (ACTUAL_STACK.prec_step > 1);


  REALMATRIX x_new,lim,limnew;
  sizetype lim_error;

  sizetype_exact(lim_error);
  x_new=x; x_new.seterror(lim_error);

  limit_debug("starting limit_matrix_lip1");

  while (1) {
   try {
     iRRAM_DEBUG2(2,"trying to compute limit_matrix_lip1 with precision %d...\n",ACTUAL_STACK.actual_prec);
     limnew=f(SAVED_STACK.data.actual_prec,x_new);
     lim=limnew;
     lim.geterror(lim_error);
     if (lim_error.exponent > SAVED_STACK.data.actual_prec ) {
      ACTUAL_STACK.prec_step+=2;
      ACTUAL_STACK.actual_prec=iRRAM_prec_array[ACTUAL_STACK.prec_step];
      iRRAM_DEBUG2(2,"limit_lip too imprecise, increasing precision locally to %d...\n",ACTUAL_STACK.actual_prec);
    } else {
      iRRAM_DEBUG0(2,fprintf(stderr,"getting result with local error %d*2^(%d)\n",
                lim_error.mantissa,lim_error.exponent););
    break;
  }}
    catch ( Iteration it)  {
      ACTUAL_STACK.prec_step+=2;
      ACTUAL_STACK.actual_prec=iRRAM_prec_array[ACTUAL_STACK.prec_step];
      iRRAM_DEBUG2(2,"limit_lip1 failed, increasing precision locally to %d...\n",ACTUAL_STACK.actual_prec);
    }}
  sizetype_exact(lim_error);
  lim.adderror(lim_error);
  x.geterror(lim_error);
  sizetype_shift(lim_error,lim_error,lip);
  lim.adderror(lim_error);
  iRRAM_DEBUG0(2,{lim.geterror(lim_error);
            fprintf(stderr,"end of limit_matrix_lip1 with error %d*2^(%d)\n",
                lim_error.mantissa,lim_error.exponent);});

  return lim;
}


// REAL iteration (void f(REAL&,REAL&,const int& param),
//             const REAL& l,const REAL& r,const int& param)
// {
// 
//   REAL lc=l;
//   REAL rc=r;
//   ITERATION_STACK SAVED_STACK;
//   ACTUAL_STACK.inlimit+=1;
// 
//   sizetype no_error,diff_size,error;
//   sizetype_exact(no_error);
//   lc.seterror(no_error);
//   rc.seterror(no_error);
//   REAL diff;
//   REAL lcc,rcc;
// 
//   ACTUAL_STACK.actual_prec=-50;//int((upperbound(rc-lc)-50)*1);
//   ACTUAL_STACK.prec_inc=-50;
// 
// 
//   iRRAM_DEBUG1(2,"starting iteration...\n");
//   while (1) {
//      try{
//       iRRAM_DEBUG2(2,"trying to compute iteration with precision %d...\n",ACTUAL_STACK.actual_prec);
//       REAL lcc=lc;
//       REAL rcc=rc;
//       f(lcc,rcc,param);
//       lcc.geterror(error);
//       lcc.seterror(no_error);
//       lc=lcc-scale(REAL(int(error.mantissa)),error.exponent);
//       rcc.geterror(error);
//       rcc.seterror(no_error);
//       rc=rcc-scale(REAL(int(error.mantissa)),error.exponent);
// stiff_begin();stiff_begin();
//       diff=rc-lc;
// stiff_end();stiff_end();
// 
//       if (diff.vsize.exponent > SAVED_STACK.actual_prec ) {
//         iRRAM_DEBUG0(2,{ fprintf(stderr,"iteration with error %d*2^(%d)\n",
//               diff.vsize.mantissa,diff.vsize.exponent);});
//       ACTUAL_STACK.prec_inc=int(ACTUAL_STACK.prec_inc * ACTUAL_STACK.prec_factor)+iRRAM_prec_inc1;
//       ACTUAL_STACK.actual_prec=iRRAM_starting_prec+ACTUAL_STACK.prec_inc;
//       iRRAM_DEBUG2(2,"iteration result too imprecise, trying a new iteration with %d...\n",ACTUAL_STACK.actual_prec);
//     } else {
//       iRRAM_DEBUG2(2,"getting result with local error %d*2^(%d)\n",
//              diff.vsize.mantissa, diff.vsize.exponent);
//       break;
//     }}
//     catch ( Iteration it){
//       ACTUAL_STACK.prec_inc=int(ACTUAL_STACK.prec_inc * ACTUAL_STACK.prec_factor)+iRRAM_prec_inc1;
//       ACTUAL_STACK.actual_prec=iRRAM_starting_prec+ACTUAL_STACK.prec_inc;
//       iRRAM_DEBUG2(2,"iteration failed, increasing precision locally to %d...\n",ACTUAL_STACK.actual_prec);
//     } }
//       sizetype_half(diff_size,diff.vsize);
//       lc=scale(lc+rc,-1);
//       lc.adderror(diff_size);
//   iRRAM_DEBUG0(2,{ fprintf(stderr,"end of iteration with error %d*2^(%d)\n",
//               diff_size.mantissa,diff_size.exponent);});
//   return lc;
// }


REAL iteration (void f(REAL&,REAL&,const int& param),
            const REAL& l,const REAL& r,const int& param)
{
  ITERATION_STACK SAVED_STACK;
  ACTUAL_STACK.inlimit+=1;
  ACTUAL_STACK.prec_step=1;
  ACTUAL_STACK.actual_prec=iRRAM_prec_array[ACTUAL_STACK.prec_step];//int((upperbound(rc-lc)-50)*1);
  iRRAM_highlevel = (ACTUAL_STACK.prec_step > 1);

  REAL lc=l;
  REAL rc=r;
  sizetype no_error,diff_size_h,diff_size,error,diff_old_size;
  sizetype_exact(no_error);
  lc.seterror(no_error);
  rc.seterror(no_error);
  REAL diff=rc-lc;
  diff.getsize(diff_size);
  REAL lcc,rcc;


  iRRAM_DEBUG1(2,"starting iteration...\n");
  while (1) {
     try{
      iRRAM_DEBUG2(2,"trying to compute iteration with precision %d...\n",ACTUAL_STACK.actual_prec);
      REAL lcc=lc;
      REAL rcc=rc;
      diff_old_size=diff_size;
      f(lcc,rcc,param);
      lcc.geterror(error);
      lcc.seterror(no_error);
      lc=lcc-scale(REAL(int(error.mantissa)),error.exponent);
      rcc.geterror(error);
      rcc.seterror(no_error);
      rc=rcc-scale(REAL(int(error.mantissa)),error.exponent);
      {
        stiff code(+2);
        diff=rc-lc;
      }
      diff.getsize(diff_size);
      if (diff_size.exponent > SAVED_STACK.data.actual_prec ) {
        iRRAM_DEBUG0(2,{ fprintf(stderr,"iteration with error %d*2^(%d)\n",
              diff_size.mantissa,diff_size.exponent);});
      if (diff_size.exponent >= diff_old_size.exponent ) {
      ACTUAL_STACK.prec_step+=2;
      ACTUAL_STACK.actual_prec=iRRAM_prec_array[ACTUAL_STACK.prec_step];}
      iRRAM_DEBUG2(2,"iteration result too imprecise, trying a new iteration with %d...\n",ACTUAL_STACK.actual_prec);
    } else {
      iRRAM_DEBUG2(2,"getting result with local error %d*2^(%d)\n",
             diff_size.mantissa, diff_size.exponent);
      break;
    }}
    catch ( Iteration it){
      ACTUAL_STACK.prec_step+=2;
      ACTUAL_STACK.actual_prec=iRRAM_prec_array[ACTUAL_STACK.prec_step];
      iRRAM_DEBUG2(2,"iteration failed, increasing precision locally to %d...\n",ACTUAL_STACK.actual_prec);
    } }
      sizetype_half(diff_size_h,diff_size);
      lc=scale(lc+rc,-1);
      lc.adderror(diff_size_h);
      lc.geterror(error);
  iRRAM_DEBUG0(2,{ fprintf(stderr,"end of iteration with error %d*2^(%d)\n",
              error.mantissa,error.exponent);});
  return lc;
}

//********************************************************************************
// general limit operator for FUNCTION objects on REAL numbers
//
// REAL limit ( FUNCTION<REAL,int> f )
// if FUNCTION f defines a normed Cauchy sequence, i.e. |f(i)-x|<= 2^{i}
// then limit(f) returns x
//********************************************************************************

REAL limit (const FUNCTION<REAL,int> & f )
{
  ITERATION_STACK SAVED_STACK;
  ACTUAL_STACK.inlimit+=1;
  ACTUAL_STACK.prec_step++;
  ACTUAL_STACK.actual_prec=iRRAM_prec_array[ACTUAL_STACK.prec_step];
  iRRAM_highlevel = (ACTUAL_STACK.prec_step > 1);

  REAL lim,limnew;
  sizetype limnew_error,element_error;
  sizetype lim_error;

  int element=SAVED_STACK.data.actual_prec;
  int element_step=SAVED_STACK.data.prec_step;
  int firsttime=2;

  limit_debug("starting limit_FUNCTION");

  while (1) {
    try {
    iRRAM_DEBUG2(2,"trying to compute limit_FUNCTION with precicion 2^(%d)...\n",element);
    limnew=f(element);
    sizetype_set(element_error,1,element);
    limnew.geterror(limnew_error);
    sizetype_inc(limnew_error,element_error);
    if (firsttime ==2 ) if ( limnew_error.exponent > iRRAM_prec_array[SAVED_STACK.data.prec_step-1]) {
    iRRAM_DEBUG0(2,{cerr<<"computation not precise enough ("
                  << limnew_error.mantissa <<"*2^"<< limnew_error.exponent
                  <<"), trying normal p-sequence\n";});
       element_step=1;
       element=4+iRRAM_prec_array[element_step];
       firsttime=1;
    }
    if ( firsttime != 0 || sizetype_less(limnew_error,lim_error) ) {
      lim=limnew;
      lim_error=limnew_error;
      iRRAM_DEBUG2(2,"getting result with error %d*2^(%d)\n",
               lim_error.mantissa, lim_error.exponent);
      } else {
      iRRAM_DEBUG1(2,"computation successful, but no improvement\n");
      }
    firsttime=0;
    if (element<=SAVED_STACK.data.actual_prec)break;
    element_step+=4;
    element=iRRAM_prec_array[element_step];
    }
    catch ( Iteration it) {
      if ( firsttime==0) {
      iRRAM_DEBUG1(2,"computation failed, using best success\n");
      break;
      } else
      if ( firsttime==2) {
      iRRAM_DEBUG1(2,"computation failed, trying normal p-sequence\n");
      element_step=1;
      element=4+iRRAM_prec_array[element_step];
      firsttime=1;
      } else {
      iRRAM_DEBUG1(1,"computation of limit_FUNCTION failed totally\n");
      REITERATE(0);
      }}
  }
  lim.seterror(lim_error);
  iRRAM_DEBUG0(2,{cerr<<"end of limit_FUNCTION with error "
                << lim_error.mantissa <<"*2^("<< lim_error.exponent<<")\n";});
  return lim;
}

// Instantiation of templates for multi-valued limits:
template COMPLEX limit_mv<COMPLEX,COMPLEX>
    (COMPLEX (*)(int, int *, COMPLEX const &), COMPLEX const &);

template REALMATRIX limit_mv<REALMATRIX,REALMATRIX>
    (REALMATRIX (*)(int, int *, REALMATRIX const &), REALMATRIX const &);

// Instantiation of templates for single-valued limits, one parameter:

template REALMATRIX limit <REALMATRIX,REALMATRIX>
    (REALMATRIX (*)(int, REALMATRIX const &), REALMATRIX const &);

template REAL limit <REAL,REAL>
    (REAL (*)(int, REAL const &), REAL const &);

template COMPLEX limit <COMPLEX,COMPLEX>
    (COMPLEX (*)(int, COMPLEX const &), COMPLEX const &);

// Instantiation of templates for single-valued limits, one parameter + int:
template REAL limit <REAL,int,REAL>
    (REAL (*)(int, REAL const &, int param), REAL const &,int param);

} // namespace iRRAM
