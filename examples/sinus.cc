#include <cstdio>
#include <cstdlib>
#include <cmath>
#include "iRRAM.h"
#include "iRRAM/core.h"

using namespace iRRAM;

// tests of alternative implemenations for sine and cosine

REAL one_minus_cos_approx (int prec, int wd, const REAL& x){
precision_policy(iRRAM_RELATIVE);
  REAL x2=square(x);
  std::vector<REAL> xpow(wd);
  REAL xf=power(x2,wd),z;
  xpow[0]=-x2/2;
  z=xpow[0];
  for (int i=1; i<wd; i++) {
     xpow[i]=xpow[i-1]*x2/(-(2*i+1)*(2*i+2));
     z+=xpow[i];
  }
  REAL xp=1,e=z;
  INTEGER f=1;
  int i=1;
  while ( upperbound(xpow[wd-1])+upperbound(xp)> prec || bool(! bound(e,-1)) ) {
    xp*=xf; e=0;
    for (int j=0; j<wd; j++) {
      f=1;
      for (int k=i;k<i+wd;k++) f*=(-(2*k+2)*(2*k+1));
      xpow[j]=xpow[j]/REAL(f);
      i=i+1;
      e+=xpow[j];
    }
    e*=xp;     z+=e;
  }
precision_policy(iRRAM_ABSOLUTE);
  return z;
}

REAL cos_new_2 (int prec,const REAL& x){
  int it;
  int wd;
  if ( prec > -100 ) {it =10; wd= 1;}
  else if ( prec > -400 ) {it =15; wd= 2;}
  else if (prec > -800 ) {it=20; wd =2;}
  else if (prec > -1600 ) {it=24; wd =2;}
  else if (prec > -3200 ) {it=40; wd =3;}
  else if (prec > -6400 ) {it=20; wd =4;}
  else if (prec > -12000 ) {it=60; wd =5;}
  else if (prec > -24000 ) {it=80; wd =8;}
  else if (prec > -48000 ) {it=100; wd =12;}
  else if (prec > -100000 ) {it=120; wd =18;}
  else if (prec > -200000 ) {it=140; wd =26;}
  else {
     it=(int)(log(double(-prec))*8+6);
     wd=int(std::sqrt(double(-prec)/18));
  }  
  REAL y=x;

precision_policy(iRRAM_RELATIVE);
  y=scale(y,-it);

  y=one_minus_cos_approx(prec,wd,y);
precision_policy(iRRAM_RELATIVE);

  for (int i=1;i<=it;i+=1) y=scale(y,2)+scale(square(y),1);
precision_policy(iRRAM_ABSOLUTE);

  return 1+y;
}


REAL cos_new_1 (int prec,const REAL& x){
  if (prec >= 0 ) return 0;
  REAL m=modulo(x,2*pi());
  return cos_new_2(prec,m-2*pi());
}



REAL trig_norm(int& position,const REAL& x){
  REAL m=modulo(x,2*pi());
  int i=round(m*100);
  if ( i <  0 ) {i=i+628;m=m+2*pi();}
  if ( i> 550 ) {
    position=1;
    return m-2*pi(); 
  } else if ( i> 392) {
    position=1;return m-2*pi(); 
  } else if ( i> 235) {
    position=1;return m-2*pi(); 
  } else if ( i> 80) {
    position=1;return m-2*pi();
  } else {
    position=1;return m-2*pi(); 
  }
}

//------------------------------------------------------------------------------------
REAL cos_new_alternative_2 (int prec,const REAL& x){
  int it;
  int wd;
  if ( prec > -100 ) {it =8; wd= 1;}
  else if ( prec > -400 ) {it =16; wd= 2;}
  else if (prec > -800 ) {it=20; wd =2;}
  else if (prec > -1600 ) {it=24; wd =2;}
  else if (prec > -3200 ) {it=40; wd =3;}
  else if (prec > -6400 ) {it=50; wd =3;}
  else if (prec > -12000 ) {it=60; wd =5;}
  else if (prec > -24000 ) {it=80; wd =8;}
  else if (prec > -48000 ) {it=100; wd =12;}
  else if (prec > -100000 ) {it=120; wd =18;}
  else if (prec > -200000 ) {it=140; wd =26;}
  else {
     it=(int)(log(double(-prec))*8+6);
     wd=int(std::sqrt(double(-prec)/18));
  }  
  REAL y=x;

precision_policy(iRRAM_RELATIVE);
  y=scale(y,-it);

  y=one_minus_cos_approx(prec,wd,y)+1;
precision_policy(iRRAM_RELATIVE);

  for (int i=1;i<=it;i+=1) y=scale(square(y),1)-1;
precision_policy(iRRAM_ABSOLUTE);

  return y;
}

