/* mbworld.c -
 */

#include "mbworld.h"
#include "mbworld.inl"



/* mb_world_new:
 */
MbWorld *mb_world_new ( guint width,
                        guint height )
{
  MbWorld *world;
  world = MB_WORLD(l_object_new(MB_CLASS_WORLD, NULL));
  return world;
}
