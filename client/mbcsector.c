/* mbcsector.c -
 */

#include "client/mbcsector.h"
#include "client/mbcsector.inl"



/* mbc_sector_new:
 */
MbObject *mbc_sector_new ( MbObject *game,
                           MbObjectID id )
{
  return MB_OBJECT(l_object_new_give(MBC_CLASS_SECTOR,
                                     "id", l_int_new(id),
                                     "game", l_object_ref(game),
                                     NULL));
}
