/* mbcroom.c -
 */

#include "client/mbcroom.h"
#include "client/mbcroom.inl"



/* mbc_room_new:
 */
MbObject *mbc_room_new ( MbObject *game,
                         guint id,
                         MbRoomType type )
{
  return MB_OBJECT(l_object_new_give(MBC_CLASS_ROOM,
                                     "id", l_int_new(id),
                                     "game", l_object_ref(game),
                                     "type", l_int_new(type),
                                     NULL));
}
