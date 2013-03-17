#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.68])
AC_INIT([ichat], [0.1])
AM_INIT_AUTOMAKE(ichat, 0.1)
AC_CONFIG_SRCDIR([src/ic_client_main.c])
AC_CONFIG_HEADERS([config.h])

# Checks for programs.
AC_PROG_CC

# Checks for libraries.

# Checks for header files.
AC_CHECK_HEADERS([stdlib.h string.h])

# Checks for typedefs, structures, and compiler characteristics.

# Checks for library functions.
AC_FUNC_MALLOC
AC_CHECK_FUNCS([memset])

AC_CONFIG_FILES([Makefile
                 res/Makefile
                 res/images/Makefile
                 res/images/loading/Makefile
                 res/images/photo/Makefile
                 res/images/status/Makefile
                 src/Makefile])
AC_OUTPUT
