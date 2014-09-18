## Process this file with automake to produce Makefile.in 
# SUBDIRS=include . examples
# doc still missing...

.PHONY: all backends check examples install-am clean-am distclean-am

ACLOCAL_AMFLAGS = -I m4

LIBTOOL_DEPS = @LIBTOOL_DEPS@
libtool: $(LIBTOOL_DEPS)
	$(SHELL) ./config.status --recheck

all:   backends include check

backends:
	for i in obj/*/ ; do $(MAKE) -C $$i; done

check: backends
	@echo -e "\n***  Compiling tests ***\n"
	$(MAKE) -C tests check

examples: install
	@echo -e "\n***  Compiling examples ***\n"
	$(MAKE) -C examples

install-am:
	for i in obj/*/ ; do $(MAKE) -C $$i LIBTOOLFLAGS=--silent install; done
	(cd include; make LIBTOOLFLAGS=--silent install)
	(cd examples; sed -e 's#\$${prefix}#${prefix}#' -e 's#\$${exec_prefix}#${exec_prefix}#' Makefile-pre > Makefile )

clean-am: 
	for i in obj/*/ ; do $(MAKE) -C $$i clean; done
	for i in examples tests; do $(MAKE) -C $$i clean; done
	rm -fr autom4te.cache

distclean-am: clean-am
	rm -f config.log iRRAM_VERSION 
	rm -fr installed 
	for i in obj/*/ ; do $(MAKE) -C $$i distclean; done
	for i in examples tests doc; do $(MAKE) -C $$i distclean; done
