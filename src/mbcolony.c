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
