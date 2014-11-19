#

lib_LTLIBRARIES = libmarabunta-common.la

libmarabunta_common_la_CFLAGS = -DCL_LOG_DOMAIN=\"mbcommon\" -I$(top_srcdir) -I$(top_builddir) @LOS_CFLAGS@

# libmarabunta_common_la_LIBADD = @LPTREE_LIBS@

libmarabunta_common_la_SOURCES = \
	mbbase.c \
	mbapp.c \
	mbobject.c \
	mbgame.c \
	mbplayer.c \
	mbworld.c \
	mbsector.c \
	mbtask.c \
	mbcolony.c \
	mbmessage.c \
	mbstate.c \
	mbpriority.c \
	mbmessagewriter.c \
	mbmessagereader.c \
	mbwatch.c \
	mbpoptype.c \
	mbresource.c

BUILT_SOURCES = \
	mbapp-def.h mbapp.inl \
	mbobject-def.h mbobject.inl \
	mbgame-def.h mbgame.inl \
	mbplayer-def.h mbplayer.inl \
	mbworld-def.h mbworld.inl \
	mbsector-def.h mbsector.inl \
	mbtask-def.h mbtask.inl \
	mbcolony-def.h mbcolony.inl \
	mbmessage-def.h mbmessage.inl
	mbstate-def.h mbstate.inl \
	mbpriority-def.h mbpriority.inl

mbapp-def.h mbapp.inl \
mbobject-def.h mbobject.inl \
mbgame-def.h mbgame.inl \
mbplayer-def.h mbplayer.inl \
mbworld-def.h mbworld.inl \
mbsector-def.h mbsector.inl \
mbtask-def.h mbtask.inl \
mbcolony-def.h mbcolony.inl \
mbmessage-def.h mbmessage.inl \
mbstate-def.h mbstate.inl \
mbpriority-def.h mbpriority.inl \
: mbtypes.stamp-h

mbtypes.stamp-h: mbtypes.in
	cpp $< | $(LOS_GENTYPES) -s $(srcdir) -u 'common/'
	echo >$@
