/* mbscolony.c -
 */

#include "server/mbscolony.h"
#include "server/mbssector.h"
#include "server/mbstask.h"
#include "server/mbscolony.inl"



/* t_spawn_process:
 */
static void t_spawn_process ( MbsTask *task )
{
}



/* mbs_colony_new:
 */
MbsColony *mbs_colony_new ( MbsSector *sector,
                            guint owner )
{
  MbsTaskFuncs t_spawn_funcs =
    {
      t_spawn_process,
    };
  MbsColony *col = MBS_COLONY_NEW(NULL);
  col->sector = sector;
  col->owner = owner;
  col->pop_tree = mb_pop_tree_new();
  col->tasks = g_list_append(col->tasks, mbs_task_new(&t_spawn_funcs));
  return col;
}
