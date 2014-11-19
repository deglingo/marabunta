/* mbcplayer.c -
 */

#include "client/mbcplayer.h"
#include "client/mbcplayer.inl"



/* mbc_player_new:
 */
MbObject *mbc_player_new ( MbObject *game,
                           MbObjectID id,
                           const gchar *name )
{
  return MB_OBJECT(l_object_new_give(MBC_CLASS_PLAYER,
                                     "id", l_int_new(id),
                                     "game", l_object_ref(game),
                                     "name", l_string_new(name),
                                     NULL));
}
