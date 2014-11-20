/* mbtktaskview.c -
 */

#include "client-al/alprivate.h"
#include "client-al/mbtktaskview.h"
#include "client-al/mbtktaskview.inl"



/* Private:
 */
typedef struct _Private
{
  MbObject *task;
  AltkWidget *title;
  AltkWidget *workers;
}
  Private;

#define PRIVATE(view) ((Private *)(MBTK_TASK_VIEW(view)->private))



/* mbtk_task_view_init:
 */
static void mbtk_task_view_init ( LObject *obj )
{
  MBTK_TASK_VIEW(obj)->private = g_new0(Private, 1);
}



/* _create_view:
 */
static void _create_view ( AltkWidget *view )
{
  Private *priv = PRIVATE(view);
  AltkWidget *top_box;
  l_trash_push();
  /* top box */
  top_box = L_TRASH_OBJECT
    (altk_box_new(ALTK_VERTICAL));
  ALTK_CONTAINER_ADD(view, top_box);
  /* title label */
  priv->title = L_TRASH_OBJECT
    (altk_label_new(""));
  ALTK_BOX_ADD(top_box, priv->title, 0);
  /* workers label */
  priv->workers = L_TRASH_OBJECT
    (altk_label_new("0"));
  ALTK_BOX_ADD(top_box, priv->workers, 0);
  l_trash_pop();
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
}



/* mbtk_task_view_new:
 */
AltkWidget *mbtk_task_view_new ( MbObject *task )
{
  AltkWidget *view = ALTK_WIDGET(l_object_new(MBTK_CLASS_TASK_VIEW, NULL));
  _create_view(view);
  _set_task(view, task);
  return view;
}
