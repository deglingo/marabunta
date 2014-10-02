#

bin_PROGRAMS = marabunta-al

marabunta_al_CPPFLAGS = -I$(top_srcdir) -I$(top_builddir) -DCL_LOG_DOMAIN=\"mbclient-al\"

marabunta_al_CFLAGS = @GLIB_CFLAGS@ @CLOG_CFLAGS@ @ALTK_CFLAGS@

marabunta_al_LDFLAGS = \
	$(top_builddir)/common/libmarabunta-common.la \
	$(top_builddir)/client/libmarabunta-client.la \
	@GLIB_LIBS@ @CLOG_LIBS@ @ALTK_LIBS@ @LPTREE_LIBS@

marabunta_al_SOURCES = \
	mbalapp.c \
	mbalmain.c

BUILT_SOURCES = \
	mbalapp-def.h mbalapp.inl

mbalapp-def.h mbalapp.inl \
: mbaltypes.stamp-h

mbaltypes.stamp-h: mbaltypes.in
	cpp $< | $(LOS_GENTYPES)
	echo >$@
