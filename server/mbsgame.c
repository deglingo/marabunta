/* mbsgame.c -
 */

#include "server/srvprivate.h"
#include "server/mbsgame.h"



/* mbs_game_new:
 */
MbsGame *mbs_game_new ( void )
{
  MbsGame *game;
  game = g_new0(MbsGame, 1);
  game->tree = lpt_tree_new();
  return game;
}



/* mbs_game_add_player:
 */
void mbs_game_add_player ( MbsGame *game,
                           guint id,
                           const gchar *name )
{
}



/* mbs_game_start:
 */
void mbs_game_start ( MbsGame *game )
{
  CL_DEBUG("starting game...");
}
