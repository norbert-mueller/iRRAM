#include "iRRAM/lib.h"
#include "iRRAM/core.h"
#include <string>
using namespace std;
using namespace iRRAM;


template int iRRAM_exec <string*,string*> (int (*) (string*,string*),string*, string*);

int iRRAM_compute(string* in_par,string *out_par) {

REAL x1=in_par[0].c_str();
REAL x2=in_par[1].c_str();
string x3;

out_par[0]=swrite(sin(x1),20);

out_par[1]=swrite(cos(x2),120);

return 0;
}

int main (int argc,char **argv)
{
iRRAM_initialize(argc,argv);
string in[2];
in[0]="1.2345";
in[1]="2.3456";

string out[2];

iRRAM_exec(iRRAM_compute,in,out);

printf("%s %s %s %s\n",in[0].c_str(),in[1].c_str(),out[0].c_str(),out[1].c_str());
}
