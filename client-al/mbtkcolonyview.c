/* mbtkcolonyview.c -
 */

#include "client-al/alprivate.h"
#include "client-al/mbtkcolonyview.h"
#include "client-al/mbtkpriority.h"
#include "client-al/mbtktaskview.h"
#include "client-al/mbtkcolonyview.inl"



/* RoomInfo:
 */
typedef struct _RoomInfo
{
  gchar *name; /* [fixme] should ot be here */
  gchar *task_name;
  gfloat x;
  gfloat y;
  gfloat width;
  gfloat height;
}
  RoomInfo;

static const RoomInfo ROOM_INFO[MB_ROOM_TYPE_COUNT] =
  {
    [MB_ROOM_TYPE_ROYAL_CHAMBER] = { "Royal Chamber", "spawn", 0.5, 0.5, 0.1, 0.1 },
  };



/* RoomData:
 */
typedef struct _RoomData
{
  MbcProxy *room;
  MbcProxy *task;
  gint cx, cy; /* center */
  gint width, height;
  gint bx, by; /* building top-left */
  AltkWidget *task_view;
}
  RoomData;



/* Private:
 */
typedef struct _Private
{
  /* AltkWidget *title; */
  /* AltkWidget *task_table; */
  GList *tasks;
  MbcProxy *colony;
  AltkBitmap *backbuf;
  AltkGC *gc_backbuf;
  RoomData room_data[MB_ROOM_TYPE_COUNT];
}
  Private;

#define PRIVATE(obj) ((Private *)(MBTK_COLONY_VIEW(obj)->private))



static void size_request ( AltkWidget *widget,
                           AltkRequisition *req );
static void size_allocate ( AltkWidget *widget,
                            AltkAllocation *alloc );
static void forall ( AltkWidget *widget,
                      AltkForeachFunc func,
                      gpointer data );
static void expose_event ( AltkWidget *widget,
                           AltkEvent *event );



/* mbtk_colony_view_class_init:
 */
static void mbtk_colony_view_class_init ( LObjectClass *cls )
{
  ALTK_WIDGET_CLASS(cls)->size_request = size_request;
  ALTK_WIDGET_CLASS(cls)->size_allocate = size_allocate;
  ALTK_WIDGET_CLASS(cls)->forall = forall;
  ALTK_WIDGET_CLASS(cls)->expose_event = expose_event;
}



/* mbtk_colony_view_init:
 */
static void mbtk_colony_view_init ( LObject *obj )
{
  MBTK_COLONY_VIEW(obj)->private = g_new0(Private, 1);
  ALTK_WIDGET(obj)->flags |= ALTK_WIDGET_FLAG_NOWINDOW;
  altk_widget_set_event_mask(ALTK_WIDGET(obj), ALTK_EVENT_MASK_EXPOSE);
}



/* mbtk_colony_view_new:
 */
AltkWidget *mbtk_colony_view_new ( void )
{
  AltkWidget *view;
  Private *priv;
  l_trash_push();
  view = ALTK_WIDGET(MBTK_COLONY_VIEW_NEW(NULL));
  priv = PRIVATE(view);
  priv->gc_backbuf = altk_gc_new(NULL);
  /* priv->title = L_TRASH_OBJECT */
  /*   (altk_label_new("TITLE")); */
  /* altk_box_pack_start(ALTK_BOX(view), priv->title, 0); */
  /* priv->task_table = L_TRASH_OBJECT */
  /*   (altk_box_new(ALTK_VERTICAL)); */
  /* altk_box_pack_start(ALTK_BOX(view), priv->task_table, 0);   */
  l_trash_pop();
  return view;
}



/* forall:
 */
static void forall ( AltkWidget *widget,
                      AltkForeachFunc func,
                      gpointer data )
{
  Private *priv = PRIVATE(widget);
  gint tp;
  for (tp = 0; tp < MB_ROOM_TYPE_COUNT; tp++)
    {
      RoomData *room_data = &priv->room_data[tp];
      if (!room_data->room)
        continue;
      if (func(room_data->task_view, data) == ALTK_FOREACH_STOP)
        return;
    }
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
      RoomData *room_data = &priv->room_data[tp];
      AltkRequisition child_req;
      if (!room_data->room)
        continue;
      altk_widget_size_request(room_data->task_view, &child_req);
    }
  req->width = req->height = 8;
}



