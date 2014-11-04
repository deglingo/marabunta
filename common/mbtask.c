/* mbtask.c -
 */

#include "common/mbtask.h"
#include "common/mbtask.inl"



/* mb_task_new:
 */
MbTask *mb_task_new ( MbTask *parent,
                      const gchar *name )
{
  MbTask *task;
  task = MB_TASK_NEW(NULL);
  task->name = g_strdup(name);
  if (parent)
    {
      task->parent = parent;
      parent->subtasks = g_list_append(parent->subtasks, task);
    }
  return task;
}
