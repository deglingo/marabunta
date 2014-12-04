/* mbworld.c -
 */

#include "mbworld.h"
#include "mbsector.h"
#include "mbworld.inl"



/* Private:
 */
typedef struct _Private
{
  guint width;
  guint height;
  MbSector ***sectors;
}
  Private;

#define PRIVATE(w) ((Private *)(MB_WORLD(w)->private))



/* mb_world_init:
 */
static void mb_world_init ( LObject *obj )
{
  MB_WORLD(obj)->private = g_new0(Private, 1);
}



/* mb_world_new:
 */
MbWorld *mb_world_new ( guint width,
                        guint height )
{
  MbWorld *world;
  Private *priv;
  guint x, y;
  world = MB_WORLD(l_object_new(MB_CLASS_WORLD, NULL));
  priv = PRIVATE(world);
  priv->width = width;
  priv->height = height;
  priv->sectors = g_new(MbSector **, height);
  for (y = 0; y < height; y++)
    {
      priv->sectors[y] = g_new(MbSector *, width);
      for (x = 0; x < width; x++)
        {
          priv->sectors[y][x] = mb_sector_new();
        }
    }
  return world;
}
