/* mbtktaskview.h -
 */

#ifndef _MBTKTASKVIEW_H_
#define _MBTKTASKVIEW_H_

#include "mbtkbase.h"
#include "mbtktaskview-def.h"

G_BEGIN_DECLS



/* MbtkTaskView:
 */
struct _MbtkTaskView
{
  MBTK_TASK_VIEW_INSTANCE_HEADER;
};



/* MbtkTaskViewClass:
 */
struct _MbtkTaskViewClass
{
  MBTK_TASK_VIEW_CLASS_HEADER;
};



AltkWidget *mbtk_task_view_new ( MbTask *task );



G_END_DECLS

#endif /* ifndef _MBTKTASKVIEW_H_ */
