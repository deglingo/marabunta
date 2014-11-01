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



/* mbtk_map_view_setup:
 */
void mbtk_map_view_setup ( MbtkMapView *view,
                           MbcGameProxy *game )
{
  gint x, y;
  /* [fixme] remove old ones */
  for (y = 0; y < game->world->height; y++)
    {
      for (x = 0; x < game->world->width; x++)
        {
          AltkWidget *cell = altk_button_new_with_label("X");
          altk_table_attach(ALTK_TABLE(view), cell,
                            y, x, y+1, x+1, 0);
          l_object_unref(cell);
        }
    }
}
