#

lib_LTLIBRARIES = libmarabunta-common.la

libmarabunta_common_la_CFLAGS = -DCL_LOG_DOMAIN=\"mbcommon\" -I$(top_srcdir) -I$(top_builddir) @LOS_CFLAGS@

# libmarabunta_common_la_LIBADD = @LPTREE_LIBS@

libmarabunta_common_la_SOURCES = \
	mbbase.c \
	mbapp.c \
	mbmessage.c \
	mbmessagewriter.c \
	mbmessagereader.c \
	mbwatch.c \
	mbworld.c

BUILT_SOURCES = \
	mbapp-def.h mbapp.inl \
	mbmessage-def.h mbmessage.inl
	mbworld-def.h mbworld.inl

mbapp-def.h mbapp.inl \
mbmessage-def.h mbmessage.inl \
mbworld-def.h mbworld.inl \
: mbtypes.stamp-h

mbtypes.stamp-h: mbtypes.in
	cpp $< | $(LOS_GENTYPES) -s $(srcdir) -u 'common/'
	echo >$@
