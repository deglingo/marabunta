/* mbcolony.c -
 */

#include "mbcolony.h"
#include "mbcolony.inl"



/* mb_colony_new:
 */
MbColony *mb_colony_new ( void )
{
  return MB_COLONY(l_object_new(MB_CLASS_COLONY, NULL));
}



/* mb_colony_update:
 */
void mb_colony_update ( MbColony *colony )
{
  CL_DEBUG("[TODO] colony update");
}
