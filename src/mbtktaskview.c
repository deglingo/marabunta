/* mbtktaskview.c -
 */

#include "mbtktaskview.h"
#include "mbtkpriorityview.h"
#include "mbtktaskview.inl"



/* Private:
 */
typedef struct _Private
{
  LSignalHandlerGroup *sig_group;
  MbTask *task;
  AltkWidget *title_label;
  AltkWidget *priority_view;
  AltkWidget *workers_label;
}
  Private;

#define PRIVATE(v) ((Private *)(MBTK_TASK_VIEW(v)->private))



/* mbtk_task_view_init:
 */
static void mbtk_task_view_init ( LObject *obj )
{
  MBTK_TASK_VIEW(obj)->private = g_new0(Private, 1);
  PRIVATE(obj)->sig_group = l_signal_handler_group_new();
}



/* _create_view:
 */
static void _create_view ( AltkWidget *view )
{
  Private *priv = PRIVATE(view);
  AltkWidget *box;
  box = L_TRASH_OBJECT
    (altk_box_new(ALTK_HORIZONTAL));
  ALTK_CONTAINER_ADD(view, box);
  priv->title_label = L_TRASH_OBJECT
    (altk_label_new("TASK"));
  ALTK_BOX_ADD(box, priv->title_label, 0);
  priv->priority_view = L_TRASH_OBJECT
    (mbtk_priority_view_new(NULL));
  ALTK_BOX_ADD(box, priv->priority_view, 0);
  priv->workers_label = L_TRASH_OBJECT
    (altk_label_new(""));
  ALTK_BOX_ADD(box, priv->workers_label, 0);
}



/* _on_workers_notify:
 */
static void _on_workers_notify ( MbTask *task,
                                 MbtkTaskView *view )
{
  Private *priv = PRIVATE(view);
  gchar text[MB_COUNT_MAX_CHARS+1];
  mb_count_print(task->workers, text);
  altk_label_set_text(ALTK_LABEL(priv->workers_label), text);
}



/* _set_task:
 */
static void _set_task ( MbtkTaskView *view,
                        MbTask *task )
{
  Private *priv = PRIVATE(view);
  if (task == priv->task)
    return;
  ASSERT(!priv->task); /* [todo] */
  if ((priv->task = task))
    {
      l_object_ref(priv->task);
      altk_label_set_text(ALTK_LABEL(priv->title_label), task->name);
      l_signal_handler_group_add
        (priv->sig_group,
         l_signal_connect(L_OBJECT(task),
                          "notify:workers",
                          (LSignalHandler) _on_workers_notify,
                          view,
                          NULL));
      _on_workers_notify(task, view);
      mbtk_priority_view_set_priority(MBTK_PRIORITY_VIEW(priv->priority_view),
                                      task->priority);
    }
}



/* mbtk_task_view_new:
 */
AltkWidget *mbtk_task_view_new ( MbTask *task )
{
  AltkWidget *view;
  view = ALTK_WIDGET(l_object_new(MBTK_CLASS_TASK_VIEW, NULL));
  l_trash_push();
  _create_view(view);
  l_trash_pop();
  _set_task(MBTK_TASK_VIEW(view), task);
  return view;
}
