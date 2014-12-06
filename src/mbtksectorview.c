/* mbtksectorview.c -
 */

#include "mbtksectorview.h"
#include "mbtksectorview.inl"



#define PRIORITY_UPDATE 600
#define FPS 10.0



/* RoomData:
 */
typedef struct _RoomData
{
  MbRoom *mb_room;
  gint cx;
  gint cy;
  gint x;
  gint y;
  gint width;
  gint height;
}
  RoomData;



/* Private:
 */
typedef struct _Private
{
  MbSector *sector;
  AltkDrawable *backbuf;
  AltkGC *gc;
  RoomData *rooms;
  gint n_rooms;
  gint max_rooms;
}
  Private;

#define PRIVATE(view) ((Private *)(MBTK_SECTOR_VIEW(view)->private))



static void _realize ( AltkWidget *widget );
static void _unrealize ( AltkWidget *widget );
static void _size_request ( AltkWidget *widget,
                            AltkRequisition *req );
static void _size_allocate ( AltkWidget *widget,
                             AltkAllocation *alloc );
static void _expose_event ( AltkWidget *widget,
                            AltkEvent *event );



/* mbtk_sector_view_class_init:
 */
static void mbtk_sector_view_class_init ( LObjectClass *cls )
{
  ALTK_WIDGET_CLASS(cls)->realize = _realize;
  ALTK_WIDGET_CLASS(cls)->unrealize = _unrealize;
  ALTK_WIDGET_CLASS(cls)->size_request = _size_request;
  ALTK_WIDGET_CLASS(cls)->size_allocate = _size_allocate;
  ALTK_WIDGET_CLASS(cls)->expose_event = _expose_event;
}



/* mbtk_sector_view_init:
 */
static void mbtk_sector_view_init ( LObject *obj )
{
  MBTK_SECTOR_VIEW(obj)->private = g_new0(Private, 1);
  ALTK_WIDGET(obj)->flags |= ALTK_WIDGET_FLAG_NOWINDOW;
  altk_widget_set_event_mask(ALTK_WIDGET(obj), ALTK_EVENT_MASK_EXPOSE);
  PRIVATE(obj)->gc = altk_gc_new(NULL);
}



/* mbtk_sector_view_new:
 */
AltkWidget *mbtk_sector_view_new ( void )
{
  AltkWidget *view;
  view = ALTK_WIDGET(l_object_new(MBTK_CLASS_SECTOR_VIEW, NULL));
  return view;
}



/* _layout:
 */
static void _layout ( MbtkSectorView *view )
{
  Private *priv = PRIVATE(view);
  gint n;
  MbGame *game;
  if (!priv->sector)
    return;
  game = priv->sector->world->game;
  for (n = 0; n < priv->n_rooms; n++)
    {
      RoomData *data = &priv->rooms[n];
      MbRoomType type = data->mb_room->type;
      const MbRoomTypeInfo *type_info = mb_game_room_type_info(game, type);
      ASSERT(type_info);
      data->cx = type_info->x * ALTK_WIDGET(view)->width;
      data->cy = type_info->y * ALTK_WIDGET(view)->height;
      data->width = type_info->width * ALTK_WIDGET(view)->width;
      data->height = type_info->height * ALTK_WIDGET(view)->height;
      data->x = data->cx - data->width / 2;
      data->y = data->cy - data->height / 2;
      CL_DEBUG("room %d: %d, %d, %d, %d", type, data->x, data->y, data->width, data->height);
    }
}



/* _set_sector:
 */
