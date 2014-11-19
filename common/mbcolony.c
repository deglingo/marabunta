/* mbcolony.c -
 */

#include "common/private.h"
#include "common/mbcolony.h"
#include "common/mbplayer.h"
#include "common/mbtask.h"
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



/* mb_colony_set_top_task:
 */
void mb_colony_set_top_task ( MbColony *colony,
                              MbObject *task )
{
  ASSERT(MB_IS_TASK(task));
  ASSERT(!colony->top_task);
  colony->top_task = l_object_ref(task);
}
