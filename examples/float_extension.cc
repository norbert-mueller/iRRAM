#include <cmath>


#include <sys/time.h>
#include <sys/resource.h>
#include <unistd.h>

#include "iRRAM/lib.h"

using namespace iRRAM;

double cputime(){
 struct rusage r;
 getrusage (RUSAGE_SELF,&r);
 return r.ru_utime.tv_sec+0.000001*r.ru_utime.tv_usec;
 }



double iRRAM_function(const double& x) {
  REAL r; 
  r=x;
  for (int i=1;i<100000;i++) r=r+REAL(1)/sqrt(REAL(i));
  return r.as_double(23);
}

// main routine that internally calls iRRAM:

int main (int argc,char **argv)
{
iRRAM_initialize(argc,argv);

double d=2,d1=0,d2=0;

double s1,s2;
int   count;

s1=cputime();s2=s1;count=1;
while (s2-s1 < 1 ) {
   for (int i=0;i<count;i++) {d1=d;for (int i=1;i<100000;i++) d1=d1+1.0/std::sqrt(double(i));}
   s2=cputime();
   count=2*count;
}
printf ("double: %f ms  (%d tries in %f s)\n",1000*(s2-s1)/(count-1),count-1,s2-s1);

s1=cputime();s2=s1;count=1;
while (s2-s1 < 1 ) {
   for (int i=0;i<count;i++) d2=iRRAM_exec(iRRAM_function,d);
   s2=cputime();
   count=2*count;
}
printf ("iRRAM: %f ms  (%d tries in %f s)\n",1000*(s2-s1)/(count-1),count-1,s2-s1);


printf ("%f %e %e\n",d2,d2-d1,(d2-d1)/d2);

}
