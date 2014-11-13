dnl Process this file with autoconf to produce a configure script.
AC_INIT(iRRAM,2014_01,[],[iRRAM],[iRRAM.uni-trier.de])

iRRAM_VERSION=2014_01
iRRAM_shared_version=14:1:0
AC_SUBST(iRRAM_VERSION)
AC_SUBST(iRRAM_shared_version)

AC_CONFIG_MACRO_DIR([m4])
AC_CONFIG_SRCDIR([sourcefile.list])

GMPLDFLAGS=
GMPCPPFLAGS=
MPFRLDFLAGS=
MPFRCPPFLAGS=

dnl TODO: should be AC_ARG_ENABLE
AC_ARG_WITH([backends],
	[AS_HELP_STRING([--with-backends=MP],[additional backends for MP arithmetic @<:@available: MPFR, default: MPFR@:>@])],
	[with_backends=$withval])
AC_ARG_WITH([gmp],
	[AS_HELP_STRING([--with-gmp=DIR],[GMP install directory])],
	[GMPCPPFLAGS="-I$withval/include"
	 GMPLDFLAGS="-L$withval/lib"])
AC_ARG_WITH([gmp-include],
	[AS_HELP_STRING([--with-gmp-include=DIR],[GMP include directory])],
	[GMPCPPFLAGS="-I$withval"])
AC_ARG_WITH([gmp-lib],
	[AS_HELP_STRING([--with-gmp-lib=DIR],[GMP lib directory])],
	[GMPLDFLAGS="-L$withval"])
AC_ARG_WITH([mpfr],
	[AS_HELP_STRING([--with-mfpr=DIR],[MPFR install directory])],
	[MPFRCPPFLAGS="-I$withval/include"
	 MPFRLDFLAGS="-L$withval/lib"])
AC_ARG_WITH([mpfr-include],
	[AS_HELP_STRING([--with-mpfr-include=DIR],[MPFR include directory])],
	[MPFRCPPFLAGS="-I$withval"])
AC_ARG_WITH([mpfr-lib],
	[AS_HELP_STRING([--with-mpfr-lib=DIR],[MPFR lib directory])],
	[MPFRLDFLAGS="-L$withval"])

dnl Profiling
AC_ARG_ENABLE([profiling],
	[AS_HELP_STRING([--enable-profiling],[build with profiler support @<:@available: gprof, default: no@:>@])],
	[AS_CASE([$enableval],
		[no|gprof],[],
		[AC_MSG_ERROR([bad value $enableval for --enable-profiling, need no/gprof])])],
	[enable_profiling=no])

AM_SILENT_RULES(yes)
AM_INIT_AUTOMAKE()

AM_CPPFLAGS="$AM_CPPFLAGS $GMPCPPFLAGS $MPFRCPPFLAGS"
AM_LDFLAGS="$AM_LDFLAGS $GMPLDFLAGS $MPFRLDFLAGS"

LT_INIT
AC_SUBST(LIBTOOL)
AC_SUBST(LIBTOOL_DEPS)
AC_SUBST(LD)

dnl Checks for programs.
AC_PROG_MAKE_SET
AC_PROG_CXX
# these macros (may) modify *FLAGS, so we'll save those
save_CFLAGS=$CFLAGS
save_CXXFLAGS=$CXXFLAGS
CFLAGS=
CXXFLAGS=
AC_PROG_CC_C99
AX_CXX_COMPILE_STDCXX_11([noext])
AM_CFLAGS="$AM_CFLAGS $CFLAGS"
AM_CXXFLAGS="$AM_CXXFLAGS $CXXFLAGS"
CFLAGS=$save_CFLAGS
CXXFLAGS=$save_CXXFLAGS

if test "$enable_profiling" != "no" ; then
  AM_CFLAGS="$AM_CFLAGS -pg -fno-inline"
  AM_CXXFLAGS="$AM_CXXFLAGS -pg -fno-inline"
  AM_LDFLAGS="$AM_LDFLAGS -pg -fno-inline"
fi

AC_LANG([C++])

