#

bin_PROGRAMS = marabunta-al

marabunta_al_CPPFLAGS = -I$(top_srcdir) -DCL_LOG_DOMAIN=\"mbclient-al\"

marabunta_al_CFLAGS = @GLIB_CFLAGS@ @CLOG_CFLAGS@ @ALTK_CFLAGS@

marabunta_al_LDFLAGS = \
	$(top_builddir)/common/libmarabunta-common.la \
	$(top_builddir)/client/libmarabunta-client.la \
	@GLIB_LIBS@ @CLOG_LIBS@ @ALTK_LIBS@

marabunta_al_SOURCES = \
	mbalmain.c
