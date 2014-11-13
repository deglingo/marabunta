/* mbtkcolonyview.c -
 */

#include "client-al/alprivate.h"
#include "client-al/mbtkcolonyview.h"
#include "client-al/mbtkpriority.h"
#include "client-al/mbtkcolonyview.inl"



/* RoomInfo:
 */
typedef struct _RoomInfo
{
  gchar *name; /* [fixme] should ot be here */
  gfloat x;
  gfloat y;
}
  RoomInfo;

static const RoomInfo ROOM_INFO[MB_ROOM_TYPE_COUNT] =
  {
    [MB_ROOM_TYPE_ROYAL_CHAMBER] = { "Royal Chamber", 0.5, 0.5 },
  };



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
}
  Private;

#define PRIVATE(obj) ((Private *)(MBTK_COLONY_VIEW(obj)->private))



static void size_request ( AltkWidget *widget,
                           AltkRequisition *req );
static void foreach ( AltkWidget *widget,
                      AltkForeachFunc func,
                      gpointer data );
static void expose_event ( AltkWidget *widget,
                           AltkEvent *event );



/* mbtk_colony_view_class_init:
 */
static void mbtk_colony_view_class_init ( LObjectClass *cls )
{
  ALTK_WIDGET_CLASS(cls)->size_request = size_request;
  ALTK_WIDGET_CLASS(cls)->foreach = foreach;
  ALTK_WIDGET_CLASS(cls)->expose_event = expose_event;
}



/* mbtk_colony_view_init:
 */
static void mbtk_colony_view_init ( LObject *obj )
{
  MBTK_COLONY_VIEW(obj)->private = g_new0(Private, 1);
  ALTK_WIDGET(obj)->flags = ALTK_WIDGET_FLAG_NOWINDOW;
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



/* size_request:
 */
static void size_request ( AltkWidget *widget,
                           AltkRequisition *req )
{
  req->width = req->height = 8;
}



/* foreach:
 */
static void foreach ( AltkWidget *widget,
                      AltkForeachFunc func,
                      gpointer data )
{
  /* CL_DEBUG("[TODO]"); */
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
  /* Private *priv = PRIVATE(view); */
  /* RoomInfo *info = ROOM_INFO[MBC_ROOM_PROXY(room)->type]; */
  /* MbcProxy *task; */
  /* task = mbc_colony_proxy_find_task(MBC_COLONY_PROXY(priv->colony), */
  /*                                   info->task_name); */
  /* task_view = mbtk_task_view_new(task); */
  /* ALTK_CONTAINER_ADD(view, task_view); */
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
      MbcProxy *room  = MBC_COLONY_PROXY(priv->colony)->rooms[tp];
      if (room)
        {
          const RoomInfo *info = &ROOM_INFO[tp];
          gint x = (gint) (ALTK_WIDGET(view)->width * info->x);
          gint y = (gint) (ALTK_WIDGET(view)->height * info->y);
          gint w = ALTK_WIDGET(view)->width * 0.1;
          gint h = ALTK_WIDGET(view)->height * 0.1;
          altk_gc_set_color_hrgb(priv->gc_backbuf, 0x0);
          altk_gc_draw_rectangle(priv->gc_backbuf,
                                 FALSE,
                                 x - w/2,
                                 y - h/2,
                                 w,
                                 h);
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
