/* mbsgame.c -
 */

#include "server/srvprivate.h"
#include "server/mbsgame.h"
#include "server/mbsplayer.h"
#include "server/mbsresource.h"
#include "server/mbsworld.h"
#include "server/mbssector.h"
#include "server/mbscolony.h"
#include "server/mbspriority.h"
#include "server/mbstask.h"
#include "server/mbsvein.h"
#include "server/mbstechnology.h"
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
  guint frame_offset;
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



/* _register_resource:
 */
static void _register_resource ( MbsGame *game,
                                 const gchar *name,
                                 MbResourceFlags flags )
{
  MbObject *resource;
  resource = mbs_resource_new(MB_OBJECT(game), name, flags);
  mb_game_register_resource(MB_GAME(game), resource);
  l_object_unref(resource);
}



static void _add_vein ( MbsGame *game,
                        MbObject *sector,
                        const gchar *name,
                        gint depth,
                        gint64 qtty )
{
  MbObject *resource, *vein;
  resource = mb_game_lookup_resource(MB_GAME(game), name);
  ASSERT(resource);
  vein = mbs_vein_new(MB_OBJECT(game), resource, depth, qtty);
  mb_sector_add_vein(MB_SECTOR(sector), vein);
  l_object_unref(vein);
}



static void _register_room_tech ( MbsGame *game,
                                  const gchar *name )
{
  MbObject *tech;
  tech = mbs_technology_new(MB_OBJECT(game), name);
  mb_game_register_technology(MB_GAME(game), tech);
  l_object_unref(tech);
}



static MbRoomType _register_room_type ( MbsGame *game,
                                        const gchar *nick,
                                        const gchar *name,
                                        const gchar *work_task )
{
  MbRoomType type = mb_game_next_room_type(MB_GAME(game));
  mb_game_register_room_type(MB_GAME(game), type, nick, name, work_task);
  return type;
}



static void _setup_rooms ( MbsGame *game )
{
  /* MbRoomType type; */
  /* type = */ _register_room_type(game,
                             "royal-chamber",
                                   "Royal chamber",
                                   "spawn");
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
  /* standard resources */
  _register_resource(game, "food", MB_RESOURCE_FOOD);
  /* mineral resources (should be random) */
  _register_resource(game, "mr1", MB_RESOURCE_MINERAL);
  _register_resource(game, "mr2", MB_RESOURCE_MINERAL);
  _register_resource(game, "mr3", MB_RESOURCE_MINERAL);
  /* create the veins */
  _add_vein(game, MB_WORLD_SECTOR(MB_GAME_WORLD(game), 0, 0), "mr1", 0, 100000);
  _add_vein(game, MB_WORLD_SECTOR(MB_GAME_WORLD(game), 0, 0), "mr2", 0, 1000000);
  _add_vein(game, MB_WORLD_SECTOR(MB_GAME_WORLD(game), 0, 0), "mr3", 0, 1000000000);
  /* setup the technologies */
  _register_room_tech(game, "farm1");
  /* setup the room types */
  _setup_rooms(game);
  /* [FIXME] */
  colony = mbs_colony_new(MB_OBJECT(game));
  ASSERT(MB_GAME(game)->players);
  mb_colony_set_owner(MB_COLONY(colony), MB_GAME(game)->players->data);
  mb_sector_set_colony(MB_SECTOR(MB_WORLD_SECTOR(world, 0, 0)), colony);
}



