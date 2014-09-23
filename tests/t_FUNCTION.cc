#include "iRRAM.h"

using namespace iRRAM;

RATIONAL R_identity( const RATIONAL& x ){ return x; }
INTEGER I_identity ( const INTEGER& x ){ return x; }

INTEGER I_multiply (const std::pair<INTEGER,INTEGER>& x ){ return x.first*x.second; }


void error(int i){
cerr << i <<"\n";
exit(1);
}

void compute(){

FUNCTION<RATIONAL,RATIONAL> fR;
fR = from_algorithm(R_identity);

FUNCTION<INTEGER,INTEGER> fI,fJ;

fI = from_algorithm(I_identity);
fJ = from_algorithm(I_identity);

/*addition*/
fI = fI + fI;
if ( ! (fI(3) == 6)   )  error(1); 

/*subtraction*/
fJ = fI - fJ;
if ( ! (fJ(2) == 2)   )  error(2);

/*multiplication*/
fI = fI * fI;
if ( ! (fI(2) == 16)  )  error(3);

/*division*/
fI = fI / fJ;
if ( ! (fI(2) == 8)   )  error(4);

/*composition*/
fI = compose (fI,fI);
if ( ! (fI(3) == 48)  )  error(5);

/*juxtaposition*/
if ( ! (
	juxtaposition(
		fI,
		from_algorithm(I_identity)
	)(3) == std::make_pair<INTEGER,INTEGER>(48,3)
)   )  error(6);

/*projection to first*/
if ( ! (
	first(product(fI,fR)
	)(  std::make_pair<INTEGER,RATIONAL>(3,2) ) == 48
)   ) error(7);

/*projection to second*/
if ( ! (
	second(product(fR,fI))(std::make_pair<RATIONAL,INTEGER>(2,3)) == 48
)   ) error (8);

/*polynomial*/
{	std::vector<INTEGER> coeff(4);
	coeff[0]=1;coeff[1]=2;coeff[2]=4;coeff[3]=8;
	fI = polynomial(coeff);
 if ( ! (
	fI(3) == 1+2*3+4*(3*3)+8*(3*3*3)
)  ) error (9);
}

#if 0 /* TODO */
/*bind_first*/
if ( ! (
	bind_first(addition(product(fI,fJ)),INTEGER(3))(2) == 1+2*3+4*(3*3)+8*(3*3*3) + 2
)  ) error (10);

/*bind_second*/
if ( ! (
	bind_second(addition(product(fI,fJ)),INTEGER(3))(2) == 1+2*2+4*(2*2)+8*(2*2*2) + 3
) ) error (11);
// #else
using INT = INTEGER;
using pairII = std::pair<INT,INT>;
FUNCTION<pairII,INT> add0 = addition(product(fI,fJ));

std::function<INT(INT,INT)> add = std::bind(
	static_cast<FUNCTIONAL_algorithm<INT,pairII>&>(*add0._fobject)._eval,
	std::bind(
		std::function<pairII(INT,INT)>(std::make_pair<INT,INT>),
		std::placeholders::_1,
		std::placeholders::_2));

if (!(std::bind(add, INTEGER(3), std::placeholders::_1)(2) == 1+2*3+4*(3*3)+8*(3*3*3) + 2))
	error(10);
if (!(std::bind(add, std::placeholders::_1, INTEGER(3))(2) == 1+2*2+4*(2*2)+8*(2*2*2) + 3))
	error(11);

cout << "test_FUNCTION:   (basic tests for functional DAGs)    passed\n";

#endif

cout << "test_FUNCTION:   (not yet fully implemented)    passed\n";

}
