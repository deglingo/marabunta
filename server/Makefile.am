#

lib_LTLIBRARIES = libmarabunta-server.la

libmarabunta_server_la_CPPFLAGS = -I$(top_srcdir) -I$(top_builddir) -DCL_LOG_DOMAIN=\"mbserver\"

libmarabunta_server_la_CFLAGS = @CLOG_CFLAGS@ @GLIB_CFLAGS@

libmarabunta_server_la_SOURCES = \
	mbsgame.c \
	mbsplayer.c

BUILT_SOURCES = \
	mbsgame-def.h mbsgame.inl \
	mbsplayer-def.h mbsplayer.inl

mbsgame-def.h mbsgame.inl \
mbsplayer-def.h mbsplayer.inl \
: mbstypes.stamp-h

mbstypes.stamp-h: mbstypes.in
	cpp $< | $(LOS_GENTYPES) -s $(srcdir) -u 'server/'
	echo >$@
