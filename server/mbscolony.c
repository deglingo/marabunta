/* mbscolony.c -
 */

#include "server/srvprivate.h"
#include "server/mbscolony.h"
#include "server/mbstask.h"
#include "server/mbspriority.h"
#include "server/mbsroom.h"
#include "server/mbscolony.inl"



/* mbs_colony_new:
 */
MbObject *mbs_colony_new ( MbObject *game )
{
  MbObject *col = MB_OBJECT(l_object_new(MBS_CLASS_COLONY,
                                         "game", game,
                                         NULL));
  MbObject *t_top, *t_spawn;
  MbsTaskFuncs t_spawn_funcs = { NULL, };
  MbObject *room;
  /* create the pop trees */
  MBS_COLONY(col)->pop_tree = mbs_pop_tree_new();
  MBS_COLONY(col)->adj_tree = mbs_pop_tree_new();
  /* hatch priorities */
  MBS_COLONY(col)->hatch_priority_queen =
    mbs_priority_new(game, 1);
  MBS_COLONY(col)->hatch_priority_worker =
    mbs_priority_new(game, 7);
  MBS_COLONY(col)->hatch_priority_soldier =
    mbs_priority_new(game, 2);
  /* [FIXME] */
  mbs_pop_tree_add(MBS_COLONY(col)->pop_tree,
                   MB_POP_ADULT_QUEEN,
                   0, 1);
  /* create the default rooms */
  room = mbs_room_new(game, MB_ROOM_TYPE_ROYAL_CHAMBER);
  mb_colony_add_room(MB_COLONY(col), room);
  /* create the default tasks */
  /* [fixme] is it the right place for this ? */
  t_top = mbs_task_new_group(game, "top", MB_POP_FLAG_ALL);
  mb_colony_set_top_task(MB_COLONY(col), t_top);
  l_object_unref(t_top);
  /* spawn */
  t_spawn = mbs_task_new(game, "spawn", MB_POP_FLAG_ADULT_QUEEN, &t_spawn_funcs);
  mb_task_add(MB_TASK(t_top), t_spawn);
  l_object_unref(t_spawn);
  return col;
}



/* mbs_colony_get_pop:
 */
void mbs_colony_get_pop ( MbsColony *colony,
                          gint64 *pop )
{
  mbs_pop_tree_get_pop(colony->pop_tree, pop);
}



static MbCast _select_hatch_cast ( MbsColony *colony,
                                   MbObject **prio )
{
  if (MB_PRIORITY_SCORE(colony->hatch_priority_queen) <= MB_PRIORITY_SCORE(colony->hatch_priority_worker) &&
      MB_PRIORITY_SCORE(colony->hatch_priority_queen) <= MB_PRIORITY_SCORE(colony->hatch_priority_soldier))
    {
      *prio = colony->hatch_priority_queen;
      return MB_CAST_QUEEN;
    }
  else if (MB_PRIORITY_SCORE(colony->hatch_priority_worker) <= MB_PRIORITY_SCORE(colony->hatch_priority_queen) &&
           MB_PRIORITY_SCORE(colony->hatch_priority_worker) <= MB_PRIORITY_SCORE(colony->hatch_priority_soldier))
    {
      *prio = colony->hatch_priority_worker;
      return MB_CAST_WORKER;
    }
  else
    {
      *prio = colony->hatch_priority_soldier;
      return MB_CAST_SOLDIER;
    }
}



/* _update_egg:
 */
static void _update_egg ( MbsColony *colony,
                          MbsPopUnit *unit )
{
  guint date = MB_GAME_FRAME_COUNT(MB_OBJECT_GAME(colony));
  guint age = date - unit->birthdate;
  if (age > 50)
    {
      MbObject *priority;
      MbCast pop_cast = _select_hatch_cast(colony, &priority);
      MbPopType pop_type = mb_pop_type(pop_cast, MB_MATURITY_LARVAE);
      mbs_pop_tree_add(colony->adj_tree, unit->type, unit->birthdate, -unit->count);
      mbs_pop_tree_add(colony->adj_tree, pop_type, unit->birthdate, unit->count);
      mb_priority_adjust_score(MB_PRIORITY(priority),
                               unit->count);
    }
}



/* _update_lq:
 */
