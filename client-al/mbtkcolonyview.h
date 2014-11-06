/* mbtkcolonyview.h -
 */

#ifndef _MBTKCOLONYVIEW_H_
#define _MBTKCOLONYVIEW_H_

#include "client-al/mbtkbase.h"
#include "client-al/mbtkcolonyview-def.h"

G_BEGIN_DECLS



/* MbtkColonyView:
 */
struct _MbtkColonyView
{
  MBTK_COLONY_VIEW_INSTANCE_HEADER;

  gpointer private;
};



/* MbtkColonyViewClass:
 */
struct _MbtkColonyViewClass
{
  MBTK_COLONY_VIEW_CLASS_HEADER;
};



AltkWidget *mbtk_colony_view_new ( void );
void mbtk_colony_view_set_colony ( MbtkColonyView *view,
                                   MbcProxy *colony );



G_END_DECLS

#endif /* ifndef _MBTKCOLONYVIEW_H_ */
