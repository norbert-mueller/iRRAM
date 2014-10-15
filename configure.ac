dnl Process this file with autoconf to produce a configure script.
AC_INIT(iRRAM,2014_01,[],[],[iRRAM.uni-trier.de])

iRRAM_VERSION=2014_01
iRRAM_shared_version=14:1:0

AC_CONFIG_MACRO_DIR([m4])

LT_INIT
AC_SUBST(LIBTOOL)
AC_SUBST(LIBTOOL_DEPS)
AC_SUBST(LD)

AM_SILENT_RULES(yes)
AM_INIT_AUTOMAKE()

AC_ARG_WITH(backend,   [ --with-backend=MP       additional backend for MP arithmetic ], with_backend=$withval)
AC_ARG_WITH(include_path,  [ --with-include-path=DIR  additional path for includes of GMP, MPFR or backends ], with_include_path=$withval)
AC_ARG_WITH(lib_path,  [ --with-lib-path=DIR      additional path for libs of GMP, MPFR orbackends ], with_lib_path=$withval)


dnl Profiling
AC_ARG_ENABLE(profiling,
AC_HELP_STRING([--enable-profiling],
               [build with profiler support [[default=no]]]),
[case $enableval in
no|gprof) ;;
*) AC_MSG_ERROR([bad value $enableval for --enable-profiling, need no/gprof]) ;;
esac],
[enable_profiling=no])
if test "$enable_profiling" != "no" ; then
  CFLAGS="$CFLAGS -pg -fno-inline"
  CXXFLAGS="$CXXFLAGS -pg -fno-inline"
  LDFLAGS="$LDFLAGS -pg -fno-inline"
fi

dnl Checks for programs.
AC_PROG_CC_C99
AC_PROG_CXX
AC_PROG_MAKE_SET
AC_PROG_LN_S

AC_LANG_CPLUSPLUS

AC_CHECK_LIB(m, main,LIBS_INST="-lm")

dnl If compiler is gcc, then use some warning flags
if test -n "$GCC"; then
  CFLAGS="$CFLAGS -Wall -Wno-implicit -Wmissing-prototypes -pedantic"
  CXXFLAGS="$CXXFLAGS -Wall -pedantic"
fi

case "$host" in
*-mingw*)
	dnl Use ANSI printf modifiers in mingw32.
	dnl See <http://sourceforge.net/p/mingw-w64/wiki2/gnu%20printf/>
	CPPFLAGS="-D__USE_MINGW_ANSI_STDIO=1 $CPPFLAGS"

	dnl Do not use dynamic linking on Windows
	AM_LDFLAGS="-all-static $AM_LDFLAGS"

	dnl GetProcessMemoryInfo() library psapi
	LIBS_INST="$LIBS_INST -lpsapi"
	LIBS="$LDADD $LIBS_INST"
	;;
esac

dnl Test backends

if test "$with_include_path" != "" ; then
  INCLUDES="-I$with_include_path"
fi

if test "$with_lib_path" != "" ; then
	LDADD="-L$with_lib_path $LDADD"
	RPATH="-Xlinker -rpath -Xlinker $with_lib_path $RPATH"
	LD_LIBRARY_PATH="$with_lib_path:$LD_LIBRARY_PATH"
	LIBS="$LDADD $LIBS_INST"
fi

CPPFLAGS_INST="$CPPFLAGS"
CPPFLAGS="$CPPFLAGS $INCLUDES"
AC_MSG_RESULT(checking gmp header... )
AC_CHECK_HEADER(gmp.h, ,gmp_lib=no)

if test -z "$gmp_lib" ;  then 
	AC_TRY_RUN( [
		#include <cstdio>
		#include <gmp.h>
		#include <cstring>
		int main(int argc, char *argv[])
		{ //if ( strcmp (gmp_version,"4.1") < 0 ) return 1;
		  if ( __GNU_MP_VERSION < 4 ) return 1;
		  if ( __GNU_MP_VERSION == 4 && __GNU_MP_VERSION_MINOR < 1 ) return 1;
		  return 0;
		}
	],AC_MSG_RESULT([found recent GMP]),
	AC_MSG_ERROR([found GMP < 4.1... GMP version too old]),
	AC_MSG_ERROR( [cross compiling not supported! ] ) ) 
fi
if test -z "$gmp_lib" ;  then 
	AC_MSG_RESULT( [GMP check successful! ] )
else
	AC_MSG_ERROR( [GMP not found! You need to install GMP first!], 100 ) 
fi

LIBS_INST="$LIBS_INST -lgmp"
LIBS="$LDADD $LIBS_INST"
 
AC_MSG_RESULT(checking mpfr headers )
AC_CHECK_HEADER(mpfr.h, ,mpfr_h=no)

if test -z "$mpfr_h" ;  then 
        AC_MSG_RESULT( MPFR backend: found )
	BACKENDS="$BACKENDS obj/MPFR"
	backends="$backends MPFR"
	EXTRAEXAMPLES="$EXTRAEXAMPLES timings-MPFR-iRRAM test-MPFR-iRRAM"
else 
	AC_MSG_ERROR( [MPFR not found! You need to install MPFR first!],101 ) 
fi

if test "$with_backend" != "" ; then
	AC_MSG_RESULT(  )
	AC_MSG_ERROR( [Currently, further backends do not exist! ] )
fi

LIBS_INST="-lmpfr $LIBS_INST"

AC_CHECK_HEADERS(pthread.h)
AC_CHECK_LIB(pthread, pthread_create,LIBS_INST="$LIBS_INST -lpthread")

LIBS="$LDADD $LIBS_INST"

AC_LANG_CPLUSPLUS
AX_CXX_COMPILE_STDCXX_11([noext])

AC_SUBST(CPPFLAGS)                                                           
AC_SUBST(CPPFLAGS_INST)                                                           
AC_SUBST(CXXFLAGS)                                                           
AC_SUBST(INCLUDES)
AC_SUBST(LDADD)
AC_SUBST(LDFLAGS)
AC_SUBST(AM_LDFLAGS)
AC_SUBST(LIBS)
AC_SUBST(LIBS_INST)
AC_SUBST(CFLAGS)
AC_SUBST(RPATH)
AC_SUBST(EXTRAEXAMPLES)
AC_SUBST(iRRAM_VERSION)
AC_SUBST(iRRAM_shared_version)

iRRAM_src="REALS.cc REALmain.cc REALLIB.cc \
limits.cc errno.cc convert.cc stack.cc STREAMS.cc \
DYADIC.cc COMPLEX.cc Integer.cc RATIONAL.cc LAZY_BOOLEAN.cc \
sqrt.cc exp_log.cc sin_cos.cc pi_ln2.cc \
REALMATRIX.cc SPARSEREALMATRIX.cc INTERVAL.cc \
GMP_int_ext.c GMP_rat_ext.c"
AC_SUBST(iRRAM_src)

MAKEFILES=""

for bg in obj/*/; do
 cd $bg
 for i in $iRRAM_src; do
 	AC_MSG_RESULT( linking $bg/$i )
 	$LN_S ../../src/$i $i
 done
 cd ../..
done


AC_CONFIG_FILES([obj/MPFR/Makefile])
AC_CONFIG_FILES([iRRAM_VERSION include/iRRAM/version.h] )

AC_OUTPUT(Makefile include/Makefile tests/Makefile examples/Makefile-pre doc/Makefile )
