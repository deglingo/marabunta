/* mbscolony.c -
 */

#include "server/srvprivate.h"
#include "server/mbscolony.h"
#include "server/mbstask.h"
#include "server/mbspriority.h"
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
                                   MbObject **prio,
                                   gint64 **score )
{
  if (colony->hatch_score_queen <= colony->hatch_score_worker &&
      colony->hatch_score_queen <= colony->hatch_score_soldier)
    {
      *prio = colony->hatch_priority_queen;
      *score = &colony->hatch_score_queen;
      return MB_CAST_QUEEN;
    }
  else if (colony->hatch_score_worker <= colony->hatch_score_queen &&
           colony->hatch_score_worker <= colony->hatch_score_soldier)
    {
      *prio = colony->hatch_priority_worker;
      *score = &colony->hatch_score_worker;
      return MB_CAST_WORKER;
    }
  else
    {
      *prio = colony->hatch_priority_soldier;
      *score = &colony->hatch_score_soldier;
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
      gint64 *score;
      MbCast pop_cast = _select_hatch_cast(colony, &priority, &score);
      MbPopType pop_type = mb_pop_type(pop_cast, MB_MATURITY_LARVAE);
      mbs_pop_tree_add(colony->adj_tree, unit->type, unit->birthdate, -unit->count);
      mbs_pop_tree_add(colony->adj_tree, pop_type, unit->birthdate, unit->count);
      *score += unit->count * MB_PRIORITY_SCORE_FACTOR(priority);
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
    case MB_POP_ADULT_QUEEN:
      _update_aq(colony, unit);
      break;
    default:
      CL_DEBUG("[TODO] type %d", unit->type);
    }
}



/* mbs_colony_update:
 */
void mbs_colony_update ( MbsColony *colony )
{
  mbs_pop_tree_traverse(colony->pop_tree, _update_pop_unit, colony);
  /* [FIXME] post update */
  mbs_pop_tree_update(colony->pop_tree, colony->adj_tree);
}
