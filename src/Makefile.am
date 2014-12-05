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
	mbpoptype.c \
	mbgame.c \
	mbworld.c \
	mbsector.c \
	mbcolony.c \
	mbtkdialog.c \
	mbtkpoptable.c \
	mbpoptree.c \
	mbtask.c \
	mbtaskgroup.c \
	mbmain.c

BUILT_SOURCES = \
	mbgame-def.h mbgame.inl \
	mbworld-def.h mbworld.inl \
	mbsector-def.h mbsector.inl \
	mbcolony-def.h mbcolony.inl \
	mbtask-def.h mbtask.inl \
	mbtaskgroup-def.h mbtaskgroup.inl \
	mbtkdialog-def.h mbtkdialog.inl \
	mbtkpoptable-def.h mbtkpoptable.inl

mbgame-def.h mbgame.inl \
mbworld-def.h mbworld.inl \
mbsector-def.h mbsector.inl \
mbcolony-def.h mbcolony.inl \
mbtask-def.h mbtask.inl \
mbtaskgroup-def.h mbtaskgroup.inl \
mbtkdialog-def.h mbtkdialog.inl \
mbtkpoptable-def.h mbtkpoptable.inl \
: mbtypes.stamp-h

mbtypes.stamp-h: mbtypes.in
	cpp $< | $(LOS_GENTYPES) -s $(srcdir) -u ''
