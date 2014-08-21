#

bin_PROGRAMS = marabunta-al

marabunta_al_CPPFLAGS = -I$(top_srcdir) -DG_LOG_DOMAIN=\"mbclient-al\"

marabunta_al_CFLAGS = @GLIB_CFLAGS@ @AL5_CFLAGS@

marabunta_al_LDFLAGS = $(top_builddir)/client/libmarabunta-client.la @GLIB_LIBS@ @AL5_LIBS@

marabunta_al_SOURCES = \
	mbalmain.c
