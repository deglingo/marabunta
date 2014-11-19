/* mbcolony.c -
 */

#include "common/private.h"
#include "common/mbcolony.h"
#include "common/mbplayer.h"
#include "common/mbcolony.inl"



/* mb_colony_set_owner:
 */
void mb_colony_set_owner ( MbColony *colony,
                           MbObject *player )
{
  ASSERT(MB_IS_PLAYER(player));
  ASSERT(!colony->owner); /* [TODO] */
  colony->owner = l_object_ref(player);
}