static void size_allocate ( AltkWidget *widget,
                            AltkAllocation *alloc )
{
  Private *priv = PRIVATE(widget);
  gint tp;
  ALTK_WIDGET_CLASS(parent_class)->size_allocate(widget, alloc);
  for (tp = 0; tp < MB_ROOM_TYPE_COUNT; tp++)
    {
      RoomData *room_data = &priv->room_data[tp];
      const RoomInfo *info = &ROOM_INFO[tp];
      AltkAllocation child_alloc;
      if (!room_data->room)
        continue;
      room_data->cx = alloc->width * info->x;
      room_data->cy = alloc->height * info->y;
      room_data->width = alloc->width * info->width;
      room_data->height = alloc->height * info->height;
      room_data->bx = room_data->cx - room_data->width / 2;
      room_data->by = room_data->cy - room_data->height / 2;
      child_alloc.x = alloc->x + (room_data->cx - room_data->task_view->size_request.width / 2);
      child_alloc.y = alloc->y + (room_data->cy + room_data->height / 2 + 4);
      child_alloc.width = room_data->task_view->size_request.width;
      child_alloc.height = room_data->task_view->size_request.height;
      altk_widget_size_allocate(room_data->task_view, &child_alloc);
    }
}



/* static void _on_task_workers_notify ( MbcProxy *task, */
/*                                       AltkWidget *label ) */
/* { */
/*   /\* [fixme] ?? *\/ */
/*   gchar text[32]; */
/*   sprintf(text, "%" G_GINT64_FORMAT, MBC_TASK_PROXY(task)->workers); */
/*   altk_label_set_text(ALTK_LABEL(label), text); */
/* } */



/* static void _create_task ( MbtkColonyView *view, */
/*                            MbcProxy *task ) */
/* { */
/*   Private *priv = PRIVATE(view); */
/*   AltkWidget *box, *label, *prio, *workers; */
/*   gchar *value; */
/*   GList *l; */
/*   box = L_TRASH_OBJECT */
/*     (altk_box_new(ALTK_HORIZONTAL)); */
/*   altk_box_pack_start(ALTK_BOX(priv->task_table), box, 0); */
/*   label = L_TRASH_OBJECT */
/*     (altk_label_new(MBC_TASK_PROXY(task)->name)); */
/*   altk_box_pack_start(ALTK_BOX(box), label, 0); */
/*   prio = L_TRASH_OBJECT */
/*     (mbtk_priority_new(MBC_TASK_PROXY(task)->priority)); */
/*   altk_box_pack_start(ALTK_BOX(box), prio, 0); */
/*   value = g_strdup_printf("%" G_GINT64_FORMAT, MBC_TASK_PROXY(task)->workers); */
/*   workers = L_TRASH_OBJECT */
/*     (altk_label_new(value)); */
/*   g_free(value); */
/*   altk_box_pack_start(ALTK_BOX(box), workers, 0); */
/*   l_signal_connect(L_OBJECT(task), */
/*                    "notify", g_quark_from_string("workers"), */
/*                    (LSignalHandler) _on_task_workers_notify, */
/*                    workers, */
/*                    NULL); */
/*   for (l = MBC_TASK_PROXY(task)->children; l; l = l->next) */
/*     { */
/*       _create_task(view, l->data); */
/*     } */
/* } */



static void _add_building ( MbtkColonyView *view,
                            MbcProxy *room )
{
  Private *priv = PRIVATE(view);
  const RoomInfo *info = &ROOM_INFO[MBC_ROOM_PROXY(room)->type];
  RoomData *data = &priv->room_data[MBC_ROOM_PROXY(room)->type];
  data->room = l_object_ref(room);
  data->task = mbc_colony_proxy_find_task(MBC_COLONY_PROXY(priv->colony),
                                          info->task_name);
  ASSERT(data->task);
  l_object_ref(data->task);
  data->task_view = mbtk_task_view_new(data->task);
  /* [FIXME] altk_container_add() ? */
  _altk_widget_set_parent(data->task_view, ALTK_WIDGET(view));
}



