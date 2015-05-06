
SUBDIRS = src tests

include $(top_srcdir)/Makefile.am.coverage

ACLOCAL_AMFLAGS = -I m4

EXTRA_DIST = bootstrap

pkgconfigdir = $(libdir)/pkgconfig
nodist_pkgconfig_DATA = arg3fmt.pc

test: check

