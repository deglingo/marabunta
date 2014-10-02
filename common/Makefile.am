#

lib_LTLIBRARIES = libmarabunta-common.la

libmarabunta_common_la_CFLAGS = -I$(top_srcdir) -I$(top_builddir) @LOS_CFLAGS@

libmarabunta_common_la_LIBADD = @LPTREE_LIBS@

libmarabunta_common_la_SOURCES = \
	mbbase.c \
	mbmessage.c \
	mbmessagewriter.c \
	mbmessagereader.c \
	mbwatch.c

BUILT_SOURCES = \
	mbmessage-def.h mbmessage.inl

mbmessage-def.h mbmessage.inl \
: mbtypes.stamp-h

mbtypes.stamp-h: mbtypes.in
	cpp $< | $(LOS_GENTYPES)
	echo >$@
