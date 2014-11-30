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

  gpointer private;
};



/* MbtkTaskViewClass:
 */
struct _MbtkTaskViewClass
{
  MBTK_TASK_VIEW_CLASS_HEADER;
};



AltkWidget *mbtk_task_view_new ( AltkOrientation orientation,
                                 MbObject *task );
void mbtk_task_view_hide_title ( MbtkTaskView *view );
void mbtk_task_view_set_task ( MbtkTaskView *view,
                               MbObject *task );



G_END_DECLS

#endif /* ifndef _MBTKTASKVIEW_H_ */
