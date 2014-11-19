/* mbsgame.c -
 */

#include "server/srvprivate.h"
#include "server/mbsgame.h"
#include "server/mbsplayer.h"
#include "server/mbsworld.h"
#include "server/mbssector.h"
#include "server/mbscolony.h"
#include "server/mbsgame.inl"



#define MBS_GAME_FPS ((gdouble) 5.0)



/* Private:
 */
typedef struct _Private
{
  GList *players; /* list < PlayerData > */
  /* update timer */
  GTimer *timer;
  gdouble next_frame;
}
  Private;

#define PRIVATE(game) ((Private *)(MBS_GAME(game)->private))



/* PlayerData:
 */
typedef struct _PlayerData
{
  MbObject *player;
  MbState *state;
}
  PlayerData;



static void add_player ( MbGame *game,
                         MbObject *player );



/* mbs_game_class_init:
 */
static void mbs_game_class_init ( LObjectClass *cls )
{
  MB_GAME_CLASS(cls)->add_player = add_player;
}



/* mbs_game_init:
 */
static void mbs_game_init ( LObject *obj )
{
  MBS_GAME(obj)->private = g_new0(Private, 1);
  PRIVATE(obj)->timer = g_timer_new();
}



/* mbs_game_new:
 */
MbObject *mbs_game_new ( void )
{
  return MB_OBJECT(l_object_new(MBS_CLASS_GAME, NULL));
}



/* add_player:
 */
static void add_player ( MbGame *game,
                         MbObject *player )
{
  PlayerData *data;
  MB_GAME_CLASS(parent_class)->add_player(game, player);
  data = g_new0(PlayerData, 1);
  data->player = player; /* ref owned by MbGame */
  PRIVATE(game)->players = g_list_append(PRIVATE(game)->players, data);
}



/* mbs_game_setup:
 */
void mbs_game_setup ( MbsGame *game )
{
  MbObject *world, *colony;
  guint x, y;
  world = mbs_world_new(MB_OBJECT(game), 3, 2);
  mb_game_set_world(MB_GAME(game), world);
  for (y = 0; y < MB_WORLD(world)->height; y++)
    {
      for (x = 0; x < MB_WORLD(world)->width; x++)
        {
          MbObject *sector = mbs_sector_new(MB_OBJECT(game));
          mb_world_add_sector(MB_WORLD(world), sector, x, y);
        }
    }
  /* [FIXME] */
  colony = mbs_colony_new(MB_OBJECT(game));
  ASSERT(MB_GAME(game)->players);
  mb_colony_set_owner(MB_COLONY(colony), MB_GAME(game)->players->data);
  mb_sector_set_colony(MB_SECTOR(MB_WORLD_SECTOR(world, 0, 0)), colony);
}



/* _send_colony_setup:
 */
static void _send_colony_setup ( MbsGame *game,
                                 MbState *state,
                                 MbObject *colony )
{
  MbStateColonySetup *st_col;
  st_col = mb_state_next(state, MB_STATE_COLONY_SETUP);
  st_col->colony_id = MB_OBJECT_ID(colony);
  st_col->sector_id = MB_OBJECT_ID(MB_COLONY_SECTOR(colony));
  st_col->owner_id = MB_OBJECT_ID(MB_COLONY_OWNER(colony));
}



/* _send_sector_setup:
 */
static void _send_sector_setup ( MbsGame *game,
                                 MbState *state,
                                 MbObject *sector )
{
  MbStateSectorSetup *st_sector;
  st_sector = mb_state_next(state, MB_STATE_SECTOR_SETUP);
  st_sector->sector_id = MB_OBJECT_ID(sector);
  st_sector->x = MB_SECTOR_X(sector);
  st_sector->y = MB_SECTOR_Y(sector);
  if (MB_SECTOR_COLONY(sector))
    _send_colony_setup(game, state, MB_SECTOR_COLONY(sector));
}



/* _send_game_setup:
 */
static void _send_game_setup ( MbsGame *game,
                               MbsPlayer *player )
{
  MbState *state = mb_state_new();
  MbStateGameSetup *st_game;
  GList *l;
  guint n;
  guint x, y;
  st_game = mb_state_next(state, MB_STATE_GAME_SETUP);
  /* players */
  st_game->n_players = g_list_length(MB_GAME(game)->players);
  for (l = MB_GAME(game)->players, n = 0; l; l = l->next, n++)
    {
      MbsPlayer *p = l->data;
      ASSERT(n < MB_GAME_MAX_PLAYERS);
      ASSERT(MB_PLAYER(p)->name->len <= MB_PLAYER_MAX_NAME);
      st_game->players[n].id = MB_OBJECT_ID(p);
      sprintf(st_game->players[n].name, MB_PLAYER(p)->name->str);
    }
  /* world */
  st_game->world_id = MB_OBJECT_ID(MB_GAME_WORLD(game));
  st_game->world_width = MB_WORLD_WIDTH(MB_GAME_WORLD(game));
  st_game->world_height = MB_WORLD_HEIGHT(MB_GAME_WORLD(game));
  for (y = 0; y < st_game->world_height; y++)
    for (x = 0; x < st_game->world_width; x++)
      _send_sector_setup(game, state, MB_WORLD_SECTOR(MB_GAME_WORLD(game), x, y));
  /* send */
  mb_player_handle_state(MB_PLAYER(player), state);
  l_object_unref(state);
}



static void _game_update ( MbsGame *game )
{
  mbs_world_update(MBS_WORLD(MB_GAME_WORLD(game)));
  mb_game_set_frame_count(MB_GAME(game),
                          MB_GAME_FRAME_COUNT(game) + 1);
}



static void _send_game_update ( MbsGame *game )
{
  Private *priv = PRIVATE(game);
  GList *l;
  for (l = priv->players; l; l = l->next)
    {
      PlayerData *player = l->data;
      MbStateGameUpdate *st_game;
      st_game = mb_state_next(player->state, MB_STATE_GAME_UPDATE);
      st_game->frame_count = MB_GAME_FRAME_COUNT(game);
      mb_player_handle_state(MB_PLAYER(player->player),
                             player->state);
      L_OBJECT_CLEAR(player->state);
    }
}



static gboolean _game_timer ( MbsGame *game )
{
  Private *priv = PRIVATE(game);
  gdouble elapsed = g_timer_elapsed(priv->timer, NULL);
  GList *l;
  /* prepare the players states */
  for (l = priv->players; l; l = l->next)
    {
      PlayerData *player = l->data;
      player->state = mb_state_new();
    }
  while (elapsed >= priv->next_frame)
    {
      _game_update(game);
      priv->next_frame = ((gdouble) MB_GAME_FRAME_COUNT(game)) / MBS_GAME_FPS;
    }
  _send_game_update(game);
  return G_SOURCE_CONTINUE;
}



/* mbs_game_start:
 */
void mbs_game_start ( MbsGame *game )
{
  Private *priv = PRIVATE(game);
  GList *l;
  for (l = MB_GAME(game)->players; l; l = l->next)
    {
      MbsPlayer *player = l->data;
      _send_game_setup(game, player);
    }
  /* install the game timer */
  g_timeout_add_full(MBS_PRIORITY_GAME_TIMER,
                     10,
                     (GSourceFunc) _game_timer,
                     game,
                     NULL);
  priv->next_frame = 0;
  g_timer_start(priv->timer);
}
