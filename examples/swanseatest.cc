#include "iRRAM.h"

using namespace iRRAM;


void printval(string s,int DP, const REAL& y)
{
cout << s<<"\n" << setRwidth(DP+8) << y<< "\n\n";
}



void compute(){

int DP;
cout << "Length of mantissa: ";
cin  >> DP ;
cout << "\n";

REAL x=REAL(15)/REAL(7);
REAL y=REAL(1)/REAL(3);

printval("x          = "          ,DP,x);
printval("y          = "          ,DP,y);
printval("abs(-pi)   = "          ,DP,abs(-pi()));
printval("max(pi,e)  = "          ,DP,maximum(pi(),euler()));
printval("min(pi,e)  = "          ,DP,minimum(pi(),euler()));
printval("sqrt(x)  = "            ,DP,sqrt(x));
printval("tan(atan(1/pi))*pi  = " ,DP,pi()*tan(atan(1/pi())));
printval("sin(x)  = "             ,DP,sin(x));
printval("asin(sin(x))  = "       ,DP,asin(sin(x)));
printval("acos(cos(x))  = "       ,DP,acos(cos(x)));
printval("sin(asin(sin(x)))  = "  ,DP,sin(asin(sin(x))));
printval("sqrt(0)  = "            ,DP,sqrt(REAL(0)));
printval("exp(x)  = "             ,DP,exp(x));
printval("pi     = "              ,DP,pi());
printval("atan(1/pi)  = "         ,DP,atan(1/pi()));
printval("tan(atan(1/pi))*pi  = " ,DP,pi()*tan(atan(1/pi())));

printval("log(x)  = "             ,DP,log(x));
printval("exp(log(x))  = "        ,DP,exp(log(x)));
printval("log(exp(x))  = "        ,DP,log(exp(x)));
printval("sin(x)  = "             ,DP,sin(x));
printval("cos(x)  = "             ,DP,cos(x));
printval("tan(x)  = "             ,DP,tan(x));
printval("sec(x)  = "             ,DP,sec(x));
printval("cosec(x)  = "           ,DP,cosec(x));
printval("cotan(x)  = "           ,DP,cotan(x));
printval("sinh(x)  = "            ,DP,sinh(x));
printval("cosh(x)  = "            ,DP,cosh(x));
printval("tanh(x)  = "            ,DP,tanh(x));
printval("sech(x)  = "            ,DP,sech(x));
printval("cosech(x)  = "          ,DP,cosech(x));
printval("coth(x)  = "            ,DP,coth(x));
printval("asinh(x)  = "           ,DP,asinh(x));
printval("acosh(x)  = "           ,DP,acosh(x));
printval("atanh(y)  = "           ,DP,atanh(y));
printval("acoth(y)  = "           ,DP,acoth(y));
printval("asech(y)  = "           ,DP,asech(y));
printval("acosech(y)  = "         ,DP,acosech(y));


printval("sqrt(pi)  = "           ,DP,sqrt(pi()));
printval("log(pi)  = "            ,DP,log(pi()));
printval("sin(exp(1))  = "        ,DP,sin(exp(REAL(1))));
printval("cos(exp(1))  = "        ,DP,cos(exp(REAL(1))));
printval("sin(sin(sin(1))) = "    ,DP,sin(sin(sin(REAL(1)))));
printval("cos(cos(cos(1))) = "    ,DP,cos(cos(cos(REAL(1)))));
printval("exp(exp(exp(1))) = "    ,DP,exp(exp(exp(REAL(1)))));
printval("log(1+log(1+log(1+pi)))  = "
                                  ,DP,log(1+log(1+log(1+pi()))) );
printval("log(1+log(1+log(1+exp(1))))  = "
                                  ,DP,log(1+log(1+log(1+exp(REAL(1))))) );
printval("log(1+log(1+log(1log(1+log(1+log(1+pi))))))  = "
                                  ,DP, log(1+log(1+log(1+log(1+log(1+log(1+pi())))))) );
printval("log(1+log(1+log(1log(1+log(1+log(1+exp(1)))))))  = "
                                  ,DP,			 
log(1+log(1+log(1+log(1+log(1+log(1+exp(REAL(1)))))))) );


printval("sin(1e50)  = "          ,DP,sin("1e50"));
printval("cos(1e50)  = "          ,DP,cos("1e50"));

printval("exp(1000)  = "          ,DP,exp(REAL(1000)));

};

