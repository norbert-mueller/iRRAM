#include "iRRAM.h"

using namespace iRRAM;

/* Compute iteration by J.M.Muller */

void compute(){
  int count;
  cout << "\nJMM-example: c=111-(1130-3000/a)/b\n";
  cout << "How many values: ";
  cin  >> count;
  cout << "\n";

  DYADIC_precision P(-100);
  float    af= float(11)/2,    bf=float(61)/11,    cf;
  double   ad= double(11)/2,   bd=double(61)/11,   cd;
  DYADIC   aD= DYADIC(11)/2,   bD=DYADIC(61)/11,   cD;
  RATIONAL aq= RATIONAL(11)/2, bq=RATIONAL(61)/11, cq;
  REAL     aR= REAL(11)/2,     bR=REAL(61)/11,     cR;
  for (int i=0;i<=count;i++ ) {
    cout << i << " " 
      << af << " " 
      << ad << " " 
      << REAL(aD).as_double() << " " 
      << REAL(aq).as_double() << " " 
      << aR.as_double() << "\n";
    cD=111-(1130-3000/aD)/bD;    aD=bD; bD=cD;   
    cR=111-(1130-3000/aR)/bR;    aR=bR; bR=cR;   
    cf=111-(1130-3000/af)/bf;    af=bf; bf=cf;   
    cd=111-(1130-3000/ad)/bd;    ad=bd; bd=cd;   
    cq=111-(1130-3000/aq)/bq;    aq=bq; bq=cq;   
  }
}
