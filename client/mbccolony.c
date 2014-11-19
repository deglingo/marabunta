/* mbccolony.c -
 */

#include "client/mbccolony.h"
#include "client/mbccolony.inl"



/* mbc_colony_new:
 */
MbObject *mbc_colony_new ( MbObject *game,
                           MbObjectID id )
{
  return MB_OBJECT(l_object_new_give(MBC_CLASS_COLONY,
                                     "id", l_int_new(id),
                                     "game", l_object_ref(game),
                                     NULL));
}
