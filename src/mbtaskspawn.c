/* mbtaskspawn.c -
 */

#include "mbtaskspawn.h"
#include "mbtaskspawn.inl"



static void _process ( MbTask *task );



/* mb_task_spawn_class_init:
 */
static void mb_task_spawn_class_init ( LObjectClass *cls )
{
  MB_TASK_CLASS(cls)->process = _process;
}




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



/* _process:
 */
static void _process ( MbTask *task )
{
  CL_DEBUG("[TODO] %" G_GINT64_FORMAT " workers", task->workers);
}
