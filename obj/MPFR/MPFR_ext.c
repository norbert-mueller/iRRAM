#include "MPFR_ext.h"


__thread int ext_mpfr_var_count=0;

#define MaxFreeVars 1000
__thread ext_mpfr_type mpfr_FreeVars[MaxFreeVars];
__thread int mpfr_FreeVarCount=0L;


