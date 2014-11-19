/* mbtkmapview.c -
 */

#include "client-al/alprivate.h"
#include "client-al/mbtkmapview.h"
#include "client-al/mbtkmapview.inl"



/* Sector:
 */
typedef struct _Sector
{
  MbObject *mb_sector;
  AltkWidget *button;
}
  Sector;



/* Private:
 */
typedef struct _Private
{
  MbObject *world;
  AltkWidget *table;
  Sector ***sectors;
}
  Private;

#define PRIVATE(view) ((Private *)(MBTK_MAP_VIEW(view)->private))



/* mbtk_map_view_init:
 */
static void mbtk_map_view_init ( LObject *obj )
{
  MBTK_MAP_VIEW(obj)->private = g_new0(Private, 1);
}



/* mbtk_map_view_new:
 */
AltkWidget *mbtk_map_view_new ( void )
{
  Private *priv;
  AltkWidget *view;
  view = ALTK_WIDGET(l_object_new(MBTK_CLASS_MAP_VIEW, NULL));
  priv = PRIVATE(view);
  priv->table = altk_table_new();
  ALTK_CONTAINER_ADD(view, priv->table);
  l_object_unref(priv->table);
  return view;
}



/* mbtk_map_view_set_world:
 */
void mbtk_map_view_set_world ( MbtkMapView *view,
                               MbObject *world )
{
  Private *priv = PRIVATE(view);
  guint x, y;
  ASSERT(MB_IS_WORLD(world));
  ASSERT(!priv->world); /* [todo] */
  CL_DEBUG("set_world: %dx%d", MB_WORLD_WIDTH(world), MB_WORLD_HEIGHT(world));
  priv->world = l_object_ref(world);
  priv->sectors = g_new(Sector **, MB_WORLD_HEIGHT(world));
  for (y = 0; y < MB_WORLD_HEIGHT(world); y++)
    {
      priv->sectors[y] = g_new(Sector *, MB_WORLD_WIDTH(world));
      for (x = 0; x < MB_WORLD_WIDTH(world); x++)
        {
          Sector *sector;
          priv->sectors[y][x] = sector = g_new0(Sector, 1);
          sector->mb_sector = MB_WORLD_SECTOR(world, x, y);
          sector->button = altk_button_new_with_label("X");
          altk_table_attach(ALTK_TABLE(priv->table),
                            sector->button,
                            y, x, y+1, x+1,
                            0);
          l_object_unref(sector->button);
        }
    }
  altk_widget_show_all(ALTK_WIDGET(view));
}
