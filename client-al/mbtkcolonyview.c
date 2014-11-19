/* mbtkcolonyview.c -
 */

#include "client-al/alprivate.h"
#include "client-al/mbtkcolonyview.h"
#include "client-al/mbtkcolonyview.inl"



/* RoomInfo:
 */
typedef struct _RoomInfo
{
  gchar *name; /* [fixme] */
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
  gint cx, cy;
  gint x, y, w, h;
}
  Room;



/* Private:
 */
typedef struct _Private
{
  MbObject *colony;
  Room rooms[MB_ROOM_TYPE_COUNT];
}
  Private;

#define PRIVATE(view) ((Private *)(MBTK_COLONY_VIEW(view)->private))



static const RoomInfo ROOM_INFO[MB_ROOM_TYPE_COUNT] =
  {
    [MB_ROOM_TYPE_ROYAL_CHAMBER] = { "Royal Chamber", 0.5, 0.5, 0.1, 0.1 },
  };



static void size_request ( AltkWidget *widget,
                           AltkRequisition *req );
static void size_allocate ( AltkWidget *widget,
                            AltkAllocation *alloc );
static void expose_event ( AltkWidget *widget,
                           AltkEvent *event );



/* mbtk_colony_view_class_init:
 */
static void mbtk_colony_view_class_init ( LObjectClass *cls )
{
  ALTK_WIDGET_CLASS(cls)->size_request = size_request;
  ALTK_WIDGET_CLASS(cls)->size_allocate = size_allocate;
  ALTK_WIDGET_CLASS(cls)->expose_event = expose_event;
}



/* mbtk_colony_view_init:
 */
static void mbtk_colony_view_init ( LObject *obj )
{
  MBTK_COLONY_VIEW(obj)->private = g_new0(Private, 1);
  ALTK_WIDGET(obj)->flags |= ALTK_WIDGET_FLAG_NOWINDOW;
  altk_widget_set_event_mask(ALTK_WIDGET(obj),
                             ALTK_EVENT_MASK_EXPOSE);
}



/* mbtk_colony_view_new:
 */
AltkWidget *mbtk_colony_view_new ( void )
{
  AltkWidget *view;
  view = ALTK_WIDGET(l_object_new(MBTK_CLASS_COLONY_VIEW, NULL));
  return view;
}



/* _rooms_layout:
 */
static void _rooms_layout ( MbtkColonyView *view )
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
    }
}



/* mbtk_colony_view_set_colony:
 */
void mbtk_colony_view_set_colony ( MbtkColonyView *view,
                                   MbObject *colony )
{
  Private *priv = PRIVATE(view);
  gint tp;
  ASSERT(MB_IS_COLONY(colony));
  ASSERT(!priv->colony); /* [todo] */
  priv->colony = l_object_ref(colony);
  for (tp = 0; tp < MB_ROOM_TYPE_COUNT; tp++)
    {
      MbObject *mb_room = MB_COLONY_ROOM(colony, tp);
      Room *room;
      if (!mb_room)
        continue;
      room = &priv->rooms[tp];
      room->room = l_object_ref(mb_room);
    }
  _rooms_layout(view);
}



/* size_request:
 */
static void size_request ( AltkWidget *widget,
                           AltkRequisition *req )
{
  req->width = 8;
  req->height = 8;
}



/* size_allocate:
 */
static void size_allocate ( AltkWidget *widget,
                            AltkAllocation *alloc )
{
  ALTK_WIDGET_CLASS(parent_class)->size_allocate(widget, alloc);
  _rooms_layout(MBTK_COLONY_VIEW(widget));
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
