#include <cstdio>
#include <cstdlib>

#include <iRRAM/core.h>

namespace iRRAM {

REAL strtoREAL(const char* s, char** endptr){
  stiff code;
  int exp=0;
  int sign=1;
  REAL y;
  REAL ten=10;
  *endptr=const_cast<char*> (s);

  while ( **endptr == '0' ) *endptr+=1;

  if ( **endptr == '-' ) {sign=-1;*endptr+=1;}
  else if ( **endptr == '+' ) {sign=1;*endptr+=1;}

  while ( **endptr >= '0' &&  **endptr <= '9' ) {
  y=ten*y + REAL(sign*(**endptr-'0')); *endptr+=1;}

  if ( **endptr == '.' ) *endptr+=1;

  while ( **endptr >= '0' &&  **endptr <= '9' ) {
  y=ten*y + REAL(sign*(**endptr-'0')); exp-=1;*endptr+=1;}

  if ( **endptr == 'E' || **endptr == 'e'  ) {
  *endptr+=1;exp+=strtol(*endptr,endptr,10); }

  if (exp !=0) y*=power(ten,exp);

  return y;
}

REAL atoREAL(const char* s){
  char*dummy;
  return strtoREAL(s,&dummy);
}


REAL modulo (const REAL& x, const REAL& y){
   return x-round2(x/y)*y;
}

REAL power(const REAL& x, const REAL& y) {
  return exp(log(x)*y);
}


REAL power(const REAL& x, int n) {
   if (n==0) return 1;
   if (n==1) return x;
   if (n==2) return square(x);
//   stiff_begin();
   REAL y=1;
   REAL xc=x;
   if (n<0) {xc=y/x;n=-n;}
   if (n==1) {
//     stiff_end();
     return xc;
   }
   for (int k=n;k>0;k=k/2) { 
     if (k%2==1) y*=xc;
     if ( k ==1) break;
     xc=square(xc);}
//   stiff_end();
   return y; 
}

// maximum without using of internal representation of LAZY_BOOLEAN
// REAL maximum (const REAL& x, const REAL& y){
//    single_valued code;
//    LAZY_BOOLEAN larger = ( x > y );
//    switch ( choose ( larger, !larger, TRUE ) ){
//    case 1: return x;
//    case 2: return y;
//    case 3: return (x+y+abs(x-y))/2;
//    }
// };

// maximum, using of internal representation of LAZY_BOOLEAN
REAL maximum (const REAL& x, const REAL& y){
   LAZY_BOOLEAN larger;
   {
     single_valued code;
     larger = ( x > y );
   }
   if ( larger.value == true  ) return x;
   if ( larger.value == false ) return y;
   return (x+y+abs(x-y))/2;
}

// minimum, using of internal representation of LAZY_BOOLEAN
REAL minimum (const REAL& x, const REAL& y){
   LAZY_BOOLEAN larger;
   {
     single_valued code;
     larger = ( x > y );
   }
   if ( larger.value  == true  ) return y;
   if ( larger.value  == false ) return x;
   return (x+y-abs(x-y))/2;
}





/*****************************************************************/
/*                     matrix arithmetic                         */
/*****************************************************************/




REALMATRIX expmatrix_approx (int prec, const REALMATRIX& x){
   if (rows(x)!=columns(x)) {
     fprintf(stderr,"Exponentiation of non-quadratic real matrix [%d,%d]\n",
           columns(x),rows(x));
     exit(1);
     }
   if ( prec >= 2 ) return eye(rows(x));
   REALMATRIX y=x,z=y+eye(rows(x));
   int i=2;
   while ( !bound(y,prec) ) {
     y=y*x/i;
     z=z+y;
     i+=1;
   }
   return z;
}

bool exp_domain(const REALMATRIX& x){
  return bound(x,-2);
}

REALMATRIX exp (const REALMATRIX& x){
  single_valued code;
  REALMATRIX y;
  if ( bound(x,-2) )
  y = limit_lip(expmatrix_approx,0,exp_domain,x);
  else
    {
    y= exp(x/2);
    y= y*y;
    }
  return y;
}

REALMATRIX  steady_state (const REALMATRIX& x) {
  single_valued code;
  REALMATRIX result(1,rows(x));
  REALMATRIX chain=x;
  REAL factor,outrate;
  if ( rows(x) != columns(x)) { 
     fprintf(stderr,"Argument is not a square matrix\n");
     exit(1);
     }
  for (unsigned int N=0; N < rows(x); N++) chain(N,N)=chain(N,N)-1;

  for (unsigned int N=0; N < rows(x)-1; N++) {
    for (unsigned int i=N+1; i < columns(x); i++) {
      factor=chain(i,N)/chain(N,N);
      for (unsigned int j=N+1; j< columns(x); j++) 
          chain(i,j)=chain(i,j)
                   -factor*chain(N,j);
  } }

  result(0,rows(x)-1)=1;
  for (int N=rows(x)-2; N >= 0; N--) {
   outrate=0;
   for (int j=rows(x)-1; j>N;j--) outrate=outrate
                 +result(0,j)*chain(j,N);
   result(0,N)=outrate/(outrate-chain(N,N));
   factor=-chain(N,N)/(outrate-chain(N,N));
   for (int j=rows(x)-1; j>N;j--)
      result(0,j)=result(0,j)*factor;
  } 
  return result;
}


REALMATRIX  steady_state (const SPARSEREALMATRIX& x) {
  iRRAM::cout <<"Part 1...\n";
  REALMATRIX result(1,rows(x));
  SPARSEREALMATRIX chain=x;
  REAL factor,outrate;
  SPM_ELEMENT* a;
  SPM_ELEMENT* b;
  if ( rows(x) != columns(x)) { 
     fprintf(stderr,"Argument is not a square matrix\n");
     exit(1);
     }
  for (unsigned int N=0; N < rows(x); N++)
    sparse_set(chain,N,N,chain(N,N)-1);

  iRRAM::cout << "\nPart 2, filled: "<< chain.filled <<"\n";

  for (unsigned int N=0; N < chain.maxrow-1; N++) {
    a=chain.colvector[N];
    while ( a != NULL ) {
      if (a->rowindex >N) { 
        factor=a->value/chain(N,N);
        b=chain.rowvector[N];
        while ( b != NULL ) {
          if (b->colindex >N) 
           sparse_set(chain,a->rowindex,b->colindex,
              chain(a->rowindex,b->colindex)-factor* b->value );
          b=b->nextcol;
        }
      }
      a=a->nextrow;
  } }

  iRRAM::cout << "\nPart 3, filled: "<< chain.filled <<"\n";
  result(0,rows(x)-1)=1;
  for (int N=rows(x)-2; N >= 0; N--) {
    outrate=0;
    a=chain.colvector[N];
    while ( a!= NULL  ) {
     if (a->rowindex > (unsigned int) N) outrate=outrate+ result(0,a->rowindex)* a->value;
     a=a->nextrow;
   }
   result(0,N)=outrate/(outrate-chain(N,N));
   factor=-chain(N,N)/(outrate-chain(N,N));
   for (int j=rows(x)-1; j>N;j--)
     result(0,j)=result(0,j)*factor;
  } 
  return result;
}

REALMATRIX  equilib (const SPARSEREALMATRIX& x) {
  SPARSEREALMATRIX chain=x;
  REALMATRIX result;
  equilib_del(chain,result);
  return result;
}

void equilib_del (SPARSEREALMATRIX& chain,REALMATRIX& result) {
  iRRAM::cout <<"Part 1...\n";

  REAL outrate,factor,divisor;
  SPM_ELEMENT* a;
  SPM_ELEMENT* b;
  SPM_ELEMENT* c;

  result=zeroes(1,1);

  for (unsigned int N=0; N < chain.maxrow; N++) {
    outrate=0;
    a=chain.rowvector[N];
    while ( a != NULL ) {
      outrate=outrate+a->value;
      a=a->nextcol;
    }
    sparse_set(chain,N,N,chain(N,N)-outrate);
  }
  for (int N=chain.maxrow-1; N >= 0; N--) {
    sparse_set(chain,N,chain.maxrow-1,1);
  }

  iRRAM::cout << "\nPart 2, filled: "<< chain.filled <<"\n";

  for (unsigned int i=0; i < chain.maxrow; i++) {
    c=sparse_get_ptr(chain,i,i);
    divisor=c->value;
    a=c->nextcol;
    while ( a != NULL ) {
      factor=a->value/divisor;
      b=c->nextrow;
      chain.hotspot=b;
      while ( b != NULL ) {
        sparse_set(chain,b->rowindex,a->colindex,
                    chain(b->rowindex,a->colindex)
                   -factor* b->value);
        b=b->nextrow;
      }
      a=a->nextcol;
    }
  }

iRRAM::cout << "\nPart 3, filled: "<< chain.filled <<"\n";

  result=zeroes(1,chain.maxrow);
  result(0,chain.maxrow-1)=1;
  for (int i=chain.maxrow-1; i >= 0; i--) {
    result(0,i)=result(0,i)/chain(i,i);
    a=chain.rowvector[i];
    while ( a != NULL && a->colindex< (unsigned int) i) {
      result(0,a->colindex)=result(0,a->colindex) - a->value * result(0,i);
      a=a->nextcol;
    }
  }

  iRRAM::cout <<"Part 4...\n";
}




//********************************************************************************
// Absolute value of vector in Euclidean space
//********************************************************************************

REAL abs(const std::vector<REAL>& x)
{
	unsigned int n=x.size();
	REAL sqrsum=0;
	for (unsigned i=0;i<n;i++) {
		sqrsum += square(x[i]);
	}
	return sqrt(sqrsum);
}



} // namespace iRRAM
