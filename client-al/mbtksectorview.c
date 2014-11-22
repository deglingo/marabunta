/* mbtksectorview.c -
 */

#include "client-al/alprivate.h"
#include "client-al/mbtksectorview.h"
#include "client-al/mbtktaskview.h"
#include "client-al/mbtksectorview.inl"



/* RoomInfo:
 */
typedef struct _RoomInfo
{
  gchar *name; /* [fixme] */
  gchar *task_path;
  gfloat x;
  gfloat y;
  gfloat w;
  gfloat h;
}
  RoomInfo;



/* Room:
 */
typedef struct _Room
{
  MbObject *room;
  MbObject *task;
  gint cx, cy;
  gint x, y, w, h;
  AltkWidget *task_view;
  AltkRequisition task_req;
  AltkAllocation task_alloc;
}
  Room;



/* Private:
 */
typedef struct _Private
{
  MbObject *sector;
  Room rooms[MB_ROOM_TYPE_COUNT];
}
  Private;

#define PRIVATE(view) ((Private *)(MBTK_SECTOR_VIEW(view)->private))



static const RoomInfo ROOM_INFO[MB_ROOM_TYPE_COUNT] =
  {
    [MB_ROOM_TYPE_ROYAL_CHAMBER] = { "Royal Chamber", "spawn",      0.5, 0.5, 0.1, 0.1 },
    [MB_ROOM_TYPE_FARM]          = { "Farm",          "work/farm",  0.15, 0.5, 0.1, 0.1 },
    [MB_ROOM_TYPE_MINE]          = { "Mine",          "work/mine",  0.5, 0.25, 0.1, 0.1 },
  };



static void size_request ( AltkWidget *widget,
                           AltkRequisition *req );
static void size_allocate ( AltkWidget *widget,
                            AltkAllocation *alloc );
static void expose_event ( AltkWidget *widget,
                           AltkEvent *event );



/* mbtk_sector_view_class_init:
 */
static void mbtk_sector_view_class_init ( LObjectClass *cls )
{
  ALTK_WIDGET_CLASS(cls)->size_request = size_request;
  ALTK_WIDGET_CLASS(cls)->size_allocate = size_allocate;
  ALTK_WIDGET_CLASS(cls)->expose_event = expose_event;
}



/* mbtk_sector_view_init:
 */
static void mbtk_sector_view_init ( LObject *obj )
{
  MBTK_SECTOR_VIEW(obj)->private = g_new0(Private, 1);
  ALTK_WIDGET(obj)->flags |= ALTK_WIDGET_FLAG_NOWINDOW;
  altk_widget_set_event_mask(ALTK_WIDGET(obj),
                             ALTK_EVENT_MASK_EXPOSE);
}



/* mbtk_sector_view_new:
 */
AltkWidget *mbtk_sector_view_new ( void )
{
  AltkWidget *view;
  view = ALTK_WIDGET(l_object_new(MBTK_CLASS_SECTOR_VIEW, NULL));
  return view;
}



/* _rooms_layout:
 */
static void _rooms_layout ( MbtkSectorView *view )
{
  Private *priv = PRIVATE(view);
  gint tp;
  gint width = ALTK_WIDGET(view)->width;
  gint height = ALTK_WIDGET(view)->height;
  for (tp = 0; tp < MB_ROOM_TYPE_COUNT; tp++)
    {
      Room *room = &priv->rooms[tp];
      const RoomInfo *info;
      if (!room->room)
        continue;
      info = &ROOM_INFO[tp];
      room->cx = width * info->x;
      room->cy = height * info->y;
      room->w = width * info->w;
      room->h = height * info->h;
      room->x = room->cx - room->w / 2;
      room->y = room->cy - room->h / 2;
      if (room->task_view)
        {
          room->task_alloc.width = room->task_req.width;
          room->task_alloc.height = room->task_req.height;
          room->task_alloc.x = ALTK_WIDGET(view)->x + room->cx - room->task_alloc.width / 2;
          room->task_alloc.y = ALTK_WIDGET(view)->y + room->y + room->h + 2;
        }
    }
}



/* mbtk_sector_view_set_sector:
 */
void mbtk_sector_view_set_sector ( MbtkSectorView *view,
                                   MbObject *sector )
{
  Private *priv = PRIVATE(view);
  gint tp;
  ASSERT(MB_IS_SECTOR(sector));
  ASSERT(!priv->sector); /* [todo] */
  priv->sector = l_object_ref(sector);
  if (MB_SECTOR_COLONY(sector))
    {
      MbObject *colony = MB_SECTOR_COLONY(sector);
      for (tp = 0; tp < MB_ROOM_TYPE_COUNT; tp++)
        {
          MbObject *mb_room = MB_COLONY_ROOM(colony, tp);
          Room *room;
          const RoomInfo *info;
          if (!mb_room)
            continue;
          room = &priv->rooms[tp];
          info = &ROOM_INFO[tp];
          room->room = l_object_ref(mb_room);
          if (info->task_path)
            {
              room->task = mb_task_find(MB_TASK(MB_COLONY_TOP_TASK(colony)),
                                        info->task_path);
              ASSERT(room->task);
              l_object_ref(room->task);
              room->task_view = mbtk_task_view_new(ALTK_VERTICAL, room->task);
              _altk_widget_set_parent(room->task_view, ALTK_WIDGET(view));
              mbtk_task_view_hide_title(MBTK_TASK_VIEW(room->task_view));
              altk_widget_show_all(room->task_view);
            }
        }
    }
  _rooms_layout(view);
}



/* size_request:
 */
static void size_request ( AltkWidget *widget,
                           AltkRequisition *req )
{
  Private *priv = PRIVATE(widget);
  gint tp;
  for (tp = 0; tp < MB_ROOM_TYPE_COUNT; tp++)
    {
      Room *room = &priv->rooms[tp];
      if (room->task_view)
        altk_widget_size_request(room->task_view, &room->task_req);
    }
  req->width = 8;
  req->height = 8;
}



/* size_allocate:
 */
static void size_allocate ( AltkWidget *widget,
                            AltkAllocation *alloc )
{
  Private *priv = PRIVATE(widget);
  gint tp;
  ALTK_WIDGET_CLASS(parent_class)->size_allocate(widget, alloc);
  _rooms_layout(MBTK_SECTOR_VIEW(widget));
  for (tp = 0; tp < MB_ROOM_TYPE_COUNT; tp++)
    {
      Room *room = &priv->rooms[tp];
      if (room->task_view)
        altk_widget_size_allocate(room->task_view, &room->task_alloc);
    }
}



/* expose_event:
 */
static void expose_event ( AltkWidget *widget,
                           AltkEvent *event )
{
  Private *priv = PRIVATE(widget);
  gint tp;
  altk_gc_set_color_hrgb(event->expose.gc,
                         0x00FF00);
  altk_gc_clear_region(event->expose.gc,
                       event->expose.area);
  for (tp = 0; tp < MB_ROOM_TYPE_COUNT; tp++)
    {
      Room *room = &priv->rooms[tp];
      if (!room->room)
        continue;
      altk_gc_set_color_hrgb(event->expose.gc, 0x0);
      altk_gc_draw_rectangle(event->expose.gc,
                             FALSE,
                             room->x,
                             room->y,
                             room->w,
                             room->h);
    }
}
