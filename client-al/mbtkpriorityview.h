/* mbtkpriorityview.h -
 */

#ifndef _MBTKPRIORITYVIEW_H_
#define _MBTKPRIORITYVIEW_H_

#include "client-al/mbtkbase.h"
#include "client-al/mbtkpriorityview-def.h"

G_BEGIN_DECLS



/* MbtkPriorityView:
 */
struct _MbtkPriorityView
{
  MBTK_PRIORITY_VIEW_INSTANCE_HEADER;

  gpointer private;
};



/* MbtkPriorityViewClass:
 */
struct _MbtkPriorityViewClass
{
  MBTK_PRIORITY_VIEW_CLASS_HEADER;
};



AltkWidget *mbtk_priority_view_new ( void );



G_END_DECLS

#endif /* ifndef _MBTKPRIORITYVIEW_H_ */
