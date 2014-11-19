#

lib_LTLIBRARIES = libmarabunta-client.la

libmarabunta_client_la_CPPFLAGS = -I$(top_srcdir) -I$(top_builddir) -DCL_LOG_DOMAIN=\"mbclient\"

libmarabunta_client_la_CFLAGS = @GLIB_CFLAGS@ @CLOG_CFLAGS@

libmarabunta_client_la_SOURCES = \
	mbcbase.c \
	mbcapp.c \
	mbcgame.c \
	mbcplayer.c \
	mbcworld.c \
	mbcsector.c \
	mbccolony.c

BUILT_SOURCES = \
	mbcapp-def.h mbcapp.inl \
	mbcgame-def.h mbcgame.inl \
	mbcplayer-def.h mbcplayer.inl \
	mbcworld-def.h mbcworld.inl \
	mbcsector-def.h mbcsector.inl \
	mbccolony-def.h mbccolony.inl

mbcapp-def.h mbcapp.inl \
mbcgame-def.h mbcgame.inl \
mbcplayer-def.h mbcplayer.inl \
mbcworld-def.h mbcworld.inl \
mbcsector-def.h mbcsector.inl \
mbccolony-def.h mbccolony.inl \
: mbctypes.stamp-h

mbctypes.stamp-h: mbctypes.in
	cpp $< | $(LOS_GENTYPES) -s $(srcdir) -u 'client/'
	echo >$@
