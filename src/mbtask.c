/* mbtask.c -
 */

#include "mbtask.h"
#include "mbtaskgroup.h"
#include "mbtask.inl"



static MbTask *_select ( MbTask *task,
                         MbPopType pop_type );



/* mb_task_class_init:
 */
static void mb_task_class_init ( LObjectClass *cls )
{
  MB_TASK_CLASS(cls)->select = _select;
}



/* mb_task_new:
 */
MbTask *mb_task_new ( LObjectClass *cls,
                      MbTask *parent,
                      const gchar *name,
                      MbPopFlags pop_flags )
{
  MbTask *task;
  ASSERT(l_object_issubclass(L_OBJECT(cls),
                             L_OBJECT(MB_CLASS_TASK)));
  ASSERT((!parent) || MB_IS_TASK_GROUP(parent));
  task = MB_TASK(l_object_new(cls, NULL));
  task->name = g_strdup(name);
  task->pop_flags = pop_flags;
  if (parent)
    {
      MbTask *p;
      MB_TASK_GROUP(parent)->children = g_list_append(MB_TASK_GROUP(parent)->children,
                                                      l_object_ref(task));
      task->parent = parent;
      task->colony = parent->colony; /* [FIXME] ref ? */
      for (p = parent; p; p = p->parent)
        p->pop_flags |= pop_flags;
    }
  return task;
}



/* mb_task_select:
 */
MbTask *mb_task_select ( MbTask *task,
                         MbPopType pop_type )
{
  return MB_TASK_GET_CLASS(task)->select(task, pop_type);
}



/* _select:
 */
static MbTask *_select ( MbTask *task,
                         MbPopType pop_type )
{
  if (task->pop_flags & (1 << pop_type))
    {
      return task;
    }
  else
    {
      return NULL;
    }
}



/* mb_task_add_workers:
 */
void mb_task_add_workers ( MbTask *task,
                           gint64 workers )
{
  ASSERT(!MB_IS_TASK_GROUP(task));
  while (task)
    {
      task->workers += workers;
      task = task->parent;
    }
}



/* mb_task_next_score:
 */
gint64 mb_task_next_score ( MbTask *task )
{
  CL_DEBUG("[FIXME]");
  return 0;
}
