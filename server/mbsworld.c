/* mbsworld.c -
 */

#include "server/mbsworld.h"
#include "server/mbsworld.inl"



/* mbs_world_new:
 */
MbsWorld *mbs_world_new ( guint width,
                          guint height )
{
  MbsWorld *world;
  guint x, y;
  world = MBS_WORLD(l_object_new(MBS_CLASS_WORLD, NULL));
  world->width = width;
  world->height = height;
  world->sectors = g_new(MbsSector **, height);
  for (y = 0; y < height; y++)
    {
      world->sectors[y] = g_new(MbsSector *, width);
      for (x = 0; x < width; x++)
        {
          world->sectors[y][x] = mbs_sector_new(x, y);
        }
    }
  return world;
}
