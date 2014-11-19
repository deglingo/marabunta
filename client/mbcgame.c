/* mbcgame.c -
 */

#include "client/cliprivate.h"
#include "client/mbcgame.h"
#include "client/mbcplayer.h"
#include "client/mbcgame.inl"



/* mbc_game_new:
 */
MbObject *mbc_game_new ( void )
{
  return MB_OBJECT(l_object_new(MBC_CLASS_GAME, NULL));
}



/* _handle_game_setup:
 */
static void _handle_game_setup ( MbcGame *game,
                                 MbStateGameSetup *st_game )
{
  ASSERT(!MB_GAME(game)->players); /* [todo] reset */
  guint n;
  CL_DEBUG("game setup");
  for (n = 0; n < st_game->n_players; n++)
    {
      MbObject *player = mbc_player_new(MB_OBJECT(game),
                                        st_game->players[n].id,
                                        st_game->players[n].name);
      mb_game_add_player(MB_GAME(game), player);
      l_object_unref(player);
      CL_DEBUG("  player %d: '%s'", n, MB_PLAYER(player)->name->str);
    }
}



/* mbc_game_update_state:
 */
void mbc_game_update_state ( MbcGame *game,
                             MbState *state )
{
  MbStateBlock *block;
  guint offset = 0;
  while ((block = mb_state_read(state, &offset)))
    {
      switch (block->type)
        {
        case MB_STATE_GAME_SETUP:
          _handle_game_setup(game, (MbStateGameSetup *) block);
          l_signal_emit(L_OBJECT(game), "started", 0);
          break;
        default:
          CL_ERROR("[TODO] block type %d", block->type);
        }
    }
}