static void _update_lq ( MbsColony *colony,
                         MbsPopUnit *unit )
{
  guint date = MB_GAME_FRAME_COUNT(MB_OBJECT_GAME(colony));
  guint age = date - unit->birthdate;
  if (age > 100)
    {
      mbs_pop_tree_add(colony->adj_tree, MB_POP_LARVAE_QUEEN, unit->birthdate, -unit->count);
      /* no new queens for now */
      /* mbs_pop_tree_add(colony->adj_tree, MB_POP_ADULT_QUEEN, unit->birthdate, unit->count); */
    }
}



/* _update_lw:
 */
static void _update_lw ( MbsColony *colony,
                         MbsPopUnit *unit )
{
  guint date = MB_GAME_FRAME_COUNT(MB_OBJECT_GAME(colony));
  guint age = date - unit->birthdate;
  if (age > 100)
    {
      mbs_pop_tree_add(colony->adj_tree, MB_POP_LARVAE_WORKER, unit->birthdate, -unit->count);
      mbs_pop_tree_add(colony->adj_tree, MB_POP_ADULT_WORKER, unit->birthdate, unit->count);
    }
}



/* _update_ls:
 */
static void _update_ls ( MbsColony *colony,
                         MbsPopUnit *unit )
{
  guint date = MB_GAME_FRAME_COUNT(MB_OBJECT_GAME(colony));
  guint age = date - unit->birthdate;
  if (age > 100)
    {
      mbs_pop_tree_add(colony->adj_tree, MB_POP_LARVAE_SOLDIER, unit->birthdate, -unit->count);
      mbs_pop_tree_add(colony->adj_tree, MB_POP_ADULT_SOLDIER, unit->birthdate, unit->count);
    }
}



/* _update_aq:
 */
static void _update_aq ( MbsColony *colony,
                         MbsPopUnit *unit )
{
  guint date = MB_GAME_FRAME_COUNT(MB_OBJECT_GAME(colony));
  gint count = g_random_int_range(0, 10 * unit->count);
  mbs_pop_tree_add(colony->adj_tree, MB_POP_EGG, date, count);
}



/* _update_aw:
 */
static void _update_aw ( MbsColony *colony,
                         MbsPopUnit *unit )
{
  /* [TODO] */
}



/* _update_as:
 */
static void _update_as ( MbsColony *colony,
                         MbsPopUnit *unit )
{
  /* [TODO] */
}



/* _update_pop_unit:
 */
static void _update_pop_unit ( MbsPopUnit *unit,
                               gpointer data )
{
  MbsColony *colony = MBS_COLONY(data);
  switch (unit->type)
    {
    case MB_POP_EGG:
      _update_egg(colony, unit);
      break;
    case MB_POP_LARVAE_QUEEN:
      _update_lq(colony, unit);
      break;
    case MB_POP_LARVAE_WORKER:
      _update_lw(colony, unit);
      break;
    case MB_POP_LARVAE_SOLDIER:
      _update_ls(colony, unit);
      break;
    case MB_POP_ADULT_QUEEN:
      _update_aq(colony, unit);
      break;
    case MB_POP_ADULT_WORKER:
      _update_aw(colony, unit);
      break;
    case MB_POP_ADULT_SOLDIER:
      _update_as(colony, unit);
      break;
    default:
      CL_DEBUG("[TODO] type %d", unit->type);
    }
}



/* mbs_colony_update:
 */
void mbs_colony_update ( MbsColony *colony )
{
  gint64 min_score;
  mbs_pop_tree_traverse(colony->pop_tree, _update_pop_unit, colony);
  /* adjust hatch scores */
  min_score = MIN(MB_PRIORITY_SCORE(colony->hatch_priority_queen),
                  MB_PRIORITY_SCORE(colony->hatch_priority_worker));
  min_score = MIN(min_score,
                  MB_PRIORITY_SCORE(colony->hatch_priority_soldier));
  MB_PRIORITY(colony->hatch_priority_queen)->score.score -= min_score;
  MB_PRIORITY(colony->hatch_priority_worker)->score.score -= min_score;
  MB_PRIORITY(colony->hatch_priority_soldier)->score.score -= min_score;
  /* [FIXME] post update */
  mbs_pop_tree_update(colony->pop_tree, colony->adj_tree);
}
