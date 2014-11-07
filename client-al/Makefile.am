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
	mbtkpriority.c \
	mbtkmapview.c \
	mbtkpoptable.c \
	mbtkcolonyview.c \
	mbaldialog-main.c \
	mbaldialog.c \
	mbalapp.c \
	mbalmain.c

BUILT_SOURCES = \
	mbtkdialog-def.h mbtkdialog.inl \
	mbtkpriority-def.h mbtkpriority.inl \
	mbtkmapview-def.h mbtkmapview.inl \
	mbtkpoptable-def.h mbtkpoptable.inl \
	mbtkcolonyview-def.h mbtkcolonyview.inl \
	mbaldialog-main.c \
	mbalapp-def.h mbalapp.inl

mbaldialog-main.c: mbaldialog-main.xml
	$(ALTK_XML2C) -n DIALOG_MAIN -o mbaldialog-main $<

mbtkdialog-def.h mbtkdialog.inl \
mbtkpriority-def.h mbtkpriority.inl \
mbtkmapview-def.h mbtkmapview.inl \
mbtkpoptable-def.h mbtkpoptable.inl \
mbtkcolonyview-def.h mbtkcolonyview.inl \
mbalapp-def.h mbalapp.inl \
: mbaltypes.stamp-h

mbaltypes.stamp-h: mbaltypes.in
	cpp $< | $(LOS_GENTYPES) -s $(srcdir) -u 'client-al/'
	echo >$@
