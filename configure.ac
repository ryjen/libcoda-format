#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.68])
AC_INIT([arg3fmt], [0.2], [c0der78@gmail.com])
AC_CONFIG_SRCDIR([src/format.cpp])
AC_CONFIG_HEADERS([src/config.h])
AM_INIT_AUTOMAKE
LT_INIT

: ${CXXFLAGS=""}

AC_CONFIG_MACRO_DIR([m4])

# Checks for programs.
AC_PROG_CXX([clang++ g++])
AC_PROG_CC

if test "x$CXX" = "xclang++"; then
	AC_SUBST([AM_CPPFLAGS], ["-std=c++11 -stdlib=libc++ -Wall -Werror"])
else
	AC_SUBST([AM_CPPFLAGS], ["-std=c++11 -Wall -Werror"])
fi

# coverage
m4_include([m4/gcov.m4])
AC_TDD_GCOV
AC_SUBST(COVERAGE_CFLAGS)
AC_SUBST(COVERAGE_CXXFLAGS)
AC_SUBST(COVERAGE_LDFLAGS)

AC_CHECK_PROG(GIT_CHECK,git,yes)
if test x"$GIT_CHECK" != x"yes" ; then
    AC_MSG_ERROR([Required program 'git' not found.])
else
    git submodule update --init --recursive
fi

# Checks for header files.

# Checks for typedefs, structures, and compiler characteristics.
AC_C_INLINE
AC_TYPE_INT8_T
AC_TYPE_SIZE_T

# Checks for library functions.

AC_CONFIG_FILES([Makefile src/Makefile tests/Makefile])
AC_OUTPUT
