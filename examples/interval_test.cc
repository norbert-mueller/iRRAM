#include "iRRAM.h"

using namespace iRRAM;

void printinterval(const INTERVAL& x){
 cout << setRwidth(20)<<"( "<<inf(x)<<"  ,  "<<sup(x)<<" )\n";
}

void compute(){

	REAL a(1);
	REAL b(2);

	INTERVAL x(a,b);
	INTERVAL y(4,3);
	INTERVAL z;

	cout << "Copy Constructor: \n";
	z=x;
	printinterval(z);

	cout << "Additions: \n";
	printinterval(x+y);
	printinterval(x+a);
	printinterval(a+b);

	cout << "Subtractions: \n";
	printinterval(x-y);
	printinterval(x-a);
	printinterval(a-x);
	printinterval(a-b);
	printinterval(y);
	printinterval(REAL(0)-y);


/*	cout << "Multiplications: \n";
	printinterval(x*y);
	printinterval(x*(REAL(0)-y));
	printinterval(x*INTERVAL(REAL(-1),REAL(1)));
	printinterval(a*b);
*/
	cout << "Divisions: \n";
	printinterval(x/y);
	printinterval(x/a);
	printinterval(a/x);
	printinterval(a/b);
	
        cout << "\n\n";
        cout << "For the interval  x= \n";
        printinterval(x);
        cout << "we have e^x...\n";
        printinterval(exp(x));
        cout << "ln(x)...\n";
        printinterval(log(x));
        cout << "log(e^x)...\n";
        printinterval(log(exp(x)));
        cout << "and especially x^y, where x=y)...\n";
        printinterval(exp(x*log(x)));
        cout << "furthermore sin(x)...\n";
        printinterval(sin(x));
        cout << "and cos(x)...\n";
        printinterval(cos(x));
        cout << "mag(x)...\n";
        cout <<mag(x)  << "\n";
        cout << "and mig(x)...\n";
        cout <<mig(x) << "\n";
        cout << "and mid(x)...\n";
        cout <<mid(x) << "\n";

}
