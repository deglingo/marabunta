#

lib_LTLIBRARIES = libmarabunta-server.la

libmarabunta_server_la_CPPFLAGS = -I$(top_srcdir) -I$(top_builddir) -DCL_LOG_DOMAIN=\"mbserver\"

libmarabunta_server_la_CFLAGS = @CLOG_CFLAGS@ @GLIB_CFLAGS@

libmarabunta_server_la_SOURCES = \
	mbsobject.c \
	mbsserver.c \
	mbsworld.c \
	mbssector.c \
	mbscolony.c \
	mbspoptree.c \
	mbscontrol.c \
	mbspriority.c \
	mbstask.c \
	mbsroom.c \
	mbsresource.c \
	mbsgame.c \
	mbsapp.c

BUILT_SOURCES = \
	mbsobject-def.h mbsobject.inl \
	mbsworld-def.h mbsworld.inl \
	mbssector-def.h mbssector.inl \
	mbscolony-def.h mbscolony.inl \
	mbscontrol-def.h mbscontrol.inl \
	mbspriority-def.h mbspriority.inl \
	mbstask-def.h mbstask.inl \
	mbsroom-def.h mbsroom.inl \
	mbsresource-def.h mbsresource.inl \
	mbsgame-def.h mbsgame.inl \
	mbsapp-def.h mbsapp.inl

mbsobject-def.h mbsobject.inl \
mbsworld-def.h mbsworld.inl \
mbssector-def.h mbssector.inl \
mbscolony-def.h mbscolony.inl \
mbscontrol-def.h mbscontrol.inl \
mbspriority-def.h mbspriority.inl \
mbstask-def.h mbstask.inl \
mbsroom-def.h mbsroom.inl \
mbsresource-def.h mbsresource.inl \
mbsgame-def.h mbsgame.inl \
mbsapp-def.h mbsapp.inl \
: mbstypes.stamp-h

mbstypes.stamp-h: mbstypes.in
	cpp $< | $(LOS_GENTYPES) -s $(srcdir) -u 'server/'
	echo >$@
