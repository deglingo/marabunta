/* mbtkmapview.c -
 */

#include "client-al/mbtkmapview.h"
#include "client-al/mbtkmapview.inl"



/* mbtk_map_view_new:
 */
AltkWidget *mbtk_map_view_new ( void )
{
  AltkWidget *view;
  view = ALTK_WIDGET(l_object_new(MBTK_CLASS_MAP_VIEW, NULL));
  return view;
}