/* /\* _send_task_setup: */
/*  *\/ */
/* static void _send_task_setup ( MbsGame *game, */
/*                                MbState *state, */
/*                                MbObject *task ) */
/* { */
/*   MbStatePrioritySetup *st_prio; */
/*   MbStateTaskSetup *st_task; */
/*   st_prio = mb_state_next(state, MB_STATE_PRIORITY_SETUP); */
/*   st_prio->priority_id = MB_OBJECT_ID(MB_TASK_PRIORITY(task)); */
/*   st_prio->value = MB_PRIORITY_VALUE(MB_TASK_PRIORITY(task)); */
/*   st_task = mb_state_next(state, MB_STATE_TASK_SETUP); */
/*   st_task->task_id = MB_OBJECT_ID(task); */
/*   st_task->priority_id = MB_OBJECT_ID(MB_TASK_PRIORITY(task)); */
/*   st_task->colony_id = MB_OBJECT_ID(MB_TASK_COLONY(task)); */
/*   st_task->parent_id = MB_TASK_PARENT(task) ? MB_OBJECT_ID(MB_TASK_PARENT(task)) : 0; */
/*   st_task->resource_id = MB_TASK_RESOURCE(task) ? MB_OBJECT_ID(MB_TASK_RESOURCE(task)) : 0; */
/*   ASSERT(strlen(MB_TASK_NAME(task)) <= MB_TASK_MAX_NAME); */
/*   sprintf(st_task->name, MB_TASK_NAME(task)); */
/*   st_task->isgroup = MB_TASK_ISGROUP(task); */
/*   if (MB_TASK_ISGROUP(task)) */
/*     { */
/*       GList *l; */
/*       for (l = MB_TASK_CHILDREN(task); l; l = l->next) */
/*         { */
/*           _send_task_setup(game, state, l->data); */
/*         } */
/*     } */
/* } */



/* /\* _send_colony_setup: */
/*  *\/ */
/* static void _send_colony_setup ( MbsGame *game, */
/*                                  MbState *state, */
/*                                  MbObject *colony ) */
/* { */
/*   MbStateColonySetup *st_col; */
/*   gint tp; */
/*   st_col = mb_state_next(state, MB_STATE_COLONY_SETUP); */
/*   st_col->colony_id = MB_OBJECT_ID(colony); */
/*   st_col->sector_id = MB_OBJECT_ID(MB_COLONY_SECTOR(colony)); */
/*   st_col->owner_id = MB_OBJECT_ID(MB_COLONY_OWNER(colony)); */
/*   _send_task_setup(game, state, MB_COLONY_TOP_TASK(colony)); */
/*   for (tp = 0; tp < MB_ROOM_TYPE_COUNT; tp++) */
/*     { */
/*       MbObject *room = MB_COLONY_ROOM(colony, tp); */
/*       MbStateRoomSetup *st_room; */
/*       if (!room) */
/*         continue; */
/*       st_room = mb_state_next(state, MB_STATE_ROOM_SETUP); */
/*       st_room->room_id = MB_OBJECT_ID(room); */
/*       st_room->colony_id = MB_OBJECT_ID(MB_ROOM_COLONY(room)); */
/*       st_room->type = MB_ROOM_TYPE(room); */
/*     } */
/* } */



/* /\* _send_sector_setup: */
/*  *\/ */
/* static void _send_sector_setup ( MbsGame *game, */
/*                                  MbState *state, */
/*                                  MbObject *sector ) */
/* { */
/*   MbStateSectorSetup *st_sector; */
/*   st_sector = mb_state_next(state, MB_STATE_SECTOR_SETUP); */
/*   st_sector->sector_id = MB_OBJECT_ID(sector); */
/*   st_sector->x = MB_SECTOR_X(sector); */
/*   st_sector->y = MB_SECTOR_Y(sector); */
/*   if (MB_SECTOR_COLONY(sector)) */
/*     _send_colony_setup(game, state, MB_SECTOR_COLONY(sector)); */
/* } */



/* /\* _send_resources_setup: */
/*  *\/ */
/* static void _send_resources_setup ( MbsGame *game, */
/*                                     MbState *state ) */
/* { */
/*   GHashTableIter iter; */
/*   MbStateResourceSetup *st_rsc; */
/*   gpointer key, rsc; */
/*   g_hash_table_iter_init(&iter, MB_GAME(game)->resources); */
/*   while (g_hash_table_iter_next(&iter, &key, &rsc)) */
/*     { */
/*       st_rsc = mb_state_next(state, MB_STATE_RESOURCE_SETUP); */
/*       st_rsc->resource_id = MB_OBJECT_ID(rsc); */
/*       ASSERT(strlen(MB_RESOURCE_NAME(rsc)) <= MB_RESOURCE_MAX_NAME); */
/*       sprintf(st_rsc->name, MB_RESOURCE_NAME(rsc)); */
/*       st_rsc->flags = MB_RESOURCE_FLAGS(rsc); */
/*     } */
/* } */



