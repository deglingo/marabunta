/* mbtkmapview.c -
 */

#include "client-al/mbtkmapview.h"
#include "client-al/mbtkmapview.inl"



/* mbtk_map_view_new:
 */
AltkWidget *mbtk_map_view_new ( void )
{
  AltkWidget *view;
  gint x, y;
  view = ALTK_WIDGET(l_object_new(MBTK_CLASS_MAP_VIEW, NULL));
  /* [temp] */
  for (y = 0; y < 2; y++)
    {
      for (x = 0; x < 2; x++)
        {
          AltkWidget *cell = altk_button_new_with_label("X");
          altk_table_attach(ALTK_TABLE(view), cell,
                            y, x, y+1, x+1, 0);
          l_object_unref(cell);
        }
    }
  return view;
}



/* mbtk_map_view_setup:
 */
void mbtk_map_view_setup ( MbtkMapView *view,
                           MbcGameProxy *game )
{
}
