/* mbsgame.c -
 */

#include "server/srvprivate.h"
#include "server/mbsgame.h"
#include "server/mbsplayer.h"
#include "server/mbsworld.h"
#include "server/mbssector.h"
#include "server/mbsgame.inl"



/* Private:
 */
typedef struct _Private
{
  int dummy;
}
  Private;

#define PRIVATE(game) ((Private *)(MBS_GAME(game)->private))



/* mbs_game_init:
 */
static void mbs_game_init ( LObject *obj )
{
  MBS_GAME(obj)->private = g_new0(Private, 1);
}



/* mbs_game_new:
 */
MbObject *mbs_game_new ( void )
{
  return MB_OBJECT(l_object_new(MBS_CLASS_GAME, NULL));
}



/* mbs_game_setup:
 */
void mbs_game_setup ( MbsGame *game )
{
  MbObject *world;
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



/* mbs_game_start:
 */
void mbs_game_start ( MbsGame *game )
{
  GList *l;
  for (l = MB_GAME(game)->players; l; l = l->next)
    {
      MbsPlayer *player = l->data;
      _send_game_setup(game, player);
    }
}
