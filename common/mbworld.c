/* mbworld.c -
 */

#include "common/private.h"
#include "common/mbworld.h"
#include "common/mbworld.inl"



/* mb_world_new:
 */
MbWorld *mb_world_new ( void )
{
  return MB_WORLD(l_object_new(MB_CLASS_WORLD, NULL));
}
