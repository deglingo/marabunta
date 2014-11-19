/* mbspriority.c -
 */

#include "server/mbspriority.h"
#include "server/mbspriority.inl"



/* mbs_priority_new:
 */
MbObject *mbs_priority_new ( MbObject *game,
                             MbPriorityValue value )
{
  return MB_OBJECT(l_object_new_give(MBS_CLASS_PRIORITY,
                                     "game", l_object_ref(game),
                                     "value", l_int_new(value),
                                     NULL));
}
