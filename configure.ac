#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.68])
AC_INIT([ichat], [0.2])
AM_INIT_AUTOMAKE(ichat, 0.2)
LT_INIT
AC_CONFIG_SRCDIR([config.h.in])
AC_CONFIG_HEADERS([config.h])

# Checks for programs.
AC_PROG_CC
AC_PROG_RANLIB
# Checks for libraries.

# Checks for header files.
AC_CHECK_HEADERS([stdlib.h string.h])

# Checks for typedefs, structures, and compiler characteristics.
# Checks for packages
PKG_CHECK_MODULES([GTK3], [gtk+-3.0])
PKG_CHECK_MODULES([XML2], [libxml-2.0])

# Checks for library functions.
AC_FUNC_MALLOC
AC_CHECK_FUNCS([memset])

AC_CONFIG_FILES([Makefile
                 resources/Makefile
                 resources/images/Makefile
                 resources/images/loading/Makefile
                 resources/images/qqfaces/Makefile
                 resources/images/status/Makefile
                 resources/sounds/Makefile
                 src/Makefile
                 src/client/Makefile
                 src/common/Makefile
                 src/gui/Makefile
                 src/libwebqq/Makefile
                 src/tools/Makefile])
AC_OUTPUT
