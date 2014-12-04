#

bin_PROGRAMS = marabunta-new

marabunta_new_CFLAGS = \
	-DCL_LOG_DOMAIN=\"marabunta\" \
	@CLOG_CFLAGS@ \
	@LOS_CFLAGS@ \
	@ALTK_CFLAGS@

marabunta_new_LDFLAGS = \
	@CLOG_LIBS@ \
	@LOS_LIBS@ \
	@ALTK_LIBS@

marabunta_new_SOURCES = \
	mbtkdialog.c \
	mbmain.c

BUILT_SOURCES = \
	mbtkdialog-def.h mbtkdialog.inl

mbtkdialog-def.h mbtkdialog.inl \
: mbtypes.stamp-h

mbtypes.stamp-h: mbtypes.in
	cpp $< | $(LOS_GENTYPES) -s $(srcdir) -u ''