dnl If compiler is gcc, then use some warning flags
if test -n "$GCC"; then
  AM_CFLAGS="$AM_CFLAGS -Wall -Wno-implicit -Wmissing-prototypes -pedantic"
  AM_CXXFLAGS="$AM_CXXFLAGS -Wall -pedantic"
fi

AS_CASE([$host],
	[*-mingw*],[
	dnl Use ANSI printf modifiers in mingw32.
	dnl See <http://sourceforge.net/p/mingw-w64/wiki2/gnu%20printf/>
	AM_CPPFLAGS="$AM_CPPFLAGS -D__USE_MINGW_ANSI_STDIO=1"

	dnl Do not use dynamic linking on Windows
	#AM_LDFLAGS="-all-static $AM_LDFLAGS"

	save_LIBS=$LIBS
	LIBS=
	AC_SEARCH_LIBS([GetProcessMemoryInfo],[psapi kernel32],,
		[AC_MSG_ERROR([unable to find library containing GetProcessMemoryInfo().])])
	LIBS_INST="$LIBS_INST $LIBS"
	LIBS="$LDADD $save_LIBS $LIBS_INST"
])

dnl Test backends

if test "$with_include_path" != "" ; then
	AM_CPPFLAGS="$AM_CPPFLAGS -I$with_include_path"
	AC_MSG_RESULT([new AM_CPPFLAGS=$AM_CPPFLAGS])
fi

if test "$with_lib_path" != "" ; then
	LDADD="-L$with_lib_path $LDADD"
	RPATH="-Xlinker -rpath -Xlinker $with_lib_path $RPATH"
	LD_LIBRARY_PATH="$with_lib_path:$LD_LIBRARY_PATH"
	LIBS="$LDADD $LIBS_INST"
fi

CPPFLAGS_INST="$AM_CPPFLAGS $CPPFLAGS"

save_CPPFLAGS="$CPPFLAGS"
save_CFLAGS="$CFLAGS"
save_CXXFLAGS="$CXXFLAGS"
save_LDFLAGS="$LDFLAGS"
CPPFLAGS="$AM_CPPFLAGS $CPPFLAGS"
CFLAGS="$AM_CFLAGS $CFLAGS"
CXXFLAGS="$AM_CXXFLAGS $CXXFLAGS"
LDFLAGS="$AM_LDFLAGS $LDFLAGS"

AC_CHECK_LIB(m,sin,LIBS_INST="-lm")

AC_CHECK_HEADERS(pthread.h)
AC_CHECK_LIB(pthread, pthread_create,LIBS_INST="$LIBS_INST -lpthread")
LIBS="$LDADD $LIBS_INST"


