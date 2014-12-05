/* mbtaskgroup.c -
 */

#include "mbtaskgroup.h"
#include "mbtaskgroup.inl"



/* mb_task_group_new_top:
 */
MbTask *mb_task_group_new_top ( struct _MbColony *colony,
                                const gchar *name )
{
  MbTask *task;
  task = mb_task_new(MB_CLASS_TASK_GROUP,
                     NULL,
                     name,
                     0);
  task->colony = colony; /* [FIXME] ref ? */
  return task;
}
