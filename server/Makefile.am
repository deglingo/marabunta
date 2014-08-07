#

bin_PROGRAMS = marabunta-server

marabunta_server_CPPFLAGS = -I$(top_srcdir) -DG_LOG_DOMAIN=\"mbserver\"

marabunta_server_CFLAGS = @GLIB_CFLAGS@

marabunta_server_LDFLAGS = @GLIB_LIBS@

marabunta_server_SOURCES = \
	mbsserver.c \
	mbsgame.c \
	mbsmain.c
