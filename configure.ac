AC_INIT([marabunta], [0.0.0])
AM_INIT_AUTOMAKE

: ${CFLAGS:=-g -Wall}

AC_PROG_CC
AM_PROG_CC_C_O
LT_INIT

PKG_CHECK_MODULES([CLOG], [clog])
PKG_CHECK_MODULES([GLIB], [glib-2.0])
PKG_CHECK_MODULES([LOS], [los])
PKG_CHECK_MODULES([LPTREE], [lptree])
dnl PKG_CHECK_MODULES([AL5], [allegro-debug-5 allegro_main-debug-5])
PKG_CHECK_MODULES([ALTK], [altk])

AC_CONFIG_FILES([ \
  Makefile \
  common/Makefile \
  server/Makefile \
  client/Makefile \
  client-al/Makefile \
])
AC_OUTPUT
