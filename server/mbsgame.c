/* mbsgame.c -
 */

#include "server/srvprivate.h"
#include "server/mbsgame.h"
#include "server/mbsapp.h"
#include "server/mbsgame.inl"



#define MAX_PLAYERS 8



/* Player:
 */
typedef struct _Player
{
  MbsGame *game;
  MbsPlayerID id;
  gchar *name;
  MbsMessageHandler message_handler;
  gpointer handler_data;
  GDestroyNotify destroy_data;
  MbState *state;
}
  Player;



/* Private:
 */
typedef struct _Private
{
  Player *players[MAX_PLAYERS];
  gint n_players;
}
  Private;

#define PRIVATE(game) ((Private *)(MBS_GAME(game)->private))



/* player_new:
 */
static Player *player_new ( MbsGame *game,
                            MbsPlayerID id,
                            const gchar *name,
                            MbsMessageHandler message_handler,
                            gpointer handler_data,
                            GDestroyNotify destroy_data )
{
  Player *p = g_new0(Player, 1);
  p->game = game;
  p->id = id;
  p->name = g_strdup(name);
  p->message_handler = message_handler;
  p->handler_data = handler_data;
  p->destroy_data = destroy_data;
  return p;
}



/* mbs_game_class_init:
 */
static void mbs_game_class_init ( LObjectClass *cls )
{
  /* MbsGameClass *gcls = (MbsGameClass *) cls; */
}



/* mbs_game_init:
 */
static void mbs_game_init ( LObject *obj )
{
  MBS_GAME(obj)->private = g_new0(Private, 1);
}



/* mbs_game_new:
 */
MbsGame *mbs_game_new ( void )
{
  return MBS_GAME_NEW(NULL);
}



/* mbs_game_add_player:
 */
MbsPlayerID mbs_game_add_player ( MbsGame *game,
                                  const gchar *name,
                                  MbsMessageHandler message_handler,
                                  gpointer message_handler_data,
                                  GDestroyNotify destroy_data )
{
  Private *priv = PRIVATE(game);
  MbsPlayerID id;
  Player *player;
  ASSERT(priv->n_players < MAX_PLAYERS);
  id = priv->n_players;
  player = player_new(game, id, name, message_handler, message_handler_data, destroy_data);
  priv->players[id] = player;
  priv->n_players++;
  CL_DEBUG("game %p/%p: player %d added: '%s'", game, priv, id, name);
  return player->id;
}



/* send:
 */
static void _send ( MbsGame *game,
                    Player *player,
                    MbMessage *message )
{
  CL_DEBUG("send(player=%d, key=%d)", player->id, message->key);
  player->message_handler(player->id, message, player->handler_data);
}



/* _game_update:
 */
static void _game_update ( MbsGame *game )
{
  Private *priv = PRIVATE(game);
  gint p;
  game->frame++;
  CL_TRACE("%d", game->frame);
  CL_TRACE("game=%p/%p, n_players=%d", game, priv, priv->n_players);
  /* prepare the messages */
  for (p = 0; p < priv->n_players; p++)
    {
      Player *player = priv->players[p];
      CL_TRACE("%d", p);
      MbStateBlock *block;
      player->state = mb_state_new();
      block = mb_state_next(player->state, MB_STATE_SIM_TIME);
      block->v0.v_int = game->frame;
    }
  /* [TODO] game update */
  /* send all the messages */
  for (p = 0; p < priv->n_players; p++)
    {
      Player *player = priv->players[p];
      MbMessage *msg = mb_message_new(MB_MESSAGE_KEY_GAME_STATE,
                                      L_OBJECT(player->state));
      _send(game, player, msg);
      l_object_unref(msg);
      L_OBJECT_CLEAR(player->state);
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



/* _setup:
 */
static void _setup ( MbsGame *game )
{
  game->world = mbs_world_new(1, 2);
  mbs_sector_create_colony(game->world->sectors[0][0], 0);
}



/* _send_game_setup:
 */
static void _send_game_setup ( MbsGame *game,
                               Player *player )
{
  MbState *state = mb_state_new();
  MbMessage *msg = mb_message_new(MB_MESSAGE_KEY_GAME_SETUP, L_OBJECT(state));
  MbStateBlock *block;
  guint x, y;
  block = mb_state_next(state, MB_STATE_RESET);
  block = mb_state_next(state, MB_STATE_WORLD_SIZE);
  block->v0.v_int = game->world->width;
  block->v1.v_int = game->world->height;
  for (y = 0; y < game->world->height; y++)
    {
      for (x = 0; x < game->world->width; x++)
        {
          MbsSector *sector = game->world->sectors[y][x];
          if (sector->colony)
            {
              block = mb_state_next(state, MB_STATE_COLONY);
              block->v0.v_int = x;
              block->v1.v_int = y;
              block->v2.v_int = sector->colony->owner;
            }
        }
    }
  _send(game, player, msg);
  l_object_unref(state);
  l_object_unref(msg);
}



/* mbs_game_start:
 */
void mbs_game_start ( MbsGame *game )
{
  Private *priv = PRIVATE(game);
  gint p;
  CL_DEBUG("starting game...");
  /* [fixme] should not be here */
  _setup(game);
  /* send the game state */
  for (p = 0; p < priv->n_players; p++)
    {
      _send_game_setup(game, priv->players[p]);
    }
  /* setup the game timer */
  game->fps = 1.0;
  game->next_frame = 0.0;
  game->timer = g_timer_new();
  g_timeout_add_full(MBS_PRIORITY_GAME_TIMER,
                     100,
                     (GSourceFunc) _on_game_timer,
                     game,
                     NULL);
}
