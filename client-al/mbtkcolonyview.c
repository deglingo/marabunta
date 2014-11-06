/* mbtkcolonyview.c -
 */

#include "client-al/mbtkcolonyview.h"
#include "client-al/mbtkcolonyview.inl"



/* mbtk_colony_view_init:
 */
static void mbtk_colony_view_init ( LObject *obj )
{
}



/* mbtk_colony_view_new:
 */
AltkWidget *mbtk_colony_view_new ( void )
{
  AltkWidget *view;
  view = ALTK_WIDGET(MBTK_COLONY_VIEW_NEW(NULL));
  return view;
}
