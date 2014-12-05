/* mbroom.c -
 */

#include "mbroom.h"
#include "mbroom.inl"



/* mb_room_new:
 */
MbRoom *mb_room_new ( MbRoomType type )
{
  MbRoom *room;
  room = MB_ROOM(l_object_new(MB_CLASS_ROOM, NULL));
  room->type = type;
  return room;
}
