
noinst_PROGRAMS = arg3fmt_test

arg3fmt_test_SOURCES = arg3.test.cpp format.test.cpp

AM_CPPFLAGS = -std=c++11 -stdlib=libc++ -Ivendor/bandit -Wall -Werror

arg3fmt_test_LDADD = ../src/libarg3fmt-0.2.la

all-local: arg3fmt_test$(EXEEXT)
	./arg3fmt_test
