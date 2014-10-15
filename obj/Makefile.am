## Process this file with automake to produce Makefile.in 
# echo -e "\n***  Compiling iRRAM with MPFR multiple precision package as backend ***\n"

AM_CPPFLAGS=-I$(top_srcdir)/include -I$(top_builddir)/include @INCLUDES@

## DEFS = @DEFS@

srcdir = @srcdir@
VPATH = ../src
     
##$(top_srcdir)/sourcefile.list defines $(main_sources)
include $(top_srcdir)/sourcefile.list

DISTCLEANFILES = @iRRAM_src@

lib_LTLIBRARIES      = libiRRAM.la
libiRRAM_la_LDFLAGS  = -version-info @iRRAM_shared_version@
libiRRAM_la_SOURCES  = $(main_sources)

if WITH_BACKEND_MPFR
libiRRAM_la_SOURCES += $(mpfr_sources)
DISTCLEANFILES      += $(mpfr_sources) $(mpfr_headers)
endif
