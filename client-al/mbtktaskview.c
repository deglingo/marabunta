/* mbtktaskview.c -
 */

#include "client-al/mbtktaskview.h"
#include "client-al/mbtktaskview.inl"



/* mbtk_task_view_new:
 */
AltkWidget *mbtk_task_view_new ( MbObject *task )
{
  return ALTK_WIDGET(l_object_new(MBTK_CLASS_TASK_VIEW, NULL));
}
