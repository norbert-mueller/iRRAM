#include "iRRAM.h"

using namespace iRRAM;


void printval(std::string s,int DP, const REAL& y)
{
cout << s <<"\n" << y << "\n\n";
}


void printval(std::string s,int DP, const DYADIC& y)
{
cout << s <<"\n" << y << "\n\n";
}



void compute(){
DYADIC value[100];

int DP;
cout << "Length of mantissa: ";
cin  >> DP;
cout << "\n";

REAL x=2;
REAL y=REAL(1)/REAL(3);
REAL PI =pi();
int prec=-10*DP/3;

cout << "0...\n";
value[0]=approx(sqrt(pi()),prec);
cout << "1...\n";
value[1]=approx(log(pi()),prec);
cout << "2...\n";
value[2]=approx(exp(exp(exp(REAL(1)))),prec);
cout << "3...\n";
value[3]=approx(log(1+log(1+log(1+pi()))),prec);
cout << "4...\n";
value[4]=approx(log(1+log(1+log(1+exp(REAL(1))))),prec);
cout << "5...\n";
value[5]=approx(log(1+log(1+log(1+log(1+log(1+log(1+pi())))))),prec);
cout << "6...\n";
value[6]=approx(log(1+log(1+log(1+log(1+log(1+log(1+exp(REAL(1)))))))) ,prec);
cout << "7...\n";
//stiff_begin();
//stiff_begin();
value[7]=approx(exp(REAL(1000)) ,prec);
//stiff_end();
//stiff_end();

cout << "\n\nResults:\n" << setRwidth(DP+8);

printval("sqrt(PI)  = ", DP,value[0]);
printval("log(PI)  = ",DP,value[1]);
printval("exp(exp(exp(1)))  = ",DP,value[2]);
printval("log(1+log(1+log(1+pi)))  = ",DP,value[3]);
printval("log(1+log(1+log(1+exp(1))))  = ",DP,value[4] );
printval("log(1+log(1+log(1+log(1+log(1+log(1+pi))))))  = ",DP, value[5]);
printval("log(1+log(1+log(1+log(1+log(1+log(1+exp(1)))))))  = ",DP, value[6]);
printval("exp(1000)  = ",DP, value[7]);

}

