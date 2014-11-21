/* mbcgame.c -
 */

#include "client/cliprivate.h"
#include "client/mbcgame.h"
#include "client/mbcresource.h"
#include "client/mbcplayer.h"
#include "client/mbcworld.h"
#include "client/mbcsector.h"
#include "client/mbccolony.h"
#include "client/mbcpriority.h"
#include "client/mbctask.h"
#include "client/mbcroom.h"
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
MbObject *mbc_game_new ( MbStateHandler handler,
                         gpointer handler_data,
                         GDestroyNotify destroy_data )
{
  MbcGame *game = MBC_GAME(l_object_new(MBC_CLASS_GAME, NULL));
  game->handler = handler;
  game->handler_data = handler_data;
  game->destroy_data = destroy_data;
  return MB_OBJECT(game);
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



/* _handle_resource_setup:
 */
static void _handle_resource_setup ( MbcGame *game,
                                     MbStateResourceSetup *st_rsc )
{
  MbObject *rsc;
  rsc = mbc_resource_new(MB_OBJECT(game),
                         st_rsc->resource_id,
                         st_rsc->name,
                         st_rsc->flags);
  mb_game_register_resource(MB_GAME(game), rsc);
  l_object_unref(rsc);
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



/* _handle_colony_update:
 */
static void _handle_colony_update ( MbcGame *game,
                                    MbStateColonyUpdate *st_col )
{
  MbObject *colony;
  gint i;
  colony = mb_game_lookup_object(MB_GAME(game), st_col->colony_id);
  ASSERT(colony && MBC_IS_COLONY(colony));
  mbc_colony_set_pop(MBC_COLONY(colony), st_col->pop);
  for (i = 0; i < st_col->stock_size; i++)
    {
      mb_colony_set_stock(MB_COLONY(colony),
                          st_col->stock[i].resource_id,
                          st_col->stock[i].qtty);
    }
}



/* _handle_priority_setup:
 */
static void _handle_priority_setup ( MbcGame *game,
                                     MbStatePrioritySetup *st_prio )
{
  MbObject *priority;
  priority = mbc_priority_new(MB_OBJECT(game), st_prio->priority_id, st_prio->value);
  l_object_unref(priority);
}



/* _handle_task_setup:
 */
static void _handle_task_setup ( MbcGame *game,
                                 MbStateTaskSetup *st_task )
{
  MbObject *parent, *colony, *priority, *resource, *task;
  colony = mb_game_lookup_object(MB_GAME(game), st_task->colony_id);
  ASSERT(colony && MBC_IS_COLONY(colony));
  priority = mb_game_lookup_object(MB_GAME(game), st_task->priority_id);
  ASSERT(priority && MBC_IS_PRIORITY(priority));
  if (st_task->resource_id) {
    resource = mb_game_lookup_object(MB_GAME(game), st_task->resource_id);
    ASSERT(resource && MBC_IS_RESOURCE(resource));
  } else {
    resource = NULL;
  }
  if (st_task->isgroup)
    task = mbc_task_new_group(MB_OBJECT(game), st_task->task_id, st_task->name, priority);
  else
    task = mbc_task_new(MB_OBJECT(game), st_task->task_id, st_task->name, resource, priority);
  if (st_task->parent_id)
    {
      parent = mb_game_lookup_object(MB_GAME(game), st_task->parent_id);
      ASSERT(parent && MBC_IS_TASK(parent));
      ASSERT(MB_TASK_COLONY(parent) == colony);
      mb_task_add(MB_TASK(parent), task);
    }
  else
    {
      parent = NULL;
      mb_colony_set_top_task(MB_COLONY(colony), task);
    }
  l_object_unref(task);
}



/* _handle_priority_update:
 */
static void _handle_priority_update ( MbcGame *game,
                                      MbStatePriorityUpdate *st_prio )
{
  MbObject *priority;
  priority = mb_game_lookup_object(MB_GAME(game), st_prio->priority_id);
  ASSERT(priority && MBC_IS_PRIORITY(priority));
  mb_priority_set_value(MB_PRIORITY(priority), st_prio->value);
}



/* _handle_task_update:
 */
static void _handle_task_update ( MbcGame *game,
                                  MbStateTaskUpdate *st_task )
{
  MbObject *task;
  task = mb_game_lookup_object(MB_GAME(game), st_task->task_id);
  ASSERT(task && MBC_IS_TASK(task));
  ASSERT(!MB_TASK_ISGROUP(task)); /* [fixme] */
  mb_task_add_workers(MB_TASK(task), st_task->workers - MB_TASK_WORKERS(task));
  /* CL_DEBUG("task update: %s +%" G_GINT64_FORMAT " -> %" G_GINT64_FORMAT, */
  /*          MB_TASK_NAME(task), st_task->workers, MB_TASK_WORKERS(task)); */
}



/* _handle_room_setup:
 */
static void _handle_room_setup ( MbcGame *game,
                                 MbStateRoomSetup *st_room )
{
  MbObject *colony, *room;
  colony = mb_game_lookup_object(MB_GAME(game), st_room->colony_id);
  ASSERT(colony && MBC_IS_COLONY(colony));
  room = mbc_room_new(MB_OBJECT(game), st_room->room_id, st_room->type);
  mb_colony_add_room(MB_COLONY(colony), room);
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
        case MB_STATE_RESOURCE_SETUP:
          _handle_resource_setup(game, (MbStateResourceSetup *) block);
          break;
        case MB_STATE_SECTOR_SETUP:
          _handle_sector_setup(game, (MbStateSectorSetup *) block);
          break;
        case MB_STATE_COLONY_SETUP:
          _handle_colony_setup(game, (MbStateColonySetup *) block);
          break;
        case MB_STATE_PRIORITY_SETUP:
          _handle_priority_setup(game, (MbStatePrioritySetup *) block);
          break;
        case MB_STATE_TASK_SETUP:
          _handle_task_setup(game, (MbStateTaskSetup *) block);
          break;
        case MB_STATE_ROOM_SETUP:
          _handle_room_setup(game, (MbStateRoomSetup *) block);
          break;
        case MB_STATE_GAME_UPDATE:
          _handle_game_update(game, (MbStateGameUpdate *) block);
          break;
        case MB_STATE_COLONY_UPDATE:
          _handle_colony_update(game, (MbStateColonyUpdate *) block);
          break;
        case MB_STATE_PRIORITY_UPDATE:
          _handle_priority_update(game, (MbStatePriorityUpdate *) block);
          break;
        case MB_STATE_TASK_UPDATE:
          _handle_task_update(game, (MbStateTaskUpdate *) block);
          break;
        default:
          CL_ERROR("[TODO] block type %d", block->type);
        }
    }
  if (started) {
    l_signal_emit(L_OBJECT(game), signals[SIG_STARTED], 0);
  }
}



/* mbc_game_send_state:
 */
void mbc_game_send_state ( MbcGame *game,
                           MbState *state )
{
  game->handler(state, game->handler_data);
}
