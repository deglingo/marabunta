/* mbtkinfopanel.c -
 */

#include "client-al/mbtkinfopanel.h"
#include "client-al/mbtkinfopanel.inl"



/* mbtk_info_panel_new:
 */
AltkWidget *mbtk_info_panel_new ( void )
{
  AltkWidget *panel;
  panel = ALTK_WIDGET(MBTK_INFO_PANEL_NEW(NULL));
  return panel;
}
