/* mbcpriority.c -
 */

#include "client/mbcpriority.h"
#include "client/mbcpriority.inl"



/* mbc_priority_new:
 */
MbObject *mbc_priority_new ( MbObject *game,
                             MbObjectID id,
                             MbPriorityValue value )
{
  return MB_OBJECT(l_object_new_give(MBC_CLASS_PRIORITY,
                                     "id", l_int_new(id),
                                     "game", l_object_ref(game),
                                     "value", l_int_new(value),
                                     NULL));
}
