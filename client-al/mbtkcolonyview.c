/* mbtkcolonyview.c -
 */

#include "client-al/alprivate.h"
#include "client-al/mbtkcolonyview.h"
#include "client-al/mbtkpriority.h"
#include "client-al/mbtkcolonyview.inl"



/* Private:
 */
typedef struct _Private
{
  AltkWidget *title;
  AltkWidget *task_table;
  GList *tasks;
  MbcProxy *colony;
}
  Private;

#define PRIVATE(obj) ((Private *)(MBTK_COLONY_VIEW(obj)->private))



/* mbtk_colony_view_init:
 */
static void mbtk_colony_view_init ( LObject *obj )
{
  MBTK_COLONY_VIEW(obj)->private = g_new0(Private, 1);
}



/* mbtk_colony_view_new:
 */
AltkWidget *mbtk_colony_view_new ( void )
{
  AltkWidget *view, *box;
  Private *priv;
  l_trash_push();
  view = ALTK_WIDGET(MBTK_COLONY_VIEW_NEW(NULL));
  priv = PRIVATE(view);
  box = L_TRASH_OBJECT
    (altk_box_new(ALTK_VERTICAL));
  ALTK_CONTAINER_ADD(view, box);
  priv->title = L_TRASH_OBJECT
    (altk_label_new("TITLE"));
  altk_box_pack_start(ALTK_BOX(box), priv->title, 0);
  priv->task_table = L_TRASH_OBJECT
    (altk_box_new(ALTK_VERTICAL));
  altk_box_pack_start(ALTK_BOX(box), priv->task_table, 0);  
  l_trash_pop();
  return view;
}



static void _on_task_workers_notify ( MbcProxy *task,
                                      AltkWidget *label )
{
  /* [fixme] ?? */
  gchar text[32];
  sprintf(text, "%" G_GINT64_FORMAT, MBC_TASK_PROXY(task)->workers);
  altk_label_set_text(ALTK_LABEL(label), text);
}



static void _create_task ( MbtkColonyView *view,
                           MbcProxy *task )
{
  Private *priv = PRIVATE(view);
  AltkWidget *box, *label, *prio, *workers;
  gchar *value;
  GList *l;
  box = L_TRASH_OBJECT
    (altk_box_new(ALTK_HORIZONTAL));
  altk_box_pack_start(ALTK_BOX(priv->task_table), box, 0);
  label = L_TRASH_OBJECT
    (altk_label_new(MBC_TASK_PROXY(task)->name));
  altk_box_pack_start(ALTK_BOX(box), label, 0);
  prio = L_TRASH_OBJECT
    (mbtk_priority_new(MBC_TASK_PROXY(task)->priority));
  altk_box_pack_start(ALTK_BOX(box), prio, 0);
  value = g_strdup_printf("%" G_GINT64_FORMAT, MBC_TASK_PROXY(task)->workers);
  workers = L_TRASH_OBJECT
    (altk_label_new(value));
  g_free(value);
  altk_box_pack_start(ALTK_BOX(box), workers, 0);
  l_signal_connect(L_OBJECT(task),
                   "notify", g_quark_from_string("workers"),
                   (LSignalHandler) _on_task_workers_notify,
                   workers,
                   NULL);
  for (l = MBC_TASK_PROXY(task)->children; l; l = l->next)
    {
      _create_task(view, l->data);
    }
}



/* mbtk_colony_view_set_colony:
 */
void mbtk_colony_view_set_colony ( MbtkColonyView *view,
                                   MbcProxy *colony )
{
  Private *priv = PRIVATE(view);
  gchar *title;
  ASSERT(MBC_IS_COLONY_PROXY(colony));
  ASSERT(!priv->colony); /* [todo] */
  l_trash_push();
  priv->colony = l_object_ref(colony);
  /* set title */
  title = l_object_to_string(L_OBJECT(colony));
  altk_label_set_text(ALTK_LABEL(priv->title), title);
  g_free(title);
  /* task list */
  _create_task(view, MBC_COLONY_PROXY(colony)->top_task);
  l_trash_pop();
}