/* /\* _send_game_setup: */
/*  *\/ */
/* static void _send_game_setup ( MbsGame *game, */
/*                                MbsPlayer *player ) */
/* { */
/*   MbState *state = mb_state_new(); */
/*   MbStateGameSetup *st_game; */
/*   GList *l; */
/*   guint n; */
/*   guint x, y; */
/*   st_game = mb_state_next(state, MB_STATE_GAME_SETUP); */
/*   /\* players *\/ */
/*   st_game->n_players = g_list_length(MB_GAME(game)->players); */
/*   for (l = MB_GAME(game)->players, n = 0; l; l = l->next, n++) */
/*     { */
/*       MbsPlayer *p = l->data; */
/*       ASSERT(n < MB_GAME_MAX_PLAYERS); */
/*       ASSERT(MB_PLAYER(p)->name->len <= MB_PLAYER_MAX_NAME); */
/*       st_game->players[n].id = MB_OBJECT_ID(p); */
/*       sprintf(st_game->players[n].name, MB_PLAYER(p)->name->str); */
/*     } */
/*   /\* resources *\/ */
/*   _send_resources_setup(game, state); */
/*   /\* world *\/ */
/*   st_game->world_id = MB_OBJECT_ID(MB_GAME_WORLD(game)); */
/*   st_game->world_width = MB_WORLD_WIDTH(MB_GAME_WORLD(game)); */
/*   st_game->world_height = MB_WORLD_HEIGHT(MB_GAME_WORLD(game)); */
/*   for (y = 0; y < st_game->world_height; y++) */
/*     for (x = 0; x < st_game->world_width; x++) */
/*       _send_sector_setup(game, state, MB_WORLD_SECTOR(MB_GAME_WORLD(game), x, y)); */
/*   /\* send *\/ */
/*   mb_player_handle_state(MB_PLAYER(player), state); */
/*   l_object_unref(state); */
/* } */



static void _game_update ( MbsGame *game )
{
  mbs_world_update(MBS_WORLD(MB_GAME_WORLD(game)));
  mb_game_set_frame_count(MB_GAME(game),
                          MB_GAME_FRAME_COUNT(game) + 1);
}



/* static void _send_task_update ( MbsGame *game, */
/*                                 PlayerData *player, */
/*                                 MbsTask *task ) */
/* { */
/*   if (MB_TASK_ISGROUP(task)) */
/*     { */
/*       GList *l; */
/*       for (l = MB_TASK_CHILDREN(task); l; l = l->next) */
/*         _send_task_update(game, player, l->data); */
/*     } */
/*   else */
/*     { */
/*       MbStateTaskUpdate *st_task; */
/*       st_task = mb_state_next(player->state, MB_STATE_TASK_UPDATE); */
/*       st_task->task_id = MB_OBJECT_ID(task); */
/*       st_task->workers = MB_TASK_WORKERS(task); */
/*     } */
/* } */



/* static void _send_colony_update ( MbsGame *game, */
/*                                   PlayerData *player, */
/*                                   MbsColony *colony ) */
/* { */
/*   MbStateColonyUpdate *st_col; */
/*   st_col = mb_state_next(player->state, MB_STATE_COLONY_UPDATE); */
/*   st_col->colony_id = MB_OBJECT_ID(colony); */
/*   if (MB_COLONY_OWNER(colony) == player->player) */
/*     { */
/*       MbColonyStockIter iter; */
/*       MbObject *rsc; */
/*       gint64 qtty; */
/*       gint i; */
/*       mbs_colony_get_pop(colony, */
/*                          st_col->pop); */
/*       ASSERT(mb_colony_stock_size(MB_COLONY(colony)) < MB_RESOURCE_MAX_TYPES); */
/*       st_col->stock_size = mb_colony_stock_size(MB_COLONY(colony)); */
/*       mb_colony_stock_iter_init(&iter, MB_COLONY(colony)); */
/*       for (i = 0; mb_colony_stock_iter_next(&iter, &rsc, &qtty); i++) */
/*         { */
/*           st_col->stock[i].resource_id = MB_OBJECT_ID(rsc); */
/*           st_col->stock[i].qtty = qtty; */
/*         } */
/*       _send_task_update(game, player, MBS_TASK(MB_COLONY_TOP_TASK(colony))); */
/*     } */
/* } */



