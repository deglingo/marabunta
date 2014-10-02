/* mbsgame.c -
 */

#include "server/srvprivate.h"
#include "server/mbsgame.h"
#include "server/mbsapp.h"
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



/* _message_handler:
 */
static void _message_handler ( LptTree *tree,
                               LptClient *client,
                               LObject *message,
                               gpointer data )
{
  MbsGame *game = data;
  guint clid;
  LTuple *mbmsg;
  /* CL_DEBUG("[TODO] handle_message: %s", l_object_to_string(message)); */
  clid = GPOINTER_TO_UINT(g_hash_table_lookup(game->lpt_rclients, client));
  ASSERT(clid);
  mbmsg = l_tuple_newl_give(2,
                            l_int_new(MB_MESSAGE_KEY_LPT_EVENT),
                            l_object_ref(message),
                            NULL);
  mbs_app_send(game->app, clid, L_OBJECT(mbmsg));
  l_object_unref(mbmsg);
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

  /* setup the shares */
  lpt_tree_set_message_handler(game->tree, _message_handler, game, NULL);
  lpt_tree_create_share(game->tree,
                        "GAME",
                        "/game",
                        0 /* [flags] */);
}



/* mbs_game_new:
 */
MbsGame *mbs_game_new ( struct _MainData *app )
{
  MbsGame *game;
  game = MBS_GAME(l_object_new(MBS_CLASS_GAME, NULL));
  game->app = app;
  game->lpt_clients = g_hash_table_new(NULL, NULL);
  game->lpt_rclients = g_hash_table_new(NULL, NULL);
  _create_tree(game);
  return game;
}



/* mbs_game_add_player:
 */
void mbs_game_add_player ( MbsGame *game,
                           guint id,
                           const gchar *name )
{
  LptClient *client;
  client = lpt_tree_add_client(game->tree, name);
  g_hash_table_insert(game->lpt_clients, GUINT_TO_POINTER(id), client);
  g_hash_table_insert(game->lpt_rclients, client, GUINT_TO_POINTER(id));
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
  game->fps = 1.0;
  g_timeout_add_full(MBS_PRIORITY_GAME_TIMER,
                     1000,
                     (GSourceFunc) _on_game_timer,
                     game,
                     NULL);
}



/* mbs_game_lpt_event:
 */
void mbs_game_lpt_event ( MbsGame *game,
                          guint clid,
                          LObject *event )
{
  LptClient *client;
  client = g_hash_table_lookup(game->lpt_clients, GUINT_TO_POINTER(clid));
  lpt_tree_handle_message(game->tree, client, event);
}
