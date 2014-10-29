#

lib_LTLIBRARIES = libmarabunta-server.la

libmarabunta_server_la_CPPFLAGS = -I$(top_srcdir) -I$(top_builddir) -DCL_LOG_DOMAIN=\"mbserver\"

libmarabunta_server_la_CFLAGS = @CLOG_CFLAGS@ @GLIB_CFLAGS@ @LPTREE_CFLAGS@

libmarabunta_server_la_SOURCES = \
	mbsserver.c \
	mbsworld.c \
	mbsgame.c \
	mbsapp.c

BUILT_SOURCES = \
	mbsworld-def.h mbsworld.inl \
	mbsgame-def.h mbsgame.inl \
	mbsapp-def.h mbsapp.inl

mbsworld-def.h mbsworld.inl \
mbsgame-def.h mbsgame.inl \
mbsapp-def.h mbsapp.inl \
: mbstypes.stamp-h

mbstypes.stamp-h: mbstypes.in
	cpp $< | $(LOS_GENTYPES) -s $(srcdir) -u 'server/'
	echo >$@
