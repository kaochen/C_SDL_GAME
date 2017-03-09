#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.69])
AC_INIT(SokoRobot, 0.5, no_bug_report_for_now)
AC_CONFIG_SRCDIR([src/main.c])
AM_INIT_AUTOMAKE
AC_CONFIG_HEADERS([config.h])

# Checks for programs.
AC_PROG_CC

# Checks for header files.
AC_CHECK_HEADERS(libintl.h locale.h stdlib.h string.h)

# Checks for typedefs, structures, and compiler characteristics.
AC_CHECK_HEADER_STDBOOL
AC_TYPE_SIZE_T

# Checks for library functions.
AC_FUNC_MALLOC
AC_CHECK_FUNCS([setlocale strchr strerror strpbrk strrchr strstr])

AC_CONFIG_FILES([
	Makefile
	src/Makefile
	data/Makefile
	data/img/Makefile
	data/levels/Makefile
])

AC_OUTPUT
