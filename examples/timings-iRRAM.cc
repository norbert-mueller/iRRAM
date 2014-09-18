#include "iRRAM.h"

using namespace iRRAM;

#include <sys/time.h>
#include <sys/resource.h>
#include <unistd.h>


double cputime(){
 struct rusage r;
 getrusage (RUSAGE_SELF,&r);
 return r.ru_utime.tv_sec+0.000001*r.ru_utime.tv_usec;
 }





void printval(string s,int DP, const REAL& y)
{
cout << s<<"\n" << setRwidth(DP+8) << y<< "\n\n";
}


void printval(string s,int DP, const DYADIC& y)
{
cout << s<<"\n" << setRwidth(DP+8) << y<< "\n\n";
}



void compute(){
DYADIC value[100];

int DP;
cout << "Length of mantissa: ";
cin  >> DP ;
cout << "\n";


REAL x;x=sqrt(REAL(3));
REAL y;y=sqrt(REAL(5));
REAL w=1/x;
REAL PI =pi();
long prec=-10*DP/3;
long min_fkt=0;
long max_fkt=12;

for (long fkt=min_fkt; fkt<=max_fkt; fkt++) {

if (fkt ==0)   value[1]=approx(x+y,prec);
if (fkt ==1)   value[1]=approx(x*y,prec);
if (fkt ==2)   value[1]=approx(x/y,prec);
if (fkt ==3)   value[1]=approx(sqrt(x),prec);
if (fkt ==4)   value[1]=approx(exp(x),prec);
if (fkt ==5)   value[1]=approx(log(x),prec);
if (fkt ==6)   value[1]=approx(sin(x),prec);
if (fkt ==7)   value[1]=approx(cos(x),prec);
if (fkt ==8)   value[1]=approx(tan(x),prec);
if (fkt ==9 )  value[1]=approx(atan(x),prec);
if (fkt ==10)  value[1]=approx(tanh(x),prec);
if (fkt ==11)  value[1]=approx(atanh(w),prec);
if (fkt ==12)  value[1]=approx(euler(),prec);
if (fkt ==13)  value[1]=approx(pi(),prec);
continous_begin();
double s1=cputime(),s2=s1;
long   count=1;
REAL z;
while (s2-s1 < 1 ) {
if (fkt ==0)   for (long i=1; i<=count;i++) z=x+y;
if (fkt ==1)   for (long i=1; i<=count;i++) z=x*y;
if (fkt ==2)   for (long i=1; i<=count;i++) z=x/y;
if (fkt ==3)   for (long i=1; i<=count;i++) z=sqrt(x);
if (fkt ==4)   for (long i=1; i<=count;i++) z=exp(x);
if (fkt ==5)   for (long i=1; i<=count;i++) z=log(x);
if (fkt ==6)   for (long i=1; i<=count;i++) z=sin(x);
if (fkt ==7)   for (long i=1; i<=count;i++) z=cos(x);
if (fkt ==8)   for (long i=1; i<=count;i++) z=tan(x);
if (fkt ==9 )  for (long i=1; i<=count;i++) z=atan(x);
if (fkt ==10)  for (long i=1; i<=count;i++) z=tanh(x);
if (fkt ==11)  for (long i=1; i<=count;i++) z=atanh(w);
if (fkt ==12)  for (long i=1; i<=count;i++) z=euler();
if (fkt ==13)  for (long i=1; i<=count;i++) z=pi();
s2=cputime();
count=2*count;
}
continous_end();
if (fkt ==0) cout << "x+y       took ";
if (fkt ==1) cout << "x*y       took ";
if (fkt ==2) cout << "x/y       took ";
if (fkt ==3) cout << "sqrt(x)   took ";
if (fkt ==4) cout << "exp(x)    took ";
if (fkt ==5) cout << "log(x)    took ";
if (fkt ==6) cout << "sin(x)    took ";
if (fkt ==7) cout << "cos(x)    took ";
if (fkt ==8) cout << "tan(x)    took ";
if (fkt ==9 ) cout << "atan(x)  took ";
if (fkt ==10) cout << "tanh(x)  took ";
if (fkt ==11) cout << "atanh(x) took ";
if (fkt ==12) cout << "e=2.17.. took ";
if (fkt ==13) cout << "pi=3.1.. took ";
cout <<1000*(s2-s1)/(count-1)<<" ms  ("
     <<count-1<<" tries in "
     << s2-s1 << " s)\n";
}


};