AS_BOX([Checking GMP headers/lib...])
AC_CHECK_HEADER(gmp.h,,[AC_MSG_ERROR([GNU Multiple Precision Arithmetic library (<https://gmplib.org/>) is required.])])

IRRAM_GMP_VERSION_CHECK(4,1,0)

LIBS="-lgmp $LIBS"

AC_DEFUN([gmp_link_check],[
	AC_LANG_PUSH([$1])
	AC_MSG_CHECKING([whether $1-linking against GMP works])
	AC_LINK_IFELSE([AC_LANG_SOURCE([[
#include <gmp.h>
int main() {
	mpz_init(0);
	return 0;
}]])],[AC_MSG_RESULT([yes])],[AC_MSG_ERROR([$1-linking to GMP failed.])])
	AC_LANG_POP([$1])
])
gmp_link_check([C])
gmp_link_check([C++])

LIBS_INST="-lgmp $LIBS_INST"
LIBS="$LDADD $LIBS_INST"


with_backends=`echo "$with_backends" | $SED 's#,\+# #g' | LC_ALL=C tr 'a-z' 'A-Z'`
with_backends=`echo $with_backends`
if test -z "$with_backends"; then
	AC_MSG_NOTICE([No backend given, defaulting to MPFR backend])
	with_backends="MPFR"
fi
iRRAM_BACKEND_MPFR=0
BACKENDS=""
for be in $with_backends; do
	if test $be == MPFR; then
		if test $iRRAM_BACKEND_MPFR -ne 0; then
			continue
		fi
		AS_BOX([Checking MPFR headers/lib])
		AC_CHECK_HEADER([mpfr.h],[],
			[AC_MSG_ERROR([MPFR headers not found! You need to install MPFR first!],101)])
		AC_CHECK_LIB([mpfr],[mpfr_get_version],
			[LIBS_INST="-lmpfr $LIBS_INST"],
			[AC_MSG_ERROR([MPFR library not usable! Please check your installation of MPFR.],101)])
		LIBS="$LDADD $LIBS_INST"
		AC_MSG_RESULT([MPFR backend: found])
		EXTRAEXAMPLES="$EXTRAEXAMPLES timings-MPFR-iRRAM test-MPFR-iRRAM"
		iRRAM_BACKEND_MPFR=1
		BACKENDS="$BACKENDS MPFR"
	else
		AC_MSG_ERROR([Backend '$be' is unavailable!])
	fi
done
BACKENDS=`echo $BACKENDS`
AC_SUBST(iRRAM_BACKEND_MPFR)
AM_CONDITIONAL([WITH_BACKEND_MPFR],[test x$iRRAM_BACKEND_MPFR = x1])
AC_SUBST(iRRAM_BACKENDS,[`echo ${BACKENDS} | tr ' ' ,`])

CPPFLAGS="$save_CPPFLAGS"
CFLAGS="$save_CFLAGS"
CXXFLAGS="$save_CXXFLAGS"
LDFLAGS="$save_LDFLAGS"

AC_SUBST(CPPFLAGS_INST)
AC_SUBST(AM_CPPFLAGS)
AC_SUBST(AM_CFLAGS)
AC_SUBST(AM_CXXFLAGS)
AC_SUBST(LDADD)
AC_SUBST(AM_LDFLAGS)
AC_SUBST(LIBS_INST)
AC_SUBST(RPATH)

dnl These are always set automatically
#AC_SUBST(CPPFLAGS)
#AC_SUBST(CFLAGS)
#AC_SUBST(CXXFLAGS)
#AC_SUBST(LDFLAGS)
#AC_SUBST(LIBS)

AC_SUBST(EXTRAEXAMPLES)

iRRAM_src="REALS.cc REALmain.cc REALLIB.cc \
limits.cc errno.cc convert.cc stack.cc STREAMS.cc \
DYADIC.cc COMPLEX.cc RATIONAL.cc LAZY_BOOLEAN.cc \
sqrt.cc exp_log.cc sin_cos.cc pi_ln2.cc \
REALMATRIX.cc SPARSEREALMATRIX.cc INTERVAL.cc \
GMP_int_ext.c GMP_rat_ext.c"
AC_SUBST(iRRAM_src)

MAKEFILES=""

for j in $iRRAM_src; do
	AC_CONFIG_LINKS(build/$j:$srcdir/src/$j)
done

MPFR_src="MPFR_ext.c mpfr_extension.cc MPFR_ext.h"

for be in $BACKENDS; do
	be_src=${be}_src
	be_src=`eval echo \\\$${be_src}`
	for i in ${be_src}; do
		AC_CONFIG_LINKS(build/$i:$srcdir/src/$be/$i)
	done
done

AC_CONFIG_FILES([build/Makefile])
AC_CONFIG_FILES([iRRAM_VERSION include/iRRAM/version.h] )

AC_OUTPUT(Makefile include/Makefile tests/Makefile examples/Makefile-pre doc/Makefile )

AC_MSG_NOTICE([

CC      : $CC
CXX     : $CXX
CPPFLAGS: $AM_CPPFLAGS $CPPFLAGS
CFLAGS  : $AM_CFLAGS $CFLAGS
CXXFLAGS: $AM_CXXFLAGS $CXXFLAGS
LDFLAGS : $AM_LDFLAGS $LDFLAGS
shared  : $enable_shared
static  : $enable_static
])
