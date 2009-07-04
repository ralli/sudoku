#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.63])
AC_INIT(sudoku, 0.1, [r_juhnke@gmx.de])
AC_CONFIG_SRCDIR([sudoku/sudoku.cpp])
AC_CONFIG_HEADERS([include/config.h])
AC_CONFIG_AUX_DIR([config])
AM_INIT_AUTOMAKE([foreign])

# Checks for programs.
AC_PROG_CXX

# Checks for libraries.
# Checks for programs.
AC_PROG_CXX

GTKMM_CFLAGS=`pkg-config --cflags gtkmm-2.4`
GTKMM_LIBS=`pkg-config --libs gtkmm-2.4`

AC_SUBST(GTKMM_CFLAGS)
AC_SUBST(GTKMM_LIBS)

# Checks for header files.

# Checks for typedefs, structures, and compiler characteristics.
AC_HEADER_STDBOOL
AC_C_INLINE

# Checks for library functions.

AC_CONFIG_FILES([Makefile sudoku/Makefile gsudoku/Makefile include/Makefile])
AC_OUTPUT
