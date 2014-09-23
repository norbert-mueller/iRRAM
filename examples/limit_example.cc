#include "iRRAM.h"

using namespace iRRAM;


/* Examples for Limits */
bool whole_domain(const REAL& x, const REAL& y){
return true;
}

REAL max_approx (int prec, const REAL& x, const REAL& y){
   if ( positive(x-y,prec) ) return x;
   else return y;
}

REAL maximum_as_a_limit (const REAL& x, const REAL& y){
   return limit_lip(max_approx,0,whole_domain,x,y);
}

REAL min_approx (int prec, const REAL& x, const REAL& y){
   if ( positive(x-y,prec) ) return y;
   else return x;
}

REAL minimum_as_a_limit (const REAL& x, const REAL& y){
   return limit_lip(min_approx,0,whole_domain,x,y);
}


void compute(){

REAL x1,x2,mi,ma;
x1="1e-100000";
x2="-1e-200";

//ma=maximum_as_a_limit(x1,x2);
//mi=minimum_as_a_limit(x1,x2);
ma=maximum(x1,x2);
mi=minimum(x1,x2);
switch (choose(x1<ma,x2<ma) ){
     case 1:
      cout << "Minimum: x1\n";     
     break;
     case 2:
      cout << "Minimum: x2\n";
     break;
}
switch (choose(x1>mi,x2>mi) ){
     case 1:
      cout << "Maximum: x1\n";     
     break;
     case 2:
      cout << "Maximum: x2\n";
     break;
}

}
