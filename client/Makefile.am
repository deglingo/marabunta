#

lib_LTLIBRARIES = libmarabunta-client.la

libmarabunta_client_la_CPPFLAGS = -I$(top_srcdir) -I$(top_builddir) -DCL_LOG_DOMAIN=\"mbclient\"

libmarabunta_client_la_CFLAGS = @GLIB_CFLAGS@ @CLOG_CFLAGS@

libmarabunta_client_la_SOURCES = \
	mbcbase.c \
	mbcapp.c

BUILT_SOURCES = \
	mbcapp-def.h mbcapp.inl

mbcapp-def.h mbcapp.inl \
: mbctypes.stamp-h

mbctypes.stamp-h: mbctypes.in
	cpp $< | $(LOS_GENTYPES) -s $(srcdir) -u 'client/'
	echo >$@
