/* mbsworld.c -
 */

#include "server/mbsworld.h"
#include "server/mbssector.h"
#include "server/mbsworld.inl"



/* mbs_world_new:
 */
MbObject *mbs_world_new ( MbObject *game,
                          guint width,
                          guint height )
{
  return MB_OBJECT(l_object_new_give(MBS_CLASS_WORLD,
                                     "game", l_object_ref(game),
                                     "width", l_int_new(width),
                                     "height", l_int_new(height),
                                     NULL));
}



/* mbs_world_update:
 */
void mbs_world_update ( MbsWorld *world )
{
  guint x, y;
  for (y = 0; y < MB_WORLD_HEIGHT(world); y++)
    {
      for (x = 0; x < MB_WORLD_WIDTH(world); x++)
        {
          mbs_sector_update(MBS_SECTOR(MB_WORLD_SECTOR(world, x, y)));
        }
    }
}
