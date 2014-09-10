/* mbsgame.c -
 */

#include "server/srvprivate.h"
#include "server/mbsgame.h"



/* mbs_game_new:
 */
MbsGame *mbs_game_new ( void )
{
  MbsGame *game;
  LptNSpec *nspec_uint;
  LString *key;
  game = g_new0(MbsGame, 1);
  game->tree = lpt_tree_new();
  nspec_uint = lpt_nspec_int_new("UINT",
                                 0,
                                 G_MAXINT,
                                 0);
  key = l_string_new("sim-time");
  lpt_node_new(nspec_uint, game->tree->root, L_OBJECT(key));
  l_object_unref(key);
  return game;
}



/* mbs_game_add_player:
 */
void mbs_game_add_player ( MbsGame *game,
                           guint id,
                           const gchar *name )
{
}



/* _game_update:
 */
static void _game_update ( MbsGame *game )
{
  game->frame++;
  CL_DEBUG("game frame %d", game->frame);
}



/* _on_game_timer:
 */
static gboolean _on_game_timer ( MbsGame *game )
{
  guint frame;
  frame = (guint) (g_timer_elapsed(game->timer, NULL) * (gdouble) game->fps);
  /* CL_DEBUG(" - %d/%d (%f)", frame, game->frame, g_timer_elapsed(game->timer, NULL)); */
  while (frame > game->frame)
    _game_update(game);
  return G_SOURCE_CONTINUE;
}



/* mbs_game_start:
 */
void mbs_game_start ( MbsGame *game )
{
  CL_DEBUG("starting game...");
  game->timer = g_timer_new();
  game->fps = 10.0;
  game->frame = 0;
  g_timeout_add_full(MBS_PRIORITY_GAME_TIMER,
                     1,
                     (GSourceFunc) _on_game_timer,
                     game,
                     NULL);
}
