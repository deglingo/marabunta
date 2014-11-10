/* mbsgame.c -
 */

#include "server/srvprivate.h"
#include "server/mbsgame.h"
#include "server/mbsapp.h"
#include "server/mbstask.h"
#include "server/mbscontrol.h"
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
  GHashTable *control_map;
}
  Private;

#define PRIVATE(game) ((Private *)(MBS_GAME(game)->private))



static void _send ( MbsGame *game,
                    Player *player,
                    MbMessage *message );



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
  PRIVATE(obj)->control_map = g_hash_table_new(NULL, NULL);
}



/* mbs_game_new:
 */
MbsGame *mbs_game_new ( void )
{
  return MBS_GAME_NEW(NULL);
}



static void _on_control_notify ( MbsControl *control,
                                 MbsGame *game )
{
  MbMessage *msg;
  MbState *state;
  MbStatePriority *st_prio;
  ASSERT(MBS_IS_PRIORITY(control));
  state = mb_state_new();
  st_prio = mb_state_next(state, MB_STATE_PRIORITY);
  st_prio->priority_id = MBS_OBJECT_ID(control);
  st_prio->value = MBS_PRIORITY(control)->value;
  msg = mb_message_new(MB_MESSAGE_KEY_GAME_STATE, L_OBJECT(state));
  _send(game, PRIVATE(game)->players[0], msg);
  l_object_unref(msg);
  l_object_unref(state);
}



/* mbs_game_register_control:
 */
void mbs_game_register_control ( MbsGame *game,
                                 MbsObject *control )
{
  ASSERT(MBS_IS_CONTROL(control));
  ASSERT(!mbs_game_lookup_control(game, MBS_OBJECT_ID(control)));
  /* [FIXME] weakref */
  g_hash_table_insert(PRIVATE(game)->control_map,
                      GUINT_TO_POINTER(MBS_OBJECT_ID(control)),
                      control);
  l_signal_connect(L_OBJECT(control),
                   "notify", g_quark_from_string("value"),
                   (LSignalHandler) _on_control_notify,
                   game,
                   NULL);
}



/* mbs_game_lookup_control:
 */
MbsObject *mbs_game_lookup_control ( MbsGame *game,
                                     MbsObjectID id )
{
  return g_hash_table_lookup(PRIVATE(game)->control_map,
                             GUINT_TO_POINTER(id));
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
  /* CL_DEBUG("send(player=%d, key=%d)", player->id, message->key); */
  player->message_handler(player->id, message, player->handler_data);
}



static void _pop_unit_update_egg ( MbsColony *colony,
                                   MbsPopUnit *unit )
{
  guint age;
  /* _pop_unit_affect_task(unit, NULL); */
  age = colony->sector->world->game->frame - unit->birthdate;
  if (age > 10)
    {
      MbPopType pop_type;
      pop_type = mbs_colony_select_hatch_cast(colony);
      mbs_colony_adjust_pop(colony, MB_POP_EGG, unit->birthdate, -unit->count);
      mbs_colony_adjust_pop(colony, pop_type, unit->birthdate, unit->count);
      mbs_colony_adjust_hatch_score(colony, pop_type, unit->count);
    }
}



static void _pop_unit_update_larvae ( MbsColony *colony,
                                      MbsPopUnit *unit )
{
  guint age;
  /* _pop_unit_affect_task(unit, NULL); */
  age = colony->sector->world->game->frame - unit->birthdate;
  if (age > 30)
    {
      MbCast pcast = MB_POP_CAST(unit->type);
      MbPopType pop_type = mb_pop_type(pcast, MB_MATURITY_ADULT);
      mbs_colony_adjust_pop(colony, unit->type, unit->birthdate, -unit->count);
      /* [fixme] just kill queens for now */
      if (pop_type != MB_POP_ADULT_QUEEN)
        mbs_colony_adjust_pop(colony, pop_type, unit->birthdate, unit->count);
    }
}



static void _pop_unit_update_aq ( MbsColony *colony,
                                  MbsPopUnit *unit )
{
  MbsTask *task;
  mbs_pop_unit_affect_task(unit, NULL);
  if ((task = mbs_colony_select_task(colony, MB_POP_ADULT_QUEEN)))
    {
      mbs_pop_unit_affect_task(unit, task);
    }
}



static void _pop_unit_update_aw ( MbsColony *colony,
                                  MbsPopUnit *unit )
{
  MbsTask *task;
  mbs_pop_unit_affect_task(unit, NULL);
  if ((task = mbs_colony_select_task(colony, MB_POP_ADULT_WORKER)))
    {
      mbs_pop_unit_affect_task(unit, task);
    }  
}



