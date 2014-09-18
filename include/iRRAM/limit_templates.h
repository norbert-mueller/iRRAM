/*

iRRAM_limit_templates.h -- template definitions file for the limit operators of the iRRAM library
 
Copyright (C) 2005 Norbert Mueller
 
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
#ifndef iRRAM_limit_templates_h
#define iRRAM_limit_templates_h

#include "iRRAM/core.h"

namespace iRRAM {

inline void limit_debug(const char* c){
  if ( unlikely(iRRAM_debug > 0) ){
    if (iRRAM_debug >=ACTUAL_STACK.inlimit + 2 )
		cerr << c <<"...\n";
    if (iRRAM_max_prec <= ACTUAL_STACK.prec_step) 
		iRRAM_max_prec  = ACTUAL_STACK.prec_step;
  }
}

inline void limit_debug2(const char* c){
      if ( unlikely(iRRAM_debug > 0) ) {
	    if (iRRAM_debug >=ACTUAL_STACK.inlimit + 2 )
		cerr << c <<", increasing precision locally to "
			<<ACTUAL_STACK.actual_prec<<"\n";
	    if (iRRAM_max_prec <= ACTUAL_STACK.prec_step) 
		iRRAM_max_prec  = ACTUAL_STACK.prec_step;
      }
}

template <class ARGUMENT, class RESULT>
RESULT limit (RESULT f(int prec,const ARGUMENT&),
                           const ARGUMENT& x)

{
  ITERATION_STACK SAVED_STACK;
  ACTUAL_STACK.inlimit+=1;
  ACTUAL_STACK.prec_step++;
  ACTUAL_STACK.actual_prec=iRRAM_prec_array[ACTUAL_STACK.prec_step];
  iRRAM_highlevel = (ACTUAL_STACK.prec_step > 1);

  RESULT lim,limnew;
  sizetype limnew_error,element_error;
  sizetype lim_error,x_error;

  int element=SAVED_STACK.data.actual_prec;
  int element_step=SAVED_STACK.data.prec_step;
  int firsttime=2;

  x.geterror(x_error);

  limit_debug("starting limit_gen1");

  while (1) {
    try {
    DEBUG2(2,"trying to compute limit_gen1 with precicion 2^(%d)...\n",element);
    limnew=f(element,x);
    sizetype_set(element_error,1,element);
    limnew.geterror(limnew_error);
    sizetype_inc(limnew_error,element_error);
    if (firsttime ==2 ) if ( limnew_error.exponent > iRRAM_prec_array[SAVED_STACK.data.prec_step-1]
    	&&  limnew_error.exponent > x_error.exponent -iRRAM_prec_array[SAVED_STACK.data.prec_step-1]) {
    DEBUG0(2,{cerr<<"computation not precise enough ("
                  << limnew_error.mantissa <<"*2^"<< limnew_error.exponent
                  <<"), trying normal p-sequence\n";});
       element_step=1;
       element=4+iRRAM_prec_array[element_step];
       firsttime=1;
    }
    if ( firsttime != 0 || sizetype_less(limnew_error,lim_error) ) {
      lim=limnew;
      lim_error=limnew_error;
      DEBUG3(2,"getting result with error %d*2^(%d)\n",
               lim_error.mantissa, lim_error.exponent);
      } else {
      DEBUG1(2,"computation successful, but no improvement\n");
      }
    firsttime=0;
    if (element<=SAVED_STACK.data.actual_prec)break;
    element_step+=4;
    element=iRRAM_prec_array[element_step];
    }
    catch ( Iteration it) {
      if ( firsttime==0) {
      DEBUG1(2,"computation failed, using best success\n");
      break;
      } else
      if ( firsttime==2) {
      DEBUG1(2,"computation failed, trying normal p-sequence\n");
      element_step=1;
      element=4+iRRAM_prec_array[element_step];
      firsttime=1;
      } else {
      DEBUG1(1,"computation of limit_gen1 failed totally\n");
      REITERATE(0);
      }}
  }
  lim.seterror(lim_error);
  DEBUG0(2,{cerr<<"end of limit_gen1 with error "
                << lim_error.mantissa <<"*2^("<< lim_error.exponent<<")\n";});
  return lim;
}

template <class ARGUMENT, class DISCRETE, class RESULT>
RESULT limit (RESULT f(int prec,const ARGUMENT&,DISCRETE param),
                           const ARGUMENT& x,DISCRETE param)

{
  ITERATION_STACK SAVED_STACK;
  ACTUAL_STACK.inlimit+=1;
  ACTUAL_STACK.prec_step++;
  ACTUAL_STACK.actual_prec=iRRAM_prec_array[ACTUAL_STACK.prec_step];
  iRRAM_highlevel = (ACTUAL_STACK.prec_step > 1);

  RESULT lim,limnew;
  sizetype limnew_error,element_error;
  sizetype lim_error,x_error;

  int element=SAVED_STACK.data.actual_prec;
  int element_step=SAVED_STACK.data.prec_step;
  int firsttime=2;

  x.geterror(x_error);

  limit_debug("starting limit_gen1");

  while (1) {
   try {
    DEBUG2(2,"trying to compute limit_gen1 with precicion 2^(%d)...\n",element);
    limnew=f(element,x,param);
    sizetype_set(element_error,1,element);
    limnew.geterror(limnew_error);
    sizetype_inc(limnew_error,element_error);
    if (firsttime ==2 ) if ( limnew_error.exponent > iRRAM_prec_array[SAVED_STACK.data.prec_step-1]
    	&&  limnew_error.exponent > x_error.exponent -iRRAM_prec_array[SAVED_STACK.data.prec_step-1]) {
    DEBUG0(2,{cerr<<"computation not precise enough ("
                  << limnew_error.mantissa <<"*2^"<< limnew_error.exponent
                  <<"), trying normal p-sequence\n";});
       element_step=1;
       element=4+iRRAM_prec_array[element_step];
       firsttime=1;
    }
    if ( firsttime  != 0 || sizetype_less(limnew_error,lim_error) ) {
      lim=limnew;
      lim_error=limnew_error;
      DEBUG3(2,"getting result with error %d*2^(%d)\n",
               lim_error.mantissa, lim_error.exponent);
      } else {
      DEBUG1(2,"computation successful, but no improvement\n");
      }
    firsttime=0;
    if (element<=SAVED_STACK.data.actual_prec)break;
    element_step+=4;
    element=iRRAM_prec_array[element_step];
    }
    catch ( Iteration it) {
      if ( firsttime==0) {
      DEBUG1(2,"computation failed, using best success\n");
      break;
      } else
      if ( firsttime==2) {
      DEBUG1(2,"computation failed, trying normal p-sequence\n");
      element_step=1;
      element=4+iRRAM_prec_array[element_step];
      firsttime=1;
      } else {
      DEBUG1(1,"computation of limit_gen1 failed totally\n");
      REITERATE(0);
      }}
  }
  lim.seterror(lim_error);
  DEBUG0(2,{fprintf(stderr,"end of limit_gen1 with error %d*2^(%d)\n",
                   lim_error.mantissa,lim_error.exponent);});
  return lim;
}

template <class ARGUMENT, class RESULT>
RESULT limit_mv (RESULT f(int prec,
                          int* choice,
                          const ARGUMENT&),
                          const ARGUMENT& x)
{
  ITERATION_STACK SAVED_STACK;
  ACTUAL_STACK.inlimit+=1;
  ACTUAL_STACK.prec_step++;
  ACTUAL_STACK.actual_prec=iRRAM_prec_array[ACTUAL_STACK.prec_step];
  iRRAM_highlevel = (ACTUAL_STACK.prec_step > 1);

  RESULT lim,limnew;
  sizetype limnew_error,element_error;
  int choice=0;
  sizetype lim_error,x_error;

  int element=SAVED_STACK.data.actual_prec;
  int element_step=SAVED_STACK.data.prec_step;
  int firsttime=2;

  if ( (ACTUAL_STACK.inlimit==0) && !iRRAM_thread_data_address->cache_i.get(choice)) iRRAM_thread_data_address->cache_i.put(choice);

  x.geterror(x_error);

  limit_debug("starting limit_mv");

  while (1) {
   try {
    DEBUG2(2,"trying to compute limit_mv with precicion 2^(%d)...\n",element);
    limnew=f(element,&choice,x);
    if ( SAVED_STACK.data.inlimit==0 ) iRRAM_thread_data_address->cache_i.modify(choice);
    sizetype_set(element_error,1,element);
    limnew.geterror(limnew_error);
    sizetype_inc(limnew_error,element_error);
    if (firsttime ==2 ) if ( limnew_error.exponent > iRRAM_prec_array[SAVED_STACK.data.prec_step-1]
    	&&  limnew_error.exponent > x_error.exponent -iRRAM_prec_array[SAVED_STACK.data.prec_step-1]) {
    DEBUG0(2,{fprintf(stderr,"computation not precise enough (%d*2^%d), trying normal p-sequence\n",
                   limnew_error.mantissa,limnew_error.exponent);});
       element_step=1;
       element=4+iRRAM_prec_array[element_step];
       firsttime=1;
    }}
     catch ( Iteration it) {
      if ( firsttime==0) {
      DEBUG1(2,"computation failed, using best success\n");
      break;
      } else
      if ( firsttime==2) {
      DEBUG1(2,"computation failed, trying normal p-sequence\n");
      element_step=1;
      element=4+iRRAM_prec_array[element_step];
      firsttime=1;
      } else {
      DEBUG1(1,"computation of limit_gen1 failed totally\n");
      REITERATE(0);
      }}
   if ( firsttime != 0 || sizetype_less(limnew_error,lim_error) ) {
      lim=limnew;
      lim_error=limnew_error;
      DEBUG3(2,"getting result with error %d*2^(%d)\n",
               lim_error.mantissa, lim_error.exponent);
      } else {
      DEBUG1(2,"computation successful, but no improvement\n");
      }
    firsttime=0;
    if (element<=SAVED_STACK.data.actual_prec)break;
    element_step+=4;
    element=iRRAM_prec_array[element_step];
    }
  lim.seterror(lim_error);
  DEBUG0(2,{fprintf(stderr,"end of limit_mv with error %d*2^(%d)\n",
                   lim_error.mantissa,lim_error.exponent);});
  return lim;
}

template <class ARGUMENT, class DISCRETE, class RESULT>
RESULT  limit_lip (RESULT  f(int,const ARGUMENT&,DISCRETE param),
            int lip_value,
	    bool on_domain(const ARGUMENT&),
            const ARGUMENT& x,DISCRETE param)
{
  if ( on_domain(x) != true ) REITERATE(0);

  ITERATION_STACK SAVED_STACK;
  ACTUAL_STACK.inlimit+=1;
  ACTUAL_STACK.prec_step+=1;
  ACTUAL_STACK.actual_prec=iRRAM_prec_array[ACTUAL_STACK.prec_step];
  iRRAM_highlevel = (ACTUAL_STACK.prec_step > 1);

  ARGUMENT x_new;
  RESULT lim;
  sizetype lim_error,x_error;

  x_new=x;
  x_new.geterror(x_error);
  sizetype_exact(lim_error);
  x_new.seterror(lim_error);

  limit_debug("starting limit_lip");

  while (1) {
     try{
      DEBUG2(2,"trying to compute limit_lip with precision %d...\n",ACTUAL_STACK.actual_prec);
    lim=f(SAVED_STACK.data.actual_prec,x_new,param);
    lim.geterror(lim_error);
    if (lim_error.exponent > SAVED_STACK.data.actual_prec ) {
      ACTUAL_STACK.prec_step+=2;
      ACTUAL_STACK.actual_prec=iRRAM_prec_array[ACTUAL_STACK.prec_step];
      limit_debug2("limit_lip too imprecise");

   } else {
      DEBUG3(2,"getting result with local error %d*2^(%d)\n",
             lim_error.mantissa, lim_error.exponent);
      break;
    }}
    catch ( Iteration it){
      ACTUAL_STACK.prec_step+=2;
      ACTUAL_STACK.actual_prec=iRRAM_prec_array[ACTUAL_STACK.prec_step];
      limit_debug2("limit_lip failed");
    } }
  sizetype_set(lim_error,1,SAVED_STACK.data.actual_prec);
  lim.adderror(lim_error);
  sizetype_shift(lim_error,x_error,lip_value);
  lim.adderror(lim_error);
  DEBUG0(2,{lim.geterror(lim_error);
            fprintf(stderr,"end of limit_lip with error %d*2^(%d)\n",
              lim_error.mantissa,lim_error.exponent);
            fprintf(stderr,"  error of argument: %d*2^(%d)\n",
              x_error.mantissa,x_error.exponent);});
  return lim;
}




// template <class ARGUMENT, class PARAM, class RESULT>
// RESULT lipschitz (RESULT f(const PARAM& param,const ARGUMENT& ),
//             REAL lip_f(const PARAM& param, const ARGUMENT&),
// 	    bool on_domain(const PARAM& param, const ARGUMENT&),
//             const PARAM& param, const ARGUMENT& x)
// {
//   if ( on_domain(param,x) != true ) REITERATE(0);
// 
//   ITERATION_STACK SAVED_STACK;
// 
//   continous_begin ();
//   DEBUG1(2,"starting lipschitz1b ...\n");
// 
// // for the computation of the Lipschitz bound, we work with
// // reduced precision:
//   ACTUAL_STACK.prec_step=(ACTUAL_STACK.prec_step+1)/2;
//   ACTUAL_STACK.actual_prec=iRRAM_prec_array[ACTUAL_STACK.prec_step];
//   iRRAM_highlevel = (ACTUAL_STACK.prec_step > 1);
// 
//   lip_bound=lip_f(param,x);
// 
//   ACTUAL_STACK.prec_step=SAVED_STACK.prec_step;
//   ACTUAL_STACK.actual_prec=iRRAM_prec_array[ACTUAL_STACK.prec_step];
//   iRRAM_highlevel = (ACTUAL_STACK.prec_step > 1);
// 
//   ARGUMENT x_new;
//   RESULT lip_result,lip_bound;
//   sizetype lip_error,lip_size,tmp_size,x_error;
// 
//   bool try_it=true;
//   x_new=x;
//   x_new.geterror(x_error);
//   sizetype_exact(x_new.error);
// 
// 
//   while (try_it) {
//   try { try_it=false;
//         lip_result=f(param,x_new); }
//   catch ( Iteration it)  { try_it=true;
//       ACTUAL_STACK.prec_step+=2;
//       ACTUAL_STACK.actual_prec=iRRAM_prec_array[ACTUAL_STACK.prec_step];
//       iRRAM_highlevel = (ACTUAL_STACK.prec_step > 1);
//       DEBUG2(2,"limit_lip2 failed, increasing precision locally to %d...\n",ACTUAL_STACK.actual_prec);
//     }
//   }
//   continous_end ();
// 
//   DEBUG3(2,"getting result with local error %d*2^(%d)\n",
//              lip_result.error.mantissa, lip_result.error.exponent);
//   lip_size=lip_bound.vsize;
//   lip_bound.geterror(tmp_size);
//   sizetype_inc(lip_size,tmp_size);
//   sizetype_mult(lip_error,lip_size,x_error);
//   lip_result.adderror(lip_error);
//   DEBUG0(2,{lip_result.geterror(lip_error);
//             fprintf(stderr,"end of lipschitz_1b with error %d*2^(%d)\n",
//               lip_error.mantissa,lip_error.exponent);
//             fprintf(stderr,"  for argument with error %d*2^(%d)\n",
//               x_error.mantissa,x_error.exponent);});
//   return lip_result;
// }

template <class CONT_ARGUMENT, class DISCRETE_ARGUMENT, class RESULT, class PARAM>
RESULT lipschitz_1p_1a (RESULT f(const DISCRETE_ARGUMENT&, const PARAM& param),
            int lip,
            bool on_domain(const CONT_ARGUMENT&, const PARAM& param),
            const CONT_ARGUMENT& x, const PARAM& param )
{
  if ( on_domain(param,x) != true ) REITERATE(0);

  ITERATION_STACK SAVED_STACK;

  continous_begin ();
  DEBUG1(2,"starting lipschitz_1p_1a ...\n");

  DISCRETE_ARGUMENT x_center;
  RESULT lip_result;
  sizetype lip_error,lip_size,x_error;

  bool try_it=true;
  x.to_formal_ball(x_center,x_error);

  while (try_it) {
  try { try_it=false;
        lip_result=f(x_center,param); }
  catch ( Iteration it)  { try_it=true;
      ACTUAL_STACK.prec_step+=2;
      ACTUAL_STACK.actual_prec=iRRAM_prec_array[ACTUAL_STACK.prec_step];
      iRRAM_highlevel = (ACTUAL_STACK.prec_step > 1);
      DEBUG2(2,"lipschitz_1p_1a failed, increasing precision locally to %d...\n",ACTUAL_STACK.actual_prec);
    }
  }
  continous_end ();

  DEBUG3(2,"getting result with local error %d*2^(%d)\n",
             lip_result.error.mantissa, lip_result.error.exponent);
  sizetype_shift(lip_error,x_error,lip);
  lip_result.adderror(lip_error);
  DEBUG0(2,{lip_result.geterror(lip_error);
            fprintf(stderr,"end of lipschitz_1p_1a with error %d*2^(%d)\n",
              lip_error.mantissa,lip_error.exponent);
            fprintf(stderr,"  for argument with error %d*2^(%d)\n",
              x_error.mantissa,x_error.exponent);});
  return lip_result;
}


} /* ! namespace iRRAM */


#endif 
/* ! iRRAM_limit_templates_h */
