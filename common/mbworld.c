/* mbworld.c -
 */

#include "common/private.h"
#include "common/mbworld.h"
#include "common/mbworld.inl"



/* mb_world_new:
 */
MbObject *mb_world_new ( MbObject *game,
                         MbObjectID id )
{
  MbObject *world;
  world = mb_object_new(MB_CLASS_WORLD, game, id);
  return world;
}
