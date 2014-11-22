/* mbtkmapview.h -
 */

#ifndef _MBTKMAPVIEW_H_
#define _MBTKMAPVIEW_H_

#include "client-al/mbtkbase.h"
#include "client-al/mbtkmapview-def.h"

G_BEGIN_DECLS



/* MbtkMapView:
 */
struct _MbtkMapView
{
  MBTK_MAP_VIEW_INSTANCE_HEADER;

  gpointer private;
};



/* MbtkMapViewClass:
 */
struct _MbtkMapViewClass
{
  MBTK_MAP_VIEW_CLASS_HEADER;
};



AltkWidget *mbtk_map_view_new ( void );
void mbtk_map_view_set_world ( MbtkMapView *view,
                               MbObject *world );
void mbtk_map_view_set_sector ( MbtkMapView *view,
                                MbObject *sector );



G_END_DECLS

#endif /* ifndef _MBTKMAPVIEW_H_ */
