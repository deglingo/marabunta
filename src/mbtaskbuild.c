/* mbtaskbuild.c -
 */

#include "mbtaskbuild.h"
#include "mbtaskbuild.inl"



static void _process ( MbTask *task );



/* mb_task_build_class_init:
 */
static void mb_task_build_class_init ( LObjectClass *cls )
{
  MB_TASK_CLASS(cls)->process = _process;
}



/* mb_task_build_new:
 */
MbTask *mb_task_build_new ( MbTask *parent,
                            struct _MbRoom *room )
{
  MbTask *task;
  task = mb_task_new(MB_CLASS_TASK_BUILD,
                     parent,
                     "build",
                     MB_POP_FLAG_ADULT_WORKER);
  MB_TASK_BUILD(task)->room = l_object_ref(room);
  return task;
}



/* _process:
 */
static void _process ( MbTask *task )
{
  CL_DEBUG("[TODO] process build: %" G_GINT64_FORMAT, task->workers);
}
