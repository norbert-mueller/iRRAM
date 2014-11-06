#
# IRRAM_GMP_VERSION_CHECK(major,minor,patch)
#
AC_DEFUN([IRRAM_GMP_VERSION_CHECK],[AC_REQUIRE([AC_PROG_EGREP])
AC_REQUIRE([AC_PROG_SED])
AC_LANG_PREPROC_REQUIRE
AC_MSG_CHECKING([for GMP header version])
AC_LANG_CONFTEST([AC_LANG_SOURCE([
#include <gmp.h>
#if __GNU_MP_VERSION < $1 || __GNU_MP_VERSION == $1 && __GNU_MP_VERSION_MINOR < $2 || __GNU_MP_VERSION == $1 && __GNU_MP_VERSION_MINOR == $2 && __GNU_MP_VERSION_PATCH < $3
# error installed GMP library header version too old
#endif
version: __GNU_MP_VERSION.__GNU_MP_VERSION_MINOR.__GNU_MP_VERSION_PATCHLEVEL])])
AS_IF([(eval "$ac_cpp conftest.$ac_ext") 2>&AS_MESSAGE_LOG_FD > conftest.cpped],[gmp_header_version_ok=yes],[gmp_header_version_ok=no])
gmp_header_version=`$EGREP "^version: .*" conftest.cpped | $SED 's#^version: ##;s# ##g'`
rm -f conftest*
AS_IF([test x$gmp_header_version_ok = xyes],[AC_MSG_RESULT([$gmp_header_version >= $1.$2.$3, OK])],
      [AC_MSG_RESULT([$gmp_header_version < $1.$2.$3])
       AC_MSG_ERROR([GMP version is too old, need >= $1.$2.$3.])])])