static void _pop_unit_update ( MbsPopUnit *unit,
                               gpointer data )
{
  MbsColony *col = data;
  switch(unit->type)
    {
    case MB_POP_EGG:
      _pop_unit_update_egg(col, unit);
      break;
    case MB_POP_LARVAE_QUEEN:
    case MB_POP_LARVAE_WORKER:
    case MB_POP_LARVAE_SOLDIER:
      /* [TODO] */
      _pop_unit_update_larvae(col, unit);
      break;
    case MB_POP_ADULT_QUEEN:
      _pop_unit_update_aq(col, unit);
      break;
    case MB_POP_ADULT_WORKER:
      _pop_unit_update_aw(col, unit);
      break;
    case MB_POP_ADULT_SOLDIER:
      /* [TODO] */
      break;
    default:
      do {} while (0);
      CL_DEBUG("[TODO] pop type %d", unit->type);
    }
}



/* _send_task_state:
 */
static void _send_task_state ( MbsGame *game,
                               MbsColony *colony,
                               MbsTask *task )
{
  Private *priv = PRIVATE(game);
  MbStateTask *st_task;
  GList *l;
  st_task = mb_state_next(priv->players[colony->owner]->state,
                          MB_STATE_TASK);
  st_task->task_id = MBS_OBJECT_ID(task);
  st_task->workers = task->workers;
  for (l = task->children; l; l = l->next)
    _send_task_state(game, colony, l->data);
}



/* _colony_update:
 */
static void _colony_update ( MbsGame *game,
                             MbsColony *colony )
{
  Private *priv = PRIVATE(game);
  MbStatePop *st_pop;
  gint tp;
  /* update the pop tree */
  mbs_pop_tree_traverse(colony->pop_tree, _pop_unit_update, colony);
  /* adjust pop tree */
  mbs_colony_update_pop_tree(colony);
  /* adjust the hatch scores */
  mbs_colony_adjust_hatch_scores(colony);
  /* process all the tasks */
  mbs_task_process(colony->top_task);
  /* send pop state */
  st_pop = (MbStatePop *) mb_state_next(priv->players[colony->owner]->state,
                                        MB_STATE_POP);
  st_pop->colony_id = MBS_OBJECT_ID(colony);
  for (tp = 0; tp < MB_POP_TYPE_COUNT; tp++)
    {
      st_pop->pop[tp] = colony->pop_tree->pop[tp];
    }
  /* send tasks state */
  _send_task_state(game, colony, colony->top_task);
}



/* _sector_update:
 */
static void _sector_update ( MbsGame *game,
                             MbsSector *sector )
{
  if (sector->colony)
    _colony_update(game, sector->colony);
}



/* _game_update:
 */
static void _game_update ( MbsGame *game )
{
  Private *priv = PRIVATE(game);
  gint p;
  gint x, y;
  game->frame++;
  /* CL_TRACE("%d", game->frame); */
  /* CL_TRACE("game=%p/%p, n_players=%d", game, priv, priv->n_players); */
  /* prepare the messages */
  for (p = 0; p < priv->n_players; p++)
    {
      Player *player = priv->players[p];
      /* CL_TRACE("%d", p); */
      MbStateFrame *st_frame;
      player->state = mb_state_new();
      st_frame = (MbStateFrame *) mb_state_next(player->state, MB_STATE_FRAME);
      st_frame->sim_time = game->frame;
    }
  /* game update */
  for (y = 0; y < game->world->height; y++)
    for (x = 0; x < game->world->width; x++)
      _sector_update(game, game->world->sectors[y][x]);
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
  game->world = mbs_world_new(game, 1, 2);
  mbs_sector_create_colony(game->world->sectors[0][0], 0);
  mbs_pop_tree_add(game->world->sectors[0][0]->colony->pop_tree,
                  MB_POP_ADULT_QUEEN, 0, 1);
  mbs_colony_create_room(game->world->sectors[0][0]->colony,
                         MB_ROOM_TYPE_ROYAL_CHAMBER);
}



static void _send_task_setup ( MbsGame *game,
                               Player *player,
                               MbState *state,
                               MbsColony *colony,
                               MbsTask *task )
{
  GList *l;
  MbStateNewPriority *st_prio = mb_state_next(state, MB_STATE_NEW_PRIORITY);
  MbStateNewTask *st_task = mb_state_next(state, MB_STATE_NEW_TASK);
  st_prio->priority_id = MBS_OBJECT_ID(task->priority);
  st_prio->value = task->priority->value;
  st_task->colony_id = MBS_OBJECT_ID(colony);
  st_task->task_id = MBS_OBJECT_ID(task);
  st_task->parent_id = task->parent ? MBS_OBJECT_ID(task->parent) : 0;
  st_task->isgroup = task->isgroup;
  st_task->priority_id = MBS_OBJECT_ID(task->priority);
  ASSERT(strlen(task->name) <= MBS_TASK_MAX_NAME);
  sprintf(st_task->name, task->name);
  st_task->workers = task->workers;
  for (l = task->children; l; l = l->next)
    _send_task_setup(game, player, state, colony, l->data);
}



