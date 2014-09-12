/* mbsgame.c -
 */

#include "server/srvprivate.h"
#include "server/mbsgame.h"
#include "server/mbsgame.inl"



/* mbs_game_class:
 */
static void mbs_game_class_init ( LObjectClass *cls )
{
  MbsGameClass *gcls = (MbsGameClass *) cls;

  gcls->nspec_dir = lpt_nspec_dir_new("DIR");

  gcls->nspec_uint = lpt_nspec_int_new("UINT",
                                       0,
                                       G_MAXINT,
                                       0);
}



/* _create_tree:
 */
static void _create_tree ( MbsGame *game )
{
  MbsGameClass *cls = MBS_GAME_GET_CLASS(game);

  game->tree = lpt_tree_new();

  lpt_tree_create_node(game->tree,
                       "/game",
                       cls->nspec_dir);

  game->n_sim_time =
    lpt_tree_create_node(game->tree,
                         "/game/sim-time",
                         cls->nspec_uint);

  { /* [removeme] */
    LInt *v = l_int_new(0);
    lpt_node_set_value(game->n_sim_time, L_OBJECT(v));
    l_object_unref(v);
  }

  /* create the proxy server */
  game->proxy = lpt_proxy_new(game->tree);

  lpt_proxy_create_share(game->proxy,
                         "GAME",
                         "/game",
                         0 /* [flags] */);
}



/* mbs_game_new:
 */
MbsGame *mbs_game_new ( void )
{
  MbsGame *game;
  game = MBS_GAME(l_object_new(MBS_CLASS_GAME, NULL));
  _create_tree(game);
  return game;
}



/* mbs_game_add_player:
 */
void mbs_game_add_player ( MbsGame *game,
                           guint id,
                           const gchar *name )
{
  lpt_proxy_connect_client(game->proxy, id);
}



/* _game_update:
 */
static void _game_update ( MbsGame *game )
{
  LInt *frame, *frame2;
  frame = L_INT(lpt_node_get_value(game->n_sim_time));
  frame2 = l_int_new(L_INT_VALUE(frame) + 1);
  CL_DEBUG("frame %d", L_INT_VALUE(frame2));
  lpt_node_set_value(game->n_sim_time, L_OBJECT(frame2));
  l_object_unref(frame2);
}



/* _on_game_timer:
 */
static gboolean _on_game_timer ( MbsGame *game )
{
  guint frame;
  frame = (guint) (g_timer_elapsed(game->timer, NULL) * (gdouble) game->fps);
  /* CL_DEBUG(" - %d/%d (%f)", frame, game->frame, g_timer_elapsed(game->timer, NULL)); */
  while (frame > L_INT_VALUE(lpt_node_get_value(game->n_sim_time)))
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
  g_timeout_add_full(MBS_PRIORITY_GAME_TIMER,
                     1,
                     (GSourceFunc) _on_game_timer,
                     game,
                     NULL);
}
