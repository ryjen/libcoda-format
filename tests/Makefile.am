
check_PROGRAMS = arg3fmt_test

arg3fmt_test_SOURCES = arg3.test.cpp format.test.cpp

AM_CPPFLAGS = @AM_CPPFLAGS@ -Ivendor/bandit -Wall -Werror

arg3fmt_test_LDADD = ../src/libarg3fmt-0.2.la

check: arg3fmt_test
	./arg3fmt_test

