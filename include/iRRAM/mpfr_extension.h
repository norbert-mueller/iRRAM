#ifndef GMP_RNDN 
#include "mpfr.h" 
#endif

#ifdef __cplusplus
extern "C" {
#endif

void mpfr_ext_exp (mpfr_ptr r, mpfr_srcptr u,int p);
void mpfr_ext_log (mpfr_ptr r, mpfr_srcptr u,int p);
void mpfr_ext_sin (mpfr_ptr r, mpfr_srcptr u,int p);
void mpfr_ext_cos (mpfr_ptr r, mpfr_srcptr u,int p);
void mpfr_ext_tan (mpfr_ptr r, mpfr_srcptr u,int p);
void mpfr_ext_test (mpfr_ptr r, mpfr_srcptr u,int p,mp_rnd_t rnd_mode);

void iRRAM_initialize(int argc,char **argv);

#ifdef __cplusplus
}
#endif
