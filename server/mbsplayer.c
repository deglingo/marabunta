/* mbsplayer.c -
 */

#include "server/mbsplayer.h"
#include "server/mbsplayer.inl"



/* mbs_player_new:
 */
MbObject *mbs_player_new ( MbObject *game,
                           const gchar *name )
{
  return MB_OBJECT(l_object_new_give(MBS_CLASS_PLAYER,
                                     "game", l_object_ref(game),
                                     "name", l_string_new(name),
                                     NULL));
}