/* mbtk_colony_view_set_colony:
 */
void mbtk_colony_view_set_colony ( MbtkColonyView *view,
                                   MbcProxy *colony )
{
  Private *priv = PRIVATE(view);
  gint tp;
  /* gchar *title; */
  ASSERT(MBC_IS_COLONY_PROXY(colony));
  ASSERT(!priv->colony); /* [todo] */
  l_trash_push();
  priv->colony = l_object_ref(colony);
  /* add the buildings */
  for (tp = 0; tp < MB_ROOM_TYPE_COUNT; tp++)
    {
      if (MBC_COLONY_PROXY(colony)->rooms[tp])
        {
          _add_building(view, MBC_COLONY_PROXY(colony)->rooms[tp]);
        }
    }
  /* /\* set title *\/ */
  /* title = l_object_to_string(L_OBJECT(colony)); */
  /* altk_label_set_text(ALTK_LABEL(priv->title), title); */
  /* g_free(title); */
  /* /\* task list *\/ */
  /* _create_task(view, MBC_COLONY_PROXY(colony)->top_task); */
  l_trash_pop();
}



/* resize_backbuf:
 */
static void resize_backbuf ( MbtkColonyView *view )
{
  Private *priv = PRIVATE(view);
  gint w, h;
  if (!ALTK_WIDGET_REALIZED(view))
    return;
  if (priv->backbuf)
    {
      altk_drawable_get_size(ALTK_DRAWABLE(priv->backbuf), &w, &h);
      if (w < ALTK_WIDGET(view)->width ||
          h < ALTK_WIDGET(view)->height)
        {
          L_OBJECT_CLEAR(priv->backbuf);
          w = MAX(w, ALTK_WIDGET(view)->width);
          h = MAX(h, ALTK_WIDGET(view)->height);
        }
    }
  else
    {
      w = ALTK_WIDGET(view)->width;
      h = ALTK_WIDGET(view)->height;
    }
  if (!priv->backbuf)
    {
      priv->backbuf = ALTK_BITMAP
        (altk_bitmap_new
         (altk_widget_get_display(ALTK_WIDGET(view)),
          w, h));
      altk_gc_set_surface(priv->gc_backbuf,
                          ALTK_DRAWABLE(priv->backbuf));
    }
}



/* update_backbuf:
 */
static void update_backbuf ( MbtkColonyView *view )
{
  Private *priv = PRIVATE(view);
  gint tp;
  altk_gc_set_color_hrgb(priv->gc_backbuf, 0x00FF00);
  altk_gc_draw_rectangle(priv->gc_backbuf,
                         TRUE,
                         0,
                         0,
                         ALTK_WIDGET(view)->width,
                         ALTK_WIDGET(view)->height);
  altk_style_context_draw_frame(ALTK_WIDGET(view)->style_context,
                                priv->gc_backbuf,
                                0,
                                0,
                                ALTK_WIDGET(view)->width,
                                ALTK_WIDGET(view)->height);
  for (tp = 0; tp < MB_ROOM_TYPE_COUNT; tp++)
    {
      RoomData *room_data = &priv->room_data[tp];
      if (room_data->room)
        {
          /* const RoomInfo *info = &ROOM_INFO[tp]; */
          altk_gc_set_color_hrgb(priv->gc_backbuf, 0x0);
          altk_gc_draw_rectangle(priv->gc_backbuf,
                                 FALSE,
                                 room_data->bx,
                                 room_data->by,
                                 room_data->width,
                                 room_data->height);
        }
    }
}



/* expose_event:
 */
static void expose_event ( AltkWidget *widget,
                           AltkEvent *event )
{
  Private *priv = PRIVATE(widget);
  resize_backbuf(MBTK_COLONY_VIEW(widget));
  update_backbuf(MBTK_COLONY_VIEW(widget));
  altk_gc_draw_bitmap_region(event->expose.gc,
                             priv->backbuf,
                             event->expose.area,
                             0,
                             0);
  /* altk_gc_set_color_hrgb(event->expose.gc, 0x0000FF); */
  /* altk_gc_clear_region(event->expose.gc, */
  /*                      event->expose.area); */
}