/* _send_colony_setup:
 */
static void _send_colony_setup ( MbsGame *game,
                                 Player *player,
                                 MbState *state,
                                 MbsColony *colony )
{
  MbStateColony *st_colony = (MbStateColony *) mb_state_next(state, MB_STATE_COLONY);
  MbStatePop *st_pop = (MbStatePop *) mb_state_next(state, MB_STATE_POP);
  gint tp;
  st_colony->colony_id = MBS_OBJECT_ID(colony);
  st_colony->sector_id = MBS_OBJECT_ID(colony->sector);
  st_colony->owner = colony->owner;
  st_pop->colony_id = MBS_OBJECT_ID(colony);
  for (tp = 0; tp < MB_POP_TYPE_COUNT; tp++)
    st_pop->pop[tp] = colony->pop_tree->pop[tp];
  for (tp = 0; tp < MB_ROOM_TYPE_COUNT; tp++)
    {
      if (!colony->rooms[tp])
        continue;
      MbStateNewRoom *st_room = mb_state_next(state, MB_STATE_NEW_ROOM);
      st_room->room_id = MBS_OBJECT_ID(colony->rooms[tp]);
      st_room->type = tp;
      st_room->colony_id = MBS_OBJECT_ID(colony);
    }
  _send_task_setup(game, player, state, colony, colony->top_task);
}



/* _send_game_setup:
 */
static void _send_game_setup ( MbsGame *game,
                               Player *player )
{
  MbState *state = mb_state_new();
  MbMessage *msg = mb_message_new(MB_MESSAGE_KEY_GAME_SETUP, L_OBJECT(state));
  MbStateReset *reset;
  guint x, y;
  reset = (MbStateReset *) mb_state_next(state, MB_STATE_RESET);
  reset->game_id = MBS_OBJECT_ID(game);
  reset->world_id = MBS_OBJECT_ID(game->world);
  reset->world_width = game->world->width;
  reset->world_height = game->world->height;
  for (y = 0; y < game->world->height; y++)
    {
      for (x = 0; x < game->world->width; x++)
        {
          MbsSector *sector = game->world->sectors[y][x];
          MbStateSector *st_sector = (MbStateSector *) mb_state_next(state, MB_STATE_SECTOR);
          st_sector->sector_id = MBS_OBJECT_ID(sector);
          st_sector->x = x;
          st_sector->y = y;
          if (sector->colony)
            _send_colony_setup(game, player, state, sector->colony);
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
  game->fps = 10.0;
  game->next_frame = 0.0;
  game->timer = g_timer_new();
  g_timeout_add_full(MBS_PRIORITY_GAME_TIMER,
                     10,
                     (GSourceFunc) _on_game_timer,
                     game,
                     NULL);
  /* g_idle_add_full(MBS_PRIORITY_GAME_TIMER, */
  /*                 (GSourceFunc) _on_game_timer, */
  /*                 game, */
  /*                 NULL); */
}



/* mbs_game_handle_message:
 */
void mbs_game_handle_message ( MbsGame *game,
                               MbMessage *msg )
{
  switch (msg->key)
    {
    case MB_MESSAGE_KEY_REQUEST_PRIORITY:
      {
        guint priority_id;
        gint value;
        MbsObject *priority;
        ASSERT(L_IS_TUPLE(msg->arg));
        ASSERT(L_TUPLE_SIZE(msg->arg) == 2);
        ASSERT(L_IS_INT(L_TUPLE_ITEM(msg->arg, 0)));
        ASSERT(L_IS_INT(L_TUPLE_ITEM(msg->arg, 1)));
        priority_id = L_INT_VALUE(L_TUPLE_ITEM(msg->arg, 0));
        value = L_INT_VALUE(L_TUPLE_ITEM(msg->arg, 1));
        priority = mbs_game_lookup_control(game, priority_id);
        ASSERT(priority);
        ASSERT(MBS_IS_PRIORITY(priority));
        mbs_priority_set_value(MBS_PRIORITY(priority), value);
      }
      break;
    default:
      CL_DEBUG("[TODO] handle message: %d", msg->key);
    }
}
