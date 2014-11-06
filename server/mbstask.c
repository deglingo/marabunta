/* mbstask.c -
 */

#include "server/srvprivate.h"
#include "server/mbstask.h"
#include "server/mbstask.inl"



static MbsTask *_task_new ( struct _MbsColony *colony,
                            MbsTask *parent,
                            const gchar *name,
                            MbsTaskFuncs *funcs,
                            gboolean isgroup )
{
  MbsTask *task = MBS_TASK_NEW(NULL);
  task->colony = colony;
  task->name = g_strdup(name);
  if (parent)
    {
      ASSERT(parent->isgroup);
      task->parent = parent;
      parent->children = g_list_append(parent->children,
                                       l_object_ref(task));
    }
  task->funcs = *funcs;
  task->isgroup = isgroup;
  return task;
}



/* mbs_task_new:
 */
MbsTask *mbs_task_new ( struct _MbsColony *colony,
                        MbsTask *parent,
                        const gchar *name,
                        MbsTaskFuncs *funcs )
{
  return _task_new(colony, parent, name, funcs, FALSE);
}



/* mbs_task_new_group:
 */
MbsTask *mbs_task_new_group ( struct _MbsColony *colony,
                              MbsTask *parent,
                              const gchar *name,
                              MbsTaskFuncs *funcs )
{
  return _task_new(colony, parent, name, funcs, TRUE);
}



/* mbs_task_check:
 */
gboolean mbs_task_check ( MbsTask *task,
                          MbPopType pop_type )
{
  return task->funcs.check(task, pop_type);
}



static void _process ( MbsTask *task )
{
  if (task->isgroup)
    {
      GList *l;
      for (l = task->children; l; l = l->next)
        _process(l->data);
    }
  else
    {
      task->funcs.process(task);
    }
}



/* mbs_task_process:
 */
void mbs_task_process ( MbsTask *task )
{
  _process(task);
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
