/* mbplayer.c -
 */

#include "common/mbplayer.h"
#include "common/mbplayer.inl"



/* mb_player_new:
 */
MbObject *mb_player_new ( MbObject *game,
                          guint id,
                          const gchar *name )
{
  MbObject *player;
  player = mb_object_new(MB_CLASS_PLAYER, game, id);
  MB_PLAYER(player)->name = g_strdup(name);
  return player;
}
