/* mbsroom.c -
 */

#include "server/mbsroom.h"
#include "server/mbsroom.inl"



/* mbs_room_new:
 */
MbObject *mbs_room_new ( MbObject *game,
                         gint type )
{
  return MB_OBJECT(l_object_new_give(MBS_CLASS_ROOM,
                                     "game", l_object_ref(game),
                                     "type", l_int_new(type),
                                     NULL));
}
