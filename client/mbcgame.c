/* mbcgame.c -
 */

#include "client/cliprivate.h"
#include "client/mbcgame.h"
#include "client/mbcplayer.h"
#include "client/mbcworld.h"
#include "client/mbcsector.h"
#include "client/mbccolony.h"
#include "client/mbcgame.inl"



/* Signals:
 */
enum
  {
    SIG_STARTED,
    SIG_COUNT,
  };

static LSignalID signals[SIG_COUNT];



/* mbc_game_class_init:
 */
static void mbc_game_class_init ( LObjectClass *cls )
{
  signals[SIG_STARTED] = l_signal_new(cls,
                                      "started");
}



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
  MbObject *world;
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
  world = mbc_world_new(MB_OBJECT(game),
                        st_game->world_id,
                        st_game->world_width,
                        st_game->world_height);
  mb_game_set_world(MB_GAME(game), world);
  l_object_unref(world);
}



/* _handle_sector_setup:
 */
static void _handle_sector_setup ( MbcGame *game,
                                   MbStateSectorSetup *st_sector )
{
  MbObject *sector;
  sector = mbc_sector_new(MB_OBJECT(game),
                          st_sector->sector_id);
  mb_world_add_sector(MB_WORLD(MB_GAME_WORLD(game)),
                      sector,
                      st_sector->x,
                      st_sector->y);
  l_object_unref(sector);
}



/* _handle_colony_setup:
 */
static void _handle_colony_setup ( MbcGame *game,
                                   MbStateColonySetup *st_col )
{
  MbObject *colony, *sector, *owner;
  owner = mb_game_lookup_object(MB_GAME(game), st_col->owner_id);
  ASSERT(owner && MB_IS_PLAYER(owner));
  sector = mb_game_lookup_object(MB_GAME(game), st_col->sector_id);
  ASSERT(sector && MB_IS_SECTOR(sector));
  colony = mbc_colony_new(MB_OBJECT(game), st_col->colony_id);
  mb_colony_set_owner(MB_COLONY(colony), owner);
  mb_sector_set_colony(MB_SECTOR(sector), colony);
}



/* _handle_game_update:
 */
static void _handle_game_update ( MbcGame *game,
                                  MbStateGameUpdate *st_game )
{
  mb_game_set_frame_count(MB_GAME(game), st_game->frame_count);
}



/* mbc_game_update_state:
 */
void mbc_game_update_state ( MbcGame *game,
                             MbState *state )
{
  MbStateBlock *block;
  guint offset = 0;
  gboolean started = FALSE;
  while ((block = mb_state_read(state, &offset)))
    {
      switch (block->type)
        {
        case MB_STATE_GAME_SETUP:
          _handle_game_setup(game, (MbStateGameSetup *) block);
          started = TRUE;
          break;
        case MB_STATE_SECTOR_SETUP:
          _handle_sector_setup(game, (MbStateSectorSetup *) block);
          break;
        case MB_STATE_COLONY_SETUP:
          _handle_colony_setup(game, (MbStateColonySetup *) block);
          break;
        case MB_STATE_GAME_UPDATE:
          _handle_game_update(game, (MbStateGameUpdate *) block);
          break;
        default:
          CL_ERROR("[TODO] block type %d", block->type);
        }
    }
  if (started) {
    l_signal_emit(L_OBJECT(game), signals[SIG_STARTED], 0);
  }
}
