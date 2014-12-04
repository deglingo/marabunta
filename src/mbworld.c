/* mbworld.c -
 */

#include "mbworld.h"
#include "mbsector.h"
#include "mbworld.inl"



/* mb_world_init:
 */
static void mb_world_init ( LObject *obj )
{
}



/* mb_world_new:
 */
MbWorld *mb_world_new ( guint width,
                        guint height )
{
  MbWorld *world;
  guint x, y;
  world = MB_WORLD(l_object_new(MB_CLASS_WORLD, NULL));
  world->width = width;
  world->height = height;
  world->sectors = g_new(MbSector **, height);
  for (y = 0; y < height; y++)
    {
      world->sectors[y] = g_new(MbSector *, width);
      for (x = 0; x < width; x++)
        {
          world->sectors[y][x] = mb_sector_new();
        }
    }
  return world;
}



/* mb_world_update:
 */
void mb_world_update ( MbWorld *world )
{
  guint x, y;
  for (y = 0; y < world->height; y++)
    {
      for (x = 0; x < world->width; x++)
        {
          mb_sector_update(world->sectors[y][x]);
        }
    }
}
