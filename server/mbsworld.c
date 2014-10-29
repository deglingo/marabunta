/* mbsworld.c -
 */

#include "server/mbsworld.h"
#include "server/mbsworld.inl"



/* mbs_world_new:
 */
MbsWorld *mbs_world_new ( void )
{
  MbsWorld *world;
  world = MBS_WORLD(l_object_new(MBS_CLASS_WORLD, NULL));
  return world;
}
