#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ(2.59)
AC_INIT(FULL-PACKAGE-NAME, VERSION, BUG-REPORT-ADDRESS)
AC_CONFIG_SRCDIR([consoleexample/consoleexample.c])
AC_CONFIG_HEADER([config.h])
AM_INIT_AUTOMAKE

# Checks for programs.
AC_PROG_CC
AM_PROG_CC_C_O
AC_PROG_INSTALL
AC_PROG_LN_S

AC_PROG_LIBTOOL

AC_CHECK_LIB([m], [sin])
AC_CHECK_LIB([pthread], [pthread_create])

# Checks for header files.
AC_HEADER_STDC
AC_CHECK_HEADERS([fcntl.h locale.h stdlib.h string.h sys/ioctl.h sys/timeb.h termios.h unistd.h])
autoconf manual
# Checks for typedefs, structures, and compiler characteristics.
AC_HEADER_STDBOOL

# Checks for library functions.
AC_PROG_GCC_TRADITIONAL
AC_FUNC_MALLOC
AC_FUNC_STRTOD
AC_CHECK_FUNCS([ftime memset setlocale strstr strtol])

LT_INIT

LIBUSB_REQUIRED=0.1.2
PKG_CHECK_MODULES(LIBUSB, libusb >= $LIBUSB_REQUIRED)
LIBUSB_VERSION=`libusb-config --version`
AC_SUBST(LIBUSB_CFLAGS)
AC_SUBST(LIBUSB_LIBS)

AC_CONFIG_FILES([Makefile
                 consoleexample/Makefile
                 lib/Makefile
                 ])

AC_OUTPUT