static void _set_sector ( MbtkSectorView *view,
                          MbSector *sector )
{
  Private *priv = PRIVATE(view);
  priv->sector = l_object_ref(sector);
  if (sector->colony)
    {
      GList *l;
      for (l = sector->colony->rooms; l; l = l->next)
        {
          RoomData *data;
          ASSERT(priv->n_rooms <= priv->max_rooms);
          if (priv->n_rooms == priv->max_rooms)
            {
              priv->max_rooms += 8;
              priv->rooms = g_realloc(priv->rooms, sizeof(RoomData) * priv->max_rooms);
            }
          data = &priv->rooms[priv->n_rooms++];
          data->mb_room = l_object_ref(l->data);
        }
    }
  if (ALTK_WIDGET_REALIZED(view))
    {
      _layout(view);
      altk_widget_queue_draw(ALTK_WIDGET(view));
    }
}



/* mbtk_sector_view_set_sector:
 */
void mbtk_sector_view_set_sector ( MbtkSectorView *view,
                                   MbSector *sector )
{
  Private *priv = PRIVATE(view);
  if (sector == priv->sector)
    return;
  ASSERT(!priv->sector); /* [TODO] */
  _set_sector(view, sector);
}



/* _grow_backbuf:
 */
static void _grow_backbuf ( MbtkSectorView *view )
{
  Private *priv = PRIVATE(view);
  ASSERT(ALTK_WIDGET_REALIZED(view));
  if (priv->backbuf)
    {
      gint w, h;
      altk_drawable_get_size(priv->backbuf, &w, &h);
      if (w != ALTK_WIDGET(view)->width || h != ALTK_WIDGET(view)->height)
        L_OBJECT_CLEAR(priv->backbuf);
    }
  if (!priv->backbuf)
    {
      priv->backbuf = altk_bitmap_new(altk_widget_get_display(ALTK_WIDGET(view)),
                                      ALTK_WIDGET(view)->width,
                                      ALTK_WIDGET(view)->height);
      altk_gc_set_surface(priv->gc, priv->backbuf);
    }
}



/* _update_timer:
 */
static gboolean _update_timer ( AltkWidget *view )
{
  Private *priv = PRIVATE(view);
  gint n;
  altk_gc_set_color_hrgb(priv->gc, 0x00FF00);
  altk_gc_draw_rectangle(priv->gc, TRUE, 0, 0, view->width, view->height);
  altk_gc_set_color_hrgb(priv->gc, 0);
  for (n = 0; n < priv->n_rooms; n++)
    {
      RoomData *data = &priv->rooms[n];
      altk_gc_draw_rectangle(priv->gc,
                             FALSE,
                             data->x,
                             data->y,
                             data->width,
                             data->height);
    }
  altk_widget_queue_draw(view);
  return G_SOURCE_CONTINUE;
}



/* _realize:
 */
static void _realize ( AltkWidget *widget )
{
  ALTK_WIDGET_CLASS(parent_class)->realize(widget);
  _grow_backbuf(MBTK_SECTOR_VIEW(widget));
  _layout(MBTK_SECTOR_VIEW(widget));
  g_timeout_add_full(PRIORITY_UPDATE,
                     1000.0 / FPS,
                     (GSourceFunc) _update_timer,
                     widget,
                     NULL);
}



/* _unrealize:
 */
static void _unrealize ( AltkWidget *widget )
{
  ALTK_WIDGET_CLASS(parent_class)->unrealize(widget);
  CL_DEBUG("[TODO] unrealize");
}



/* _size_request:
 */
static void _size_request ( AltkWidget *widget,
                            AltkRequisition *req )
{
  req->width = req->height = 8;
}



/* _size_allocate:
 */
static void _size_allocate ( AltkWidget *widget,
                             AltkAllocation *alloc )
{
  ALTK_WIDGET_CLASS(parent_class)->size_allocate(widget, alloc);
  _grow_backbuf(MBTK_SECTOR_VIEW(widget));
  _layout(MBTK_SECTOR_VIEW(widget));
}



/* _expose_event:
 */
static void _expose_event ( AltkWidget *widget,
                            AltkEvent *event )
{
  Private *priv = PRIVATE(widget);
  altk_gc_draw_bitmap_region(event->expose.gc,
                             ALTK_BITMAP(priv->backbuf),
                             event->expose.area,
                             0,
                             0);
}
