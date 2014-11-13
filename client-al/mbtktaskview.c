/* mbtktaskview.c -
 */

#include "client-al/mbtktaskview.h"
#include "client-al/mbtkpriority.h"
#include "client-al/mbtktaskview.inl"



/* mbtk_task_view_new:
 */
AltkWidget *mbtk_task_view_new ( MbcProxy *task )
{
  MbtkTaskView *view = MBTK_TASK_VIEW_NEW(NULL);
  AltkWidget *top_box, *box;
  l_trash_push();
  view->task = l_object_ref(task);
  /* top box */
  top_box = L_TRASH_OBJECT
    (altk_box_new(ALTK_VERTICAL));
  ALTK_CONTAINER_ADD(view, top_box);
  /* title */
  view->title = L_TRASH_OBJECT
    (altk_label_new(MBC_TASK_PROXY(task)->name));
  ALTK_BOX_ADD(top_box, view->title, 0);
  /* box */
  box = L_TRASH_OBJECT
    (altk_box_new(ALTK_HORIZONTAL));
  ALTK_BOX_ADD(top_box, box, 0);
  /* /\* priority *\/ */
  /* view->priority = L_TRASH_OBJECT */
  /*   (mbtk_priority_new(MBC_TASK_PROXY(task)->priority)); */
  /* ALTK_BOX_ADD(box, view->priority, 0); */
  /* counter */
  view->count_label = L_TRASH_OBJECT
    (altk_label_new("0"));
  ALTK_BOX_ADD(box, view->count_label, 0);
  l_trash_pop();
  return ALTK_WIDGET(view);
}
