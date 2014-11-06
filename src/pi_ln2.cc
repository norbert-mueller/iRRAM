#include <cstdio>
#include <cstdlib>

#include <iRRAM/core.h>

namespace iRRAM {

/*****************************************************************/
/*                     ln(2)=0.6931471805....                    */
/*   Usage: ln2()                                                */
/*   Application: range reduction, AGM methods for logarithms, ..*/ 
/*****************************************************************/

double ln2_time=0.0;
double pi_time=0.0;

REAL ln2_approx (int prec){
  stiff code;
  int N=100-prec/2;
  REAL a=1,b=scale(REAL(1),N),c,d;
  bool end;
  do {
    c=a*b;
    a=scale(c/(a+b),1);
    b=sqrt(c);
    d=a-b;
    end=bound(d,-2*N);
  } while ( !end );
  sizetype error;
  d.getsize(error);
  a.adderror(error);
  c= a/(2*N+4);
  return c;
}

REAL ln2(){

static __thread REAL *ln2_val;
static __thread int ln2_err=0;

   if (ln2_err==0) ln2_val=new(REAL);
   if (ln2_err >  ACTUAL_STACK.actual_prec || ln2_err == 0) {
     unsigned int dummy;double s1;resources(s1,dummy);
     REAL p=pi();
     REAL ln2a;
     {
       ITERATION_STACK SAVED_STACK;
       ACTUAL_STACK.prec_step+=1;
       ACTUAL_STACK.actual_prec=iRRAM_prec_array[ACTUAL_STACK.prec_step];
       iRRAM_highlevel = (ACTUAL_STACK.prec_step > 1);

       ln2a=limit(ln2_approx);
     ln2_time-=s1;resources(s1,dummy);ln2_time+=s1;
     delete ln2_val; ln2_val=new(REAL);
     (*ln2_val) = p*ln2a;
     sizetype error; ln2_val->geterror(error);
     ln2_err =  error.mantissa;
     ln2_err =  ACTUAL_STACK.actual_prec;
     }

   }
   return (*ln2_val);
}


/*****************************************************************/
/*                     pi = 3.141592653...                       */
/*   Usage: pi()                                                 */
/*   Application: range reduction, ...                           */ 
/*****************************************************************/


__thread REAL *pi_val;
__thread int pi_err=0;

REAL pi_approx_MACHIN (int prec){
   REAL z1=REAL(1)/REAL(5);
   REAL z2=REAL(1)/REAL(239);
   REAL y,z;
   int i;
  
   i=3;
   y=z1;
   z=y;
   while ( !bound(z,prec-5) ) {
     y=-y/(5*5);
     z=y/i;
     z1=z1+z;
     i+=2;
   }
   i=3;
   y=z2;
   z=y;
   while ( !bound(z,prec-3) ) {
     y=-y/(239*239);
     z=y/i;
     z2=z2+z;
     i+=2;
   }
   return 4*(4*z1-z2);
}

REAL pi_approx_AGM (int prec){
   REAL a= 1;
   REAL b= 1/sqrt(REAL(2));
   REAL t= 1/REAL(4);
   REAL pi_l,pi_u;
   int j=1;
   REAL y;
   do {
     y=a;
     a=(a+b)/2;
     b=sqrt(b*y);
     t=t-j*power(a-y,2);
     j=2*j; 
   } while ( ! bound(a-b, prec));
   return a*a/t;
}

REAL pi_inv_approx_BORWEIN (int prec){
   REAL a= 6-4*sqrt(REAL(2));
   REAL y= sqrt(REAL(2))-1;
   REAL s=2,t,aold;
   int k=1;
   do {
     aold=a;
     t=sqrt(sqrt(1-power(y,4)));
     y=(1-t)/(1+t);
     s=s*4;
     a=a*power(1+y,4)-s*y*(1+y+y*y);
     k=2*k; 
   } while (  !bound (a-aold,prec/4 )) ;
   return a;
}

REAL pi (){
   if (pi_err==0) pi_val=new(REAL);
   if (pi_err >  ACTUAL_STACK.actual_prec || pi_err == 0) {
//   pi_val = limit(pi_approx_MACHIN);
//   pi_val = limit(pi_approx_AGM);
     unsigned int dummy;double s1;resources(s1,dummy);
     ITERATION_STACK SAVED_STACK;
     ACTUAL_STACK.prec_step+=1;
     ACTUAL_STACK.actual_prec=iRRAM_prec_array[ACTUAL_STACK.prec_step];
     iRRAM_highlevel = (ACTUAL_STACK.prec_step > 1);

     delete pi_val; pi_val=new(REAL);
     (*pi_val) = 1/limit(pi_inv_approx_BORWEIN);
     pi_time-=s1;resources(s1,dummy);pi_time+=s1;

     sizetype error; pi_val->geterror(error);
     pi_err =  error.mantissa;
     pi_err =  ACTUAL_STACK.actual_prec;
   }
   return (*pi_val);
}

} // namespace iRRAM
