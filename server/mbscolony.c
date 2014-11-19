/* mbscolony.c -
 */

#include "server/srvprivate.h"
#include "server/mbscolony.h"
#include "server/mbstask.h"
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



/* _update_egg:
 */
static void _update_egg ( MbsColony *colony,
                          MbsPopUnit *unit )
{
  /* [TODO] */
}



/* _update_aq:
 */
static void _update_aq ( MbsColony *colony,
                         MbsPopUnit *unit )
{
  guint date = MB_GAME_FRAME_COUNT(MB_OBJECT_GAME(colony));
  mbs_pop_tree_add(colony->adj_tree, MB_POP_EGG, date, 10);
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
      CL_ERROR("[TODO] type %d", unit->type);
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

