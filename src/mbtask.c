/* mbtask.c -
 */

#include "mbtask.h"
#include "mbtaskgroup.h"
#include "mbtask.inl"



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
