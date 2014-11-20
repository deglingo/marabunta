#

bin_PROGRAMS = marabunta-al

marabunta_al_CPPFLAGS = -I$(top_srcdir) -I$(top_builddir) -DCL_LOG_DOMAIN=\"mbclient-al\"

marabunta_al_CFLAGS = @GLIB_CFLAGS@ @CLOG_CFLAGS@ @ALTK_CFLAGS@

marabunta_al_LDFLAGS = \
	$(top_builddir)/common/libmarabunta-common.la \
	$(top_builddir)/server/libmarabunta-server.la \
	$(top_builddir)/client/libmarabunta-client.la \
	@GLIB_LIBS@ @CLOG_LIBS@ @ALTK_LIBS@

marabunta_al_SOURCES = \
	mbtkdialog.c \
	mbtkmapview.c \
	mbtkpoptable.c \
	mbtkcolonyview.c \
	mbtktaskview.c \
	mbalapp.c \
	mbalmain.c

BUILT_SOURCES = \
	mbtkdialog-def.h mbtkdialog.inl \
	mbtkmapview-def.h mbtkmapview.inl \
	mbtkpoptable-def.h mbtkpoptable.inl \
	mbtkcolonyview-def.h mbtkcolonyview.inl \
	mbtktaskview-def.h mbtktaskview.inl \
	mbalapp-def.h mbalapp.inl

mbtkdialog-def.h mbtkdialog.inl \
mbtkmapview-def.h mbtkmapview.inl \
mbtkpoptable-def.h mbtkpoptable.inl \
mbtkcolonyview-def.h mbtkcolonyview.inl \
mbtktaskview-def.h mbtktaskview.inl \
mbalapp-def.h mbalapp.inl \
: mbaltypes.stamp-h

mbaltypes.stamp-h: mbaltypes.in
	cpp $< | $(LOS_GENTYPES) -s $(srcdir) -u 'client-al/'
	echo >$@
