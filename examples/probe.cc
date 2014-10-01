#include "iRRAM.h"

#include <sys/time.h>
#include <sys/resource.h>
#include <unistd.h>

using namespace iRRAM;

using std::string;

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


REAL x=sqrt(REAL(3));
REAL y=sqrt(REAL(5));
REAL PI =pi();
long prec=-10*DP/3;
long min_fkt=1;
long max_fkt=7;

for (long fkt=min_fkt; fkt<=max_fkt; fkt++) {
if (fkt ==1)  value[1]=approx(x*y,prec);
if (fkt ==2)  value[1]=approx(x/y,prec);
if (fkt ==3)  value[1]=approx(sqrt(x),prec);
if (fkt ==4)  value[1]=approx(exp(x),prec);
if (fkt ==5)  value[1]=approx(log(x),prec);
if (fkt ==6)  value[1]=approx(sin(x),prec);
if (fkt ==7)  value[1]=approx(cos(x),prec);
}

for (long fkt=min_fkt; fkt<=max_fkt; fkt++) {

double s1=cputime(),s2=s1;
long   count=1;
{ 
  single_valued code;
  while (s2-s1 < 1 ) {
  if (fkt ==1) for (long i=1; i<=count;i++) value[1]=approx(x*y,prec);
  if (fkt ==2) for (long i=1; i<=count;i++) value[1]=approx(x/y,prec);
  if (fkt ==3) for (long i=1; i<=count;i++) value[1]=approx(sqrt(x),prec);
  if (fkt ==4) for (long i=1; i<=count;i++) value[1]=approx(exp(x),prec);
  if (fkt ==5) for (long i=1; i<=count;i++) value[1]=approx(log(x),prec);
  if (fkt ==6) for (long i=1; i<=count;i++) value[1]=approx(sin(x),prec);
  if (fkt ==7) for (long i=1; i<=count;i++) value[1]=approx(cos(x),prec);
  s2=cputime();
  count=2*count;
  }
}
if (fkt ==1) cout << "x*y     took ";
if (fkt ==2) cout << "x/y     took ";
if (fkt ==3) cout << "sqrt(x) took ";
if (fkt ==4) cout << "exp(x)  took ";
if (fkt ==5) cout << "log(x)  took ";
if (fkt ==6) cout << "sin(x)  took ";
if (fkt ==7) cout << "cos(x)  took ";
cout <<1000*(s2-s1)/(count-1)<<" ms  ("
     <<count-1<<" tries in "
     << s2-s1 << " s)\n";
}


}

