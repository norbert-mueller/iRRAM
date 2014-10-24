/* modified version of MPFRs timings.c */
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <gmp.h>
#include <mpfr.h>
#include <iRRAM/mpfr_extension.h>

void iRRAM_initialize(int,char**);

int main(int argc, char *argv[])
{
  int prec, n ; mpfr_t x, y, z, z2;

  iRRAM_initialize(argc,argv);
  
  if (argc != 2 && argc != 3) {
    fprintf(stderr, "Usage: timing digits \n"); exit(1);
  }
  n = atoi(argv[1]);
  prec = (int) ( n * log(10.0) / log(2.0) + 1.0 );
  printf("prec=%u\n", prec);
  mpfr_init2(x, prec); mpfr_init2(y, prec); mpfr_init2(z, prec); 
  mpfr_init2(z2, prec);
  mpfr_set_d(x, 3.0, GMP_RNDN); mpfr_sqrt(x, x, GMP_RNDN);
  mpfr_set_d(y, 5.0, GMP_RNDN); mpfr_sqrt(y, y, GMP_RNDN);

  mpfr_log(z, x, GMP_RNDN);


mpfr_out_str(stdout,10,0,x,GMP_RNDD);
printf("value 1 : ");
mpfr_ext_test(z, x, prec,GMP_RNDD);
printf("\nvalue 2 : ");mpfr_out_str(stdout,10,0,z,GMP_RNDD);
printf("\n");

  mpfr_clear(x); mpfr_clear(y); mpfr_clear(z);

  return 0;
}
