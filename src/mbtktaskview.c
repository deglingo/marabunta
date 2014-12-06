/* mbtktaskview.c -
 */

#include "mbtktaskview.h"
#include "mbtktaskview.inl"



/* _create_view:
 */
static void _create_view ( AltkWidget *view )
{
  AltkWidget *title;
  title = L_TRASH_OBJECT
    (altk_label_new("TASK"));
  ALTK_CONTAINER_ADD(view, title);
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
  return view;
}
