AC_INIT([marabunta], [0.0.0])
AM_INIT_AUTOMAKE

: ${CFLAGS:=-g -Wall}

AC_PROG_CC
LT_INIT

PKG_CHECK_MODULES([GLIB], [glib-2.0])

AC_CONFIG_FILES([ \
  Makefile \
  server/Makefile \
  client/Makefile \
])
AC_OUTPUT
