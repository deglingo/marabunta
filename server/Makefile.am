#

bin_PROGRAMS = marabunta-server

marabunta_server_CPPFLAGS = -I$(top_srcdir) -I$(top_builddir) -DCL_LOG_DOMAIN=\"mbserver\"

marabunta_server_CFLAGS = @CLOG_CFLAGS@ @GLIB_CFLAGS@ @LPTREE_CFLAGS@

marabunta_server_LDFLAGS = \
	$(top_builddir)/common/libmarabunta-common.la \
	@CLOG_LIBS@ @GLIB_LIBS@ @LPTREE_LIBS@

marabunta_server_SOURCES = \
	mbsserver.c \
	mbsgame.c \
	mbsmain.c

BUILT_SOURCES = \
	mbsgame-def.h mbsgame.inl

mbsgame-def.h mbsgame.inl \
: mbstypes.stamp-h

mbstypes.stamp-h: mbstypes.in
	cpp $< | $(LOS_GENTYPES)
	echo >$@
