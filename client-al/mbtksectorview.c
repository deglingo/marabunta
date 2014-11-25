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
  GList *rooms;
}
  Private;

#define PRIVATE(view) ((Private *)(MBTK_SECTOR_VIEW(view)->private))



static void size_request ( AltkWidget *widget,
                           AltkRequisition *req );
static void size_allocate ( AltkWidget *widget,
                            AltkAllocation *alloc );
static void expose_event ( AltkWidget *widget,
                           AltkEvent *event );
static void _remove ( AltkContainer *cont,
                      AltkWidget *child );



/* room_new:
 */
static Room *room_new ( void )
{
  return g_new0(Room, 1);
}



/* room_free:
 */
static void room_free ( Room *room )
{
  g_free(room);
}



/* mbtk_sector_view_class_init:
 */
static void mbtk_sector_view_class_init ( LObjectClass *cls )
{
  ALTK_WIDGET_CLASS(cls)->size_request = size_request;
  ALTK_WIDGET_CLASS(cls)->size_allocate = size_allocate;
  ALTK_WIDGET_CLASS(cls)->expose_event = expose_event;
  ALTK_CONTAINER_CLASS(cls)->remove = _remove;
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



/* _remove:
 */
static void _remove ( AltkContainer *cont,
                      AltkWidget *child )
{
  /* [FIXME] the ref is dropped by _unset_sector() */
  _altk_widget_unset_parent(child);
}



/* _rooms_layout:
 */
static void _rooms_layout ( MbtkSectorView *view )
{
  Private *priv = PRIVATE(view);
  GList *l;
  gint width = ALTK_WIDGET(view)->width;
  gint height = ALTK_WIDGET(view)->height;
  CL_TRACE("%p", view);
  for (l = priv->rooms; l; l = l->next)
    {
      Room *room = l->data;
      room->cx = width * MB_ROOM_X(room->room);
      room->cy = height * MB_ROOM_Y(room->room);
      room->w = width * MB_ROOM_WIDTH(room->room);
      room->h = height * MB_ROOM_HEIGHT(room->room);
      room->x = room->cx - room->w / 2;
      room->y = room->cy - room->h / 2;
      CL_TRACE("room: %d, %d, %d, %d",
               room->x, room->y,
               room->w, room->h);
      if (room->task_view)
        {
          room->task_alloc.width = room->task_req.width;
          room->task_alloc.height = room->task_req.height;
          room->task_alloc.x = ALTK_WIDGET(view)->x + room->cx - room->task_alloc.width / 2;
          room->task_alloc.y = ALTK_WIDGET(view)->y + room->y + room->h + 2;
        }
    }
}



/* _unset_sector:
 */
static void _unset_sector ( MbtkSectorView *view )
{
  Private *priv = PRIVATE(view);
  GList *l;
  /* cleanup rooms */
  for (l = priv->rooms; l; l = l->next)
    {
      Room *room = l->data;
      if (room->room)
        L_OBJECT_CLEAR(room->room);
      if (room->task)
        L_OBJECT_CLEAR(room->task);
      if (room->task_view) {
        altk_widget_destroy(room->task_view);
        L_OBJECT_CLEAR(room->task_view);
      }
      room_free(room);
    }
  g_list_free(priv->rooms);
  priv->rooms = NULL;
  L_OBJECT_CLEAR(priv->sector);
}



/* mbtk_sector_view_set_sector:
 */
void mbtk_sector_view_set_sector ( MbtkSectorView *view,
                                   MbObject *sector )
{
  Private *priv = PRIVATE(view);
  if (sector == priv->sector)
    return;
  if (priv->sector)
    {
      _unset_sector(view);
    }
  if (sector)
    {
      ASSERT(MB_IS_SECTOR(sector));
      /* CL_DEBUG("set_sector: %p", sector); */
      priv->sector = l_object_ref(sector);
      if (MB_SECTOR_COLONY(sector))
        {
          MbObject *colony = MB_SECTOR_COLONY(sector);
          GList *l;
          /* CL_DEBUG("set_colony: %p", colony); */
          for (l = MB_COLONY_ROOMS(colony); l; l = l->next)
            {
              MbObject *mb_room = l->data;
              Room *room = room_new();
              priv->rooms = g_list_append(priv->rooms, room);
              /* CL_DEBUG("set room %d: %p", tp, mb_room); */
              room->room = l_object_ref(mb_room);
              if (MB_ROOM_WORK_TASK(room->room))
                {
                  room->task = l_object_ref(MB_ROOM_WORK_TASK(room->room));
                  room->task_view = mbtk_task_view_new(ALTK_VERTICAL, room->task);
                  _altk_widget_set_parent(room->task_view, ALTK_WIDGET(view));
                  mbtk_task_view_hide_title(MBTK_TASK_VIEW(room->task_view));
                  altk_widget_show_all(room->task_view);
                }
            }
        }
    }
  /* _rooms_layout(view); */
  altk_widget_queue_resize(ALTK_WIDGET(view));
  altk_widget_queue_draw(ALTK_WIDGET(view));
}



/* size_request:
 */
static void size_request ( AltkWidget *widget,
                           AltkRequisition *req )
{
  Private *priv = PRIVATE(widget);
  GList *l;
  for (l = priv->rooms; l; l = l->next)
    {
      Room *room = l->data;
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
  GList *l;
  ALTK_WIDGET_CLASS(parent_class)->size_allocate(widget, alloc);
  _rooms_layout(MBTK_SECTOR_VIEW(widget));
  for (l = priv->rooms; l; l = l->next)
    {
      Room *room = l->data;
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
  GList *l;
  altk_gc_set_color_hrgb(event->expose.gc,
                         0x00FF00);
  altk_gc_clear_region(event->expose.gc,
                       event->expose.area);
  for (l = priv->rooms; l; l = l->next)
    {
      Room *room = l->data;
      altk_gc_set_color_hrgb(event->expose.gc, 0x0);
      altk_gc_draw_rectangle(event->expose.gc,
                             FALSE,
                             room->x,
                             room->y,
                             room->w,
                             room->h);
    }
}
