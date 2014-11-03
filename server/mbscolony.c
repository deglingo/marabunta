/* mbscolony.c -
 */

#include "server/srvprivate.h"
#include "server/mbscolony.h"
#include "server/mbssector.h"
#include "server/mbstask.h"
#include "server/mbscolony.inl"



/* t_spawn_check:
 */
static gboolean t_spawn_check ( MbsTask *task,
                                MbPopType pop_type )
{
  return pop_type == MB_POP_ADULT_QUEEN;
}



/* t_spawn_process:
 */
static void t_spawn_process ( MbsTask *task )
{
  CL_DEBUG("process: t_spawn(%qd)", task->workers);
}



/* mbs_colony_new:
 */
MbsColony *mbs_colony_new ( MbsSector *sector,
                            guint owner )
{
  MbsTaskFuncs t_spawn_funcs =
    {
      t_spawn_check,
      t_spawn_process,
    };
  MbsColony *col = MBS_COLONY_NEW(NULL);
  col->sector = sector;
  col->owner = owner;
  col->pop_tree = mb_pop_tree_new();
  col->tasks = g_list_append(col->tasks, mbs_task_new(col, &t_spawn_funcs));
  return col;
}



/* mbs_colony_select_task:
 */
MbsTask *mbs_colony_select_task ( MbsColony *colony,
                                  MbPopType pop_type )
{
  GList *l;
  MbsTask *found = NULL;
  gint64 found_score;
  for (l = colony->tasks; l; l = l->next)
    {
      MbsTask *task = l->data;
      if (mbs_task_check(task, pop_type))
        {
          gint64 score = mbs_task_get_score(task);
          if ((!found) || score < found_score)
            {
              found = task;
              found_score = score;
            }
        }
    }
  return found;
}
