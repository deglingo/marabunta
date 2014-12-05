/* mbtaskspawn.c -
 */

#include "mbtaskspawn.h"
#include "mbtaskspawn.inl"



/* mb_task_spawn_new:
 */
MbTask *mb_task_spawn_new ( MbTask *parent,
                            const gchar *name )
{
  MbTask *task;
  task = mb_task_new(MB_CLASS_TASK_SPAWN,
                     parent,
                     name,
                     MB_POP_FLAG_ADULT_QUEEN);
  return task;
}
