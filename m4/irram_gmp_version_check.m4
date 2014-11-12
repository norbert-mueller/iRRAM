#
# IRRAM_GMP_VERSION_CHECK(major,minor,patch)
#
AC_DEFUN([IRRAM_GMP_VERSION_CHECK],[AC_REQUIRE([AC_PROG_EGREP])
AC_REQUIRE([AC_PROG_SED])
AC_LANG_PREPROC_REQUIRE
AC_MSG_CHECKING([GMP header version])
dnl
dnl Since autoconf (at least) >= 2.69 we could also access the preprocessed
dnl output in 'conftest.i' using AC_PREPROC_IFELSE, but we currently have no
dnl hard requirement on that autoconf version.
dnl
AC_LANG_CONFTEST([AC_LANG_SOURCE([[
#include <gmp.h>
version: __GNU_MP_VERSION.__GNU_MP_VERSION_MINOR.__GNU_MP_VERSION_PATCHLEVEL
]])])
AS_IF([(eval "$ac_cpp conftest.$ac_ext") 2>&AS_MESSAGE_LOG_FD > conftest.cpped],
      [irram_gmp_header_version_ok=yes],
      [irram_gmp_header_version_ok=no])
irram_gmp_header_version=`$EGREP "^version: .*" conftest.cpped | $SED 's#^version: ##;s# ##g'`
rm -f conftest*
AS_IF([test x$irram_gmp_header_version_ok = xno],
      [AC_MSG_ERROR([unable to run preprocessor on <gmp.h>])])
AC_PREPROC_IFELSE([AC_LANG_SOURCE([
#include <gmp.h>
#if __GNU_MP_VERSION       < $1 || __GNU_MP_VERSION       == $1 && (\
    __GNU_MP_VERSION_MINOR < $2 || __GNU_MP_VERSION_MINOR == $2 && (\
    __GNU_MP_VERSION_PATCH < $3))
# error installed GMP library header version too old
#endif
])],[AC_MSG_RESULT([$irram_gmp_header_version >= $1.$2.$3, OK])],
    [AC_MSG_RESULT([$irram_gmp_header_version < $1.$2.$3])
     AC_MSG_ERROR([GMP version is too old, need >= $1.$2.$3.])])])