REAL cos_new_alternative (int prec,const REAL& x){
  if (prec >= 0 ) return 0;
  REAL m=modulo(x,2*pi());
  return cos_new_alternative_2(prec,m-2*pi());
}

bool total_domain(const REAL& x){ return true;}

REAL cos_new2 (const REAL& x){ return  limit_lip(cos_new_alternative,0,total_domain,x);}

//------------------------------------------------------------------------------------

REAL cos_new (const REAL& x){ return  limit_lip(cos_new_1,0,total_domain,x);}

REAL sin_new (const REAL& x){ return  cos_new(x-pi()/2);}

void sincos_new (REAL& s,REAL& c,const REAL& x){ 
  REAL arg=x;
  s=sin_new(arg);
  c=cos_new(arg);
}


/*
void compute(){


  int deci_places;
  int n;
  rprintf("Desired Decimals: ");  rscanf("%d",&deci_places);

  do {
    rprintf("\n cos(0.5)=");
    rwrite(cos_new(REAL(0.5)),deci_places+8);
    rprintf("\n cos2(0.5)=");
    rwrite(cos_new2(REAL(0.5)),deci_places+8);
    rprintf("\n sin(0.5)=");
    rwrite(sin_new(REAL(0.5)),deci_places+8);
    rprintf("\n\nAnother try? ");
    rscanf("%d",&deci_places);
  } while ( deci_places > 0 );
};

*/
#include <sys/time.h>
#include <sys/resource.h>
#include <unistd.h>


double cputime(){
 struct rusage r;
 getrusage (RUSAGE_SELF,&r);
 return r.ru_utime.tv_sec+0.000001*r.ru_utime.tv_usec;
 }




void printval(std::string s,int DP, const REAL& y)
{
cout << s <<"\n" <<setRwidth(DP+8)<<y<<"\n\n";
}


void printval(std::string s,int DP, const DYADIC& y)
{
cout << s <<"\n" <<setRwidth(DP+8)<<y<<"\n\n";
}



void compute(){
DYADIC value[100];

int DP;
cout << "Length of mantissa: ";
cin  >> DP;
cout <<"\n";


REAL x=sqrt(REAL(3));
REAL y=sqrt(REAL(5));
REAL PI =pi();
int prec=-10*DP/3;
int min_fkt=1;
int max_fkt=10;

for (int fkt=min_fkt; fkt<=max_fkt; fkt++) {
if (fkt ==1)  value[1]=approx(x*y,prec);
if (fkt ==2)  value[1]=approx(x/y,prec);
if (fkt ==3)  value[1]=approx(sqrt(x),prec);
if (fkt ==4)  value[1]=approx(exp(x),prec);
if (fkt ==5)  value[1]=approx(log(x),prec);
if (fkt ==6)  value[1]=approx(sin(x),prec);
if (fkt ==7)  value[1]=approx(cos(x),prec);
if (fkt ==8)  value[1]=approx(sin_new(x),prec);
if (fkt ==9)  value[1]=approx(cos_new(x),prec);
if (fkt ==10)  value[1]=approx(cos_new2(x),prec);
}

for (int fkt=min_fkt; fkt<=max_fkt; fkt++) {

double s1=cputime(),s2=s1;
int   count=1;
{
  single_valued code;
  while (s2-s1 < 1 ) {
  if (fkt ==1) for (int i=1; i<=count;i++) value[1]=approx(x*y,prec);
  if (fkt ==2) for (int i=1; i<=count;i++) value[1]=approx(x/y,prec);
  if (fkt ==3) for (int i=1; i<=count;i++) value[1]=approx(sqrt(x),prec);
  if (fkt ==4) for (int i=1; i<=count;i++) value[1]=approx(exp(x),prec);
  if (fkt ==5) for (int i=1; i<=count;i++) value[1]=approx(log(x),prec);
  if (fkt ==6) for (int i=1; i<=count;i++) value[1]=approx(sin(x),prec);
  if (fkt ==7) for (int i=1; i<=count;i++) value[1]=approx(cos(x),prec);
  if (fkt ==8) for (int i=1; i<=count;i++) value[1]=approx(sin_new(x),prec);
  if (fkt ==9) for (int i=1; i<=count;i++) value[1]=approx(cos_new(x),prec);
  if (fkt ==10) for (int i=1; i<=count;i++) value[1]=approx(cos_new2(x),prec);
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
if (fkt ==8) cout << "sin_n(x)took ";
if (fkt ==9) cout << "cos_n(x)took ";
if (fkt ==10) cout << "cos_n2(x)took ";
cout <<1000*(s2-s1)/(count-1)<<" ms  ("
     <<count-1<<" tries in "
     << s2-s1 << " s)\n";
}

}

