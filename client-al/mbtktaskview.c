/* mbtktaskview.c -
 */

#include "client-al/alprivate.h"
#include "client-al/mbtktaskview.h"
#include "client-al/mbtkpriorityview.h"
#include "client-al/mbtktaskview.inl"



/* Private:
 */
typedef struct _Private
{
  LSignalHandlerGroup *sig_group;
  AltkOrientation orientation;
  MbObject *task;
  AltkWidget *title;
  AltkWidget *priority_view;
  AltkWidget *workers;
  AltkWidget *resource;
}
  Private;

#define PRIVATE(view) ((Private *)(MBTK_TASK_VIEW(view)->private))



static void _dispose ( LObject *object );



/* mbtk_task_view_class_init:
 */
static void mbtk_task_view_class_init ( LObjectClass *cls )
{
  cls->dispose = _dispose;
}



/* mbtk_task_view_init:
 */
static void mbtk_task_view_init ( LObject *obj )
{
  MBTK_TASK_VIEW(obj)->private = g_new0(Private, 1);
  PRIVATE(obj)->sig_group = l_signal_handler_group_new();
}



/* _dispose:
 */
static void _dispose ( LObject *object )
{
  l_signal_handler_group_remove_all(PRIVATE(object)->sig_group);
  if (PRIVATE(object)->task)
    L_OBJECT_CLEAR(PRIVATE(object)->task);
  L_OBJECT_CLASS(parent_class)->dispose(object);
}



/* _create_view:
 */
static void _create_view ( AltkWidget *view )
{
  Private *priv = PRIVATE(view);
  AltkWidget *top_box, *box;
  l_trash_push();
  /* top_box */
  top_box = L_TRASH_OBJECT
    (altk_box_new(ALTK_VERTICAL));
  ALTK_CONTAINER_ADD(view, top_box);
  /* box */
  box = L_TRASH_OBJECT
    (altk_box_new(priv->orientation));
  ALTK_BOX_ADD(top_box, box, ALTK_PACK_EXPAND_FILL);
  /* title label */
  priv->title = L_TRASH_OBJECT
    (altk_label_new(""));
  ALTK_BOX_ADD(box, priv->title, 0);
  /* priority */
  priv->priority_view = L_TRASH_OBJECT
    (mbtk_priority_view_new());
  ALTK_BOX_ADD(box, priv->priority_view, 0);
  /* workers label */
  priv->workers = L_TRASH_OBJECT
    (altk_label_new("0"));
  ALTK_BOX_ADD(box, priv->workers, 0);
  /* resource label */
  priv->resource = L_TRASH_OBJECT
    (altk_label_new("0"));
  ALTK_BOX_ADD(top_box, priv->resource, 0);
  altk_widget_set_enable_show_all(priv->resource, FALSE);
  altk_widget_hide(priv->resource);
  l_trash_pop();
}



/* _on_workers_notify:
 */
static void _on_workers_notify ( MbObject *task,
                                 AltkWidget *view )
{
  Private *priv = PRIVATE(view);
  gchar text[MB_COUNT_CHARS+1];
  mb_count_print(MB_TASK_WORKERS(task), text);
  altk_label_set_text(ALTK_LABEL(priv->workers), text);
}



/* _on_stock_notify:
 */
static void _on_stock_notify ( MbObject *colony,
                               AltkWidget *view )
{
  Private *priv = PRIVATE(view);
  gint64 stock;
  stock = mb_colony_get_stock(MB_COLONY(colony),
                              MB_OBJECT_ID(MB_TASK_RESOURCE(priv->task)));
  gchar text[MB_COUNT_CHARS+1];
  mb_count_print(stock, text);
  altk_label_set_text(ALTK_LABEL(priv->resource), text);
}



/* _set_task:
 */
static void _set_task ( AltkWidget *view,
                        MbObject *task )
{
  Private *priv = PRIVATE(view);
  ASSERT(MB_IS_TASK(task));
  ASSERT(!priv->task); /* [todo] */
  priv->task = l_object_ref(task);
  altk_label_set_text(ALTK_LABEL(priv->title), MB_TASK_NAME(task));
  l_signal_handler_group_connect
    (priv->sig_group,
     L_OBJECT(task),
     "notify:workers",
     (LSignalHandler) _on_workers_notify,
     view, NULL);
  if (!MB_TASK_RESOURCE(task))
    {
      altk_widget_set_enable_show_all(priv->resource, FALSE);
      altk_widget_hide(priv->resource);
    }
  else
    {
      gchar *signame;
      altk_widget_set_enable_show_all(priv->resource, TRUE);
      altk_widget_show(priv->resource);
      signame = g_strdup_printf("stock_notify:%s", MB_RESOURCE_NAME(MB_TASK_RESOURCE(task)));
      l_signal_handler_group_connect
        (priv->sig_group,
         L_OBJECT(MB_TASK_COLONY(task)),
         signame,
         (LSignalHandler) _on_stock_notify,
         view,
         NULL);
      g_free(signame);
    }
  mbtk_priority_view_set_priority(MBTK_PRIORITY_VIEW(PRIVATE(view)->priority_view),
                                  MB_TASK_PRIORITY(PRIVATE(view)->task));
}



/* mbtk_task_view_new:
 */
AltkWidget *mbtk_task_view_new ( AltkOrientation orientation,
                                 MbObject *task )
{
  AltkWidget *view = ALTK_WIDGET(l_object_new(MBTK_CLASS_TASK_VIEW, NULL));
  Private *priv = PRIVATE(view);
  priv->orientation = orientation;
  _create_view(view);
  if (task)
    _set_task(view, task);
  return view;
}



/* mbtk_task_view_hide_title:
 */
void mbtk_task_view_hide_title ( MbtkTaskView *view )
{
  Private *priv = PRIVATE(view);
  altk_widget_set_enable_show_all(priv->title, FALSE);
  altk_widget_hide(priv->title);
}
