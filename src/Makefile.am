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
	mbbase.c \
	mbpoptype.c \
	mbgame.c \
	mbworld.c \
	mbsector.c \
	mbcolony.c \
	mbtkdialog.c \
	mbtkpoptable.c \
	mbtksectorview.c \
	mbpoptree.c \
	mbpriority.c \
	mbtask.c \
	mbtaskgroup.c \
	mbtaskspawn.c \
	mbmain.c

BUILT_SOURCES = \
	mbgame-def.h mbgame.inl \
	mbworld-def.h mbworld.inl \
	mbsector-def.h mbsector.inl \
	mbcolony-def.h mbcolony.inl \
	mbpriority-def.h mbpriority.inl \
	mbtask-def.h mbtask.inl \
	mbtaskgroup-def.h mbtaskgroup.inl \
	mbtaskspawn-def.h mbtaskspawn.inl \
	mbtkdialog-def.h mbtkdialog.inl \
	mbtkpoptable-def.h mbtkpoptable.inl \
	mbtksectorview-def.h mbtksectrview.inl

mbgame-def.h mbgame.inl \
mbworld-def.h mbworld.inl \
mbsector-def.h mbsector.inl \
mbcolony-def.h mbcolony.inl \
mbpriority-def.h mbpriority.inl \
mbtask-def.h mbtask.inl \
mbtaskgroup-def.h mbtaskgroup.inl \
mbtaskspawn-def.h mbtaskspawn.inl \
mbtkdialog-def.h mbtkdialog.inl \
mbtkpoptable-def.h mbtkpoptable.inl \
mbtksectorview-def.h mbtksectrview.inl \
: mbtypes.stamp-h

mbtypes.stamp-h: mbtypes.in
	cpp $< | $(LOS_GENTYPES) -s $(srcdir) -u ''
