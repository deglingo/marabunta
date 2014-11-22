/* mbtksectorview.h -
 */

#ifndef _MBTKSECTORVIEW_H_
#define _MBTKSECTORVIEW_H_

#include "client-al/mbtkbase.h"
#include "client-al/mbtksectorview-def.h"

G_BEGIN_DECLS



/* MbtkSectorView:
 */
struct _MbtkSectorView
{
  MBTK_SECTOR_VIEW_INSTANCE_HEADER;
  gpointer private;
};



/* MbtkSectorViewClass:
 */
struct _MbtkSectorViewClass
{
  MBTK_SECTOR_VIEW_CLASS_HEADER;
};



AltkWidget *mbtk_sector_view_new ( void );
void mbtk_sector_view_set_sector ( MbtkSectorView *view,
                                   MbObject *sector );



G_END_DECLS

#endif /* ifndef _MBTKSECTORVIEW_H_ */
