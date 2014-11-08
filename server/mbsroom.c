/* mbsroom.c -
 */

#include "server/mbsroom.h"
#include "server/mbsroom.inl"



/* mbs_room_new:
 */
MbsRoom *mbs_room_new ( MbRoomType type )
{
  MbsRoom *room;
  room = MBS_ROOM_NEW(NULL);
  room->type = type;
  return room;
}
