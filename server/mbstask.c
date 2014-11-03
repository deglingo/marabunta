/* mbstask.c -
 */

#include "server/srvprivate.h"
#include "server/mbstask.h"
#include "server/mbstask.inl"



/* mbs_task_new:
 */
MbsTask *mbs_task_new ( struct _MbsColony *colony,
                        MbsTaskFuncs *funcs )
{
  MbsTask *task = MBS_TASK_NEW(NULL);
  task->colony = colony;
  task->funcs = *funcs;
  return task;
}



/* mbs_task_check:
 */
gboolean mbs_task_check ( MbsTask *task,
                          MbPopType pop_type )
{
  return task->funcs.check(task, pop_type);
}



/* mbs_task_get_score:
 */
gint64 mbs_task_get_score ( MbsTask *task )
{
  return task->score;
}



/* mbs_task_adjust_workers:
 */
void mbs_task_adjust_workers ( MbsTask *task,
                               gint64 count )
{
  task->workers += count;
  ASSERT(task->workers >= 0);
}
