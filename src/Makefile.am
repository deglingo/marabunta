#

noinst_SCRIPTS = memcheck

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
	mbroom.c \
	mbtkdialog.c \
	mbtkpoptable.c \
	mbtkinfopanel.c \
	mbtkinfopage.c \
	mbtkroomspage.c \
	mbtkfoodpage.c \
	mbtktaskview.c \
	mbtkpriorityview.c \
	mbtksectorview.c \
	mbpoptree.c \
	mbpriority.c \
	mbtask.c \
	mbmain.c

BUILT_SOURCES = \
	mbgame-def.h mbgame.inl \
	mbworld-def.h mbworld.inl \
	mbsector-def.h mbsector.inl \
	mbcolony-def.h mbcolony.inl \
	mbroom-def.h mbroom.inl \
	mbpriority-def.h mbpriority.inl \
	mbtask-def.h mbtask.inl \
	mbtkdialog-def.h mbtkdialog.inl \
	mbtkpoptable-def.h mbtkpoptable.inl \
	mbtkinfopanel-def.h mbtkinfopanel.inl \
	mbtkinfopage-def.h mbtkinfopage.inl \
	mbtkroomspage-def.h mbtkroomspage.inl \
	mbtkfoodpage-def.h mbtkfoodpage.inl \
	mbtktaskview-def.h mbtktaskview.inl \
	mbtkpriorityview-def.h mbtkpriorityview.inl \
	mbtksectorview-def.h mbtksectrview.inl

mbgame-def.h mbgame.inl \
mbworld-def.h mbworld.inl \
mbsector-def.h mbsector.inl \
mbcolony-def.h mbcolony.inl \
mbroom-def.h mbroom.inl \
mbpriority-def.h mbpriority.inl \
mbtask-def.h mbtask.inl \
mbtkdialog-def.h mbtkdialog.inl \
mbtkpoptable-def.h mbtkpoptable.inl \
mbtkinfopanel-def.h mbtkinfopanel.inl \
mbtkinfopage-def.h mbtkinfopage.inl \
mbtkroomspage-def.h mbtkroomspage.inl \
mbtkfoodpage-def.h mbtkfoodpage.inl \
mbtktaskview-def.h mbtktaskview.inl \
mbtkpriorityview-def.h mbtkpriorityview.inl \
mbtksectorview-def.h mbtksectrview.inl \
: mbtypes.stamp-h

mbtypes.stamp-h: mbtypes.in
	cpp $< | $(LOS_GENTYPES) -s $(srcdir) -u ''
