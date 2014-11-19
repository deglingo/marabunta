/* mbsgame.c -
 */

#include "server/srvprivate.h"
#include "server/mbsgame.h"
#include "server/mbsplayer.h"
#include "server/mbsworld.h"
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
  world = mbs_world_new(MB_OBJECT(game), 3, 2);
  mb_game_set_world(MB_GAME(game), world);
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
  st_game = mb_state_next(state, MB_STATE_GAME_SETUP);
  st_game->n_players = g_list_length(MB_GAME(game)->players);
  for (l = MB_GAME(game)->players, n = 0; l; l = l->next, n++)
    {
      MbsPlayer *p = l->data;
      ASSERT(n < MB_GAME_MAX_PLAYERS);
      ASSERT(MB_PLAYER(p)->name->len <= MB_PLAYER_MAX_NAME);
      st_game->players[n].id = MB_OBJECT_ID(p);
      sprintf(st_game->players[n].name, MB_PLAYER(p)->name->str);
    }
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
