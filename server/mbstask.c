/* mbstask.c -
 */

#include "server/mbstask.h"
#include "server/mbstask.inl"



/* mbs_task_new:
 */
MbsTask *mbs_task_new ( MbsTaskFuncs *funcs )
{
  MbsTask *task = MBS_TASK_NEW(NULL);
  task->funcs = *funcs;
  return task;
}
