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
};



/* MbtkMapViewClass:
 */
struct _MbtkMapViewClass
{
  MBTK_MAP_VIEW_CLASS_HEADER;
};



AltkWidget *mbtk_map_view_new ( void );



G_END_DECLS

#endif /* ifndef _MBTKMAPVIEW_H_ */
