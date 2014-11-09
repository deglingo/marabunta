/* mbcroomproxy.c -
 */

#include "client/cliprivate.h"
#include "client/mbcroomproxy.h"
#include "client/mbcgameproxy.h"
#include "client/mbcroomproxy.inl"



/* mbc_room_proxy_new:
 */
MbcProxy *mbc_room_proxy_new ( MbcProxy *game,
                               guint id,
                               MbRoomType type )
{
  MbcRoomProxy *room;
  ASSERT(MBC_IS_GAME_PROXY(game));
  room = MBC_ROOM_PROXY(mbc_game_proxy_create_object(MBC_GAME_PROXY(game),
                                                     MBC_CLASS_ROOM_PROXY,
                                                     id));
  room->type = type;
  return MBC_PROXY(room);
}
