#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.68])
AC_INIT([ichat], [0.2])
AM_INIT_AUTOMAKE(ichat, 0.2)
LT_INIT
AC_CONFIG_SRCDIR([config.h.in])
AC_CONFIG_HEADERS([config.h])

# Checks for programs.
AC_PROG_CXX
AC_PROG_AWK
AC_PROG_CC
AC_PROG_CPP
AC_PROG_INSTALL
AC_PROG_LN_S
AC_PROG_MAKE_SET
AC_PROG_RANLIB

# Checks for libraries.
PKG_CHECK_MODULES([GTK3], [gtk+-3.0])
PKG_CHECK_MODULES([GLIB], [glib-2.0])
PKG_CHECK_MODULES([SQLITE3], [sqlite3])

# Checks for header files.
AC_FUNC_ALLOCA
AC_CHECK_HEADERS([fcntl.h memory.h netdb.h netinet/in.h stdint.h stdlib.h string.h sys/socket.h sys/time.h unistd.h])

# Checks for typedefs, structures, and compiler characteristics.
AC_TYPE_INT64_T
AC_TYPE_SIZE_T
AC_TYPE_UINT64_T

# Checks for library functions.
AC_FUNC_MALLOC
AC_FUNC_MKTIME
AC_FUNC_REALLOC
AC_CHECK_FUNCS([gethostbyname gettimeofday memmove memset mkdir socket strcasecmp strchr strdup strerror strndup strstr strtol])

AC_CONFIG_FILES([Makefile
                 resources/Makefile
                 resources/images/Makefile
                 resources/images/loading/Makefile
                 resources/images/qqfaces/Makefile
                 resources/images/status/Makefile
                 resources/sounds/Makefile
                 src/Makefile
                 src/client/Makefile
                 src/gui/Makefile
                 src/include/Makefile
                 src/libwebqq/Makefile
                 src/libwebqq/libghttp/Makefile
                 src/tools/Makefile])
AC_OUTPUT
