/* mbtktaskview.h -
 */

#ifndef _MBTKTASKVIEW_H_
#define _MBTKTASKVIEW_H_

#include "client-al/mbtkbase.h"
#include "client-al/mbtktaskview-def.h"

G_BEGIN_DECLS



/* MbtkTaskView:
 */
struct _MbtkTaskView
{
  MBTK_TASK_VIEW_INSTANCE_HEADER;

  MbcProxy *task;

  AltkWidget *title;
  AltkWidget *priority;
  AltkWidget *count_label;
};



/* MbtkTaskViewClass:
 */
struct _MbtkTaskViewClass
{
  MBTK_TASK_VIEW_CLASS_HEADER;
};



AltkWidget *mbtk_task_view_new ( MbcProxy *task );



G_END_DECLS

#endif /* ifndef _MBTKTASKVIEW_H_ */
