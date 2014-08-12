
SUBDIRS = src tests

include $(top_srcdir)/Makefile.am.coverage

BUILT_SOURCES = module-update

module-update:
	git submodule update --init --recursive
