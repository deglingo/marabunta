/* mbsgame.c -
 */

#include "server/srvprivate.h"
#include "server/mbsgame.h"
#include "server/mbsapp.h"
#include "server/mbsgame.inl"



/* Player:
 */
typedef struct _Player
{
  MbsGame *game;
  gchar *name;
  LptClient *lpt_client;
  MbsMessageHandler message_handler;
  gpointer handler_data;
  GDestroyNotify destroy_data;
  MbMessage *message;
}
  Player;



/* player_new:
 */
static Player *player_new ( MbsGame *game,
                            const gchar *name,
                            MbsMessageHandler message_handler,
                            gpointer handler_data,
                            GDestroyNotify destroy_data )
{
  Player *p = g_new0(Player, 1);
  p->game = game;
  p->name = g_strdup(name);
  p->message_handler = message_handler;
  p->handler_data = handler_data;
  p->destroy_data = destroy_data;
  return p;
}



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



/* mbs_game_new:
 */
MbsGame *mbs_game_new ( MbsGameTreeHandler tree_handler )
{
  MbsGame *game;
  game = MBS_GAME(l_object_new(MBS_CLASS_GAME, NULL));
  /* game->app = app; */
  /* game->lpt_clients = g_hash_table_new(NULL, NULL); */
  /* game->lpt_rclients = g_hash_table_new(NULL, NULL); */
  game->tree_handler = tree_handler;
  game->tree = lpt_tree_new();
  return game;
}



/* mbs_game_add_player:
 */
MbsPlayerID mbs_game_add_player ( MbsGame *game,
                                  const gchar *name,
                                  MbsMessageHandler message_handler,
                                  gpointer message_handler_data,
                                  GDestroyNotify destroy_data )
{
  Player *player = player_new(game, name, message_handler, message_handler_data, destroy_data);
  game->players = g_list_append(game->players, player);
  player->lpt_client = lpt_tree_add_client(game->tree, name, player, NULL);
  return player;
}



/* send:
 */
static void _send ( MbsGame *game,
                    Player *player,
                    MbMessage *message )
{
  player->message_handler(player, message, player->handler_data);
}



/* _game_update:
 */
static void _game_update ( MbsGame *game )
{
  GList *l;
  game->frame++;
  CL_TRACE("%d", game->frame);
  /* prepare the messages */
  for (l = game->players; l; l = l->next)
    {
      Player *p = l->data;
      p->message = mb_message_new(MB_MESSAGE_KEY_GAME_UPDATE);
      /* [fixme] */
      p->message->frame = game->frame;
    }
  /* [TODO] game update */
  /* send all the messages */
  for (l = game->players; l; l = l->next)
    {
      Player *p = l->data;
      _send(game, p, p->message);
      L_OBJECT_CLEAR(p->message);
    }
}



/* _on_game_timer:
 */
static gboolean _on_game_timer ( MbsGame *game )
{
  gdouble elapsed = g_timer_elapsed(game->timer, NULL);
  while (elapsed >= game->next_frame)
    {
      _game_update(game);
      game->next_frame = ((gdouble) game->frame) / game->fps;
    }
  return G_SOURCE_CONTINUE;
}



/* mbs_game_start:
 */
void mbs_game_start ( MbsGame *game )
{
  CL_DEBUG("starting game...");
  game->world = mbs_world_new();
  game->fps = 1.0;
  game->next_frame = 0.0;
  game->timer = g_timer_new();
  g_timeout_add_full(MBS_PRIORITY_GAME_TIMER,
                     1,
                     (GSourceFunc) _on_game_timer,
                     game,
                     NULL);
}



/* mbs_game_lpt_event:
 */
void mbs_game_lpt_event ( MbsGame *game,
                          MbsPlayerID player,
                          LObject *event )
{
  LptClient *client = ((Player *) player)->lpt_client;
  lpt_tree_handle_message(game->tree, client, event);
}