/* static void _send_sector_update ( MbsGame *game, */
/*                                   PlayerData *player, */
/*                                   MbsSector *sector ) */
/* { */
/*   if (MB_SECTOR_COLONY(sector)) */
/*     _send_colony_update(game, player, */
/*                         MBS_COLONY(MB_SECTOR_COLONY(sector))); */
/* } */



/* static void _send_world_update ( MbsGame *game, */
/*                                  PlayerData *player, */
/*                                  MbsWorld *world ) */
/* { */
/*   guint x, y; */
/*   for (y = 0; y < MB_WORLD_HEIGHT(world); y++) */
/*     for (x = 0; x < MB_WORLD_WIDTH(world); x++) */
/*       _send_sector_update(game, player, */
/*                           MBS_SECTOR(MB_WORLD_SECTOR(world, x, y))); */
/* } */



/* static void _send_game_update ( MbsGame *game ) */
/* { */
/*   Private *priv = PRIVATE(game); */
/*   GList *l; */
/*   for (l = priv->players; l; l = l->next) */
/*     { */
/*       PlayerData *player = l->data; */
/*       MbStateGameUpdate *st_game; */
/*       st_game = mb_state_next(player->state, MB_STATE_GAME_UPDATE); */
/*       st_game->frame_count = MB_GAME_FRAME_COUNT(game); */
/*       _send_world_update(game, player, MBS_WORLD(MB_GAME_WORLD(game))); */
/*       mb_player_handle_state(MB_PLAYER(player->player), */
/*                              player->state); */
/*       L_OBJECT_CLEAR(player->state); */
/*     } */
/* } */



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
      priv->next_frame = ((gdouble) (MB_GAME_FRAME_COUNT(game) - priv->frame_offset)) / MBS_GAME_FPS;
    }
  /* _send_game_update(game); */
  return G_SOURCE_CONTINUE;
}



/* mbs_game_start:
 */
void mbs_game_start ( MbsGame *game )
{
  Private *priv = PRIVATE(game);
  /* GList *l; */
  /* for (l = MB_GAME(game)->players; l; l = l->next) */
  /*   { */
  /*     MbsPlayer *player = l->data; */
  /*     _send_game_setup(game, player); */
  /*   } */
  mb_game_started(MB_GAME(game));
  /* [FIXME] quick start */
  {
    MbObject *col = MB_SECTOR_COLONY(MB_WORLD_SECTOR(MB_GAME_WORLD(game), 0, 0));
    while (MB_COLONY(col)->pop[MB_POP_ADULT_WORKER] == 0)
      _game_update(game);
    priv->frame_offset = MB_GAME_FRAME_COUNT(game);
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



/* mbs_game_handle_request:
 */
void mbs_game_handle_request ( MbsGame *game,
                               MbObject *player,
                               MbState *state )
{
  guint offset = 0;
  MbStateBlock *block;
  while ((block = mb_state_read(state, &offset)))
    {
      switch (block->type)
        {
        case MB_STATE_REQUEST_PRIORITY_VALUE:
          {
            MbStateRequestPriorityValue *st_prio = (gpointer) block;
            MbObject *prio;
            MbState *answer_state;
            MbStatePriorityUpdate *st_answer;
            prio = mb_game_lookup_object(MB_GAME(game), st_prio->priority_id);
            ASSERT(prio && MBS_IS_PRIORITY(prio));
            mb_priority_set_value(MB_PRIORITY(prio), st_prio->value);
            answer_state = mb_state_new();
            st_answer = mb_state_next(answer_state, MB_STATE_PRIORITY_UPDATE);
            st_answer->priority_id = MB_OBJECT_ID(prio);
            st_answer->value = MB_PRIORITY_VALUE(prio);
            mb_player_handle_state(MB_PLAYER(player), answer_state);
            l_object_unref(answer_state);
          }
          break;
        default:
          CL_ERROR("[TODO] block type %d", block->type);
        }
    }
}
