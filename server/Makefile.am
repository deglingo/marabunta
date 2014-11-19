#

lib_LTLIBRARIES = libmarabunta-server.la

libmarabunta_server_la_CPPFLAGS = -I$(top_srcdir) -I$(top_builddir) -DCL_LOG_DOMAIN=\"mbserver\"

libmarabunta_server_la_CFLAGS = @CLOG_CFLAGS@ @GLIB_CFLAGS@

libmarabunta_server_la_SOURCES = \
	mbsgame.c \
	mbsplayer.c \
	mbsworld.c \
	mbssector.c \
	mbscolony.c \
	mbstask.c \
	mbspoptree.c \
	mbspriority.c \
	mbsroom.c

BUILT_SOURCES = \
	mbsgame-def.h mbsgame.inl \
	mbsplayer-def.h mbsplayer.inl \
	mbswold-def.h mbsworld.inl \
	mbssector-def.h mbssector.inl \
	mbstask-def.h mbstask.inl \
	mbscolony-def.h mbscolony.inl \
	mbspriority-def.h mbspriority.inl \
	mbsroom-def.h mbsroom.inl

mbsgame-def.h mbsgame.inl \
mbsplayer-def.h mbsplayer.inl \
mbswold-def.h mbsworld.inl \
mbssector-def.h mbssector.inl \
mbstask-def.h mbstask.inl \
mbscolony-def.h mbscolony.inl \
mbspriority-def.h mbspriority.inl \
mbsroom-def.h mbsroom.inl \
: mbstypes.stamp-h

mbstypes.stamp-h: mbstypes.in
	cpp $< | $(LOS_GENTYPES) -s $(srcdir) -u 'server/'
	echo >$@
