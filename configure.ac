AC_INIT([marabunta], [0.0.0])
AM_INIT_AUTOMAKE

: ${CFLAGS:=-g -Wall}

AC_PROG_CC
AM_PROG_CC_C_O
LT_INIT

PKG_CHECK_MODULES([GLIB], [glib-2.0])
PKG_CHECK_MODULES([LPTREE], [lptree])

AC_CONFIG_FILES([ \
  Makefile \
  server/Makefile \
  client/Makefile \
  client-al/Makefile \
])
AC_OUTPUT
