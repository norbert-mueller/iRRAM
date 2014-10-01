#include "iRRAM.h"

using namespace iRRAM;
using std::string;

bool error=false;

inline void chk1(const string& s1, const string& s2a, int n ){
  string s=swrite(REAL(s1),n,iRRAM_float_relative);
  if  ( s == s2a){ return; };
  cout << s1 << "  yields  "
       << s << "  instead of  " 
       << s2a <<"  -> ERROR!\n"; 
  error=true;
}

inline void chk2(const string& s1, const string& s2a, const string& s2b, int n ){
  string s=swrite(REAL(s1),n,iRRAM_float_relative);
  if  ( s == s2a){
     return;
  }
  if  ( s == s2b){
     cout << s1 
          <<  "  yields  "<< s 
          << "  instead of  " << s2a 
          <<"  (still OK for 0.65 ulp )\n"; 
     return;
  }
  cout << s1 
       << "  yields  " << s 
       << "  instead of  " << s2a 
       <<"  -> ERROR!\n"; 
  error=true;
}


void compute(){
/* Test-Routines for conversion from REAL to string 
 The results are checked whether they have 
 a precision of at most 0.65 ulp
 As an example: give one decimal of a value between 1 and 2:
 Values between 1 and 1.35 must return 1
 between 1.35 and 1.65, the value may be 1 or 2,
 and between 1.65 and 2 the value must be 2 
 In consequnece, the checks can sometimes give 
 different correct results!
*/

  chk1("1","+.100E+0001",3+8);
  chk1("-1","-.100E+0001",3+8);
  chk1("1E10","+.100E+0011",3+8);
  chk1("-1E10","-.100E+0011",3+8);

  chk1(".9999999999","+.100E+0001",3+8);
  chk1(".999999999","+.100E+0001",3+8);
  chk1(".99999999","+.100E+0001",3+8);
  chk1(".9999999","+.100E+0001",3+8);
  chk1(".999999","+.100E+0001",3+8);
  chk1(".99999","+.100E+0001",3+8);
  chk1(".9999","+.100E+0001",3+8);
  chk1(".999","+.999E+0000",3+8);


  chk1(".9999E9999","+.10E+10000",3+8);
  chk1(".999E-10000","+.100E-9999",3+8);
  chk1(".9999E99999","+.1E+100000",3+8);
// precision quite high, so it takes very long...
//  chk1(".999E-100000","+.10E-99999",3+8);

  chk1("1.23056","+.123E+0001",3+8);
  chk1("1.23156","+.123E+0001",3+8);
  chk1("1.23256","+.123E+0001",3+8);
  chk1("1.23356","+.123E+0001",3+8);
  chk2("1.23456","+.123E+0001","+.124E+0001",3+8);
  chk2("1.23556","+.124E+0001","+.123E+0001",3+8);
  chk1("1.23656","+.124E+0001",3+8);
  chk1("1.23756","+.124E+0001",3+8);
  chk1("1.23856","+.124E+0001",3+8);
  chk1("1.23956","+.124E+0001",3+8);

  chk2(".12351E1","+.124E+0001","+.123E+0001",3+8);
  chk2(".12349E1","+.123E+0001","+.124E+0001",3+8);

  chk2("1.2351","+.124E+0001","+.123E+0001",3+8);
  chk2("1.2349","+.123E+0001","+.124E+0001",3+8);
  chk2(".12351E1","+.124E+0001","+.123E+0001",3+8);
  chk2(".12349E1","+.123E+0001","+.124E+0001",3+8);
  chk2(".12351E2","+.124E+0002","+.123E+0002",3+8);
  chk2(".12349E2","+.123E+0002","+.124E+0002",3+8);
  chk2(".12351E3","+.124E+0003","+.123E+0003",3+8);
  chk2(".12349E3","+.123E+0003","+.124E+0003",3+8);
  chk2(".12351E4","+.124E+0004","+.123E+0004",3+8);
  chk2(".12349E4","+.123E+0004","+.124E+0004",3+8);

  chk2(".92351E1","+.924E+0001","+.923E+0001",3+8);
  chk2(".92349E1","+.923E+0001","+.924E+0001",3+8);
  chk2(".92351E2","+.924E+0002","+.923E+0002",3+8);
  chk2(".92349E2","+.923E+0002","+.924E+0002",3+8);
  chk2(".92351E3","+.924E+0003","+.923E+0003",3+8);
  chk2(".92349E3","+.923E+0003","+.924E+0003",3+8);
  chk2(".92351E4","+.924E+0004","+.923E+0004",3+8);
  chk2(".92349E4","+.923E+0004","+.924E+0004",3+8);


  chk1("1.236501","+.124E+0001",3+8);
  chk2("1.236499","+.124E+0001","+.123E+0001",3+8);
  chk2("1.234999","+.123E+0001","+.124E+0001",3+8);


  if (error) exit(1);

 cout << "test_string_conv:   passed\n";
}
