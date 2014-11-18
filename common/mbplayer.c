/* mbplayer.c -
 */

#include "common/mbplayer.h"
#include "common/mbplayer.inl"



/* mb_player_new:
 */
MbObject *mb_player_new ( MbObject *game,
                          guint id,
                          const gchar *name,
                          MbPlayerHandler handler,
                          gpointer handler_data,
                          GDestroyNotify destroy_data )
{
  MbPlayer *player;
  player = MB_PLAYER(mb_object_new(MB_CLASS_PLAYER, game, id));
  player->name = g_strdup(name);
  player->handler = handler;
  player->handler_data = handler_data;
  player->destroy_data = destroy_data;
  return MB_OBJECT(player);
}



/* mb_player_handle_state:
 */
void mb_player_handle_state ( MbPlayer *player,
                              MbState *state )
{
  player->handler(player, state, player->handler_data);
}
