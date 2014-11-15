#

lib_LTLIBRARIES = libmarabunta-client.la

libmarabunta_client_la_CPPFLAGS = -I$(top_srcdir) -I$(top_builddir) -DCL_LOG_DOMAIN=\"mbclient\"

libmarabunta_client_la_CFLAGS = @GLIB_CFLAGS@ @CLOG_CFLAGS@

libmarabunta_client_la_SOURCES = \
	mbcbase.c \
	mbcproxy.c \
	mbcgameproxy.c \
	mbcworldproxy.c \
	mbcsectorproxy.c \
	mbccolonyproxy.c \
	mbcpriorityproxy.c \
	mbctaskproxy.c \
	mbcroomproxy.c \
	mbcresourceproxy.c \
	mbcapp.c \
	mbcclient.c

BUILT_SOURCES = \
	mbcproxy-def.h mbcproxy.inl \
	mbcgameproxy-def.h mbcgameproxy.inl \
	mbcworldproxy-def.h mbcworldproxy.inl \
	mbcsectorproxy-def.h mbcsectorproxy.inl \
	mbccolonyproxy-def.h mbccolonyproxy.inl \
	mbcpriorityproxy-def.h mbcpriorityproxy.inl \
	mbctaskproxy-def.h mbctaskproxy.inl \
	mbcroomproxy-def.h mbcroomproxy.inl \
	mbcresourceproxy-def.h mbcresourceproxy.inl \
	mbcapp-def.h mbcapp.inl

mbcproxy-def.h mbcproxy.inl \
mbcgameproxy-def.h mbcgameproxy.inl \
mbcworldproxy-def.h mbcworldproxy.inl \
mbcsectorproxy-def.h mbcsectorproxy.inl \
mbccolonyproxy-def.h mbccolonyproxy.inl \
mbcpriorityproxy-def.h mbcpriorityproxy.inl \
mbctaskproxy-def.h mbctaskproxy.inl \
mbcroomproxy-def.h mbcroomproxy.inl \
mbcresourceproxy-def.h mbcresourceproxy.inl \
mbcapp-def.h mbcapp.inl \
: mbctypes.stamp-h

mbctypes.stamp-h: mbctypes.in
	cpp $< | $(LOS_GENTYPES) -s $(srcdir) -u 'client/'
	echo >$@
