/* mbstask.c -
 */

#include "server/srvprivate.h"
#include "server/mbstask.h"
#include "server/mbspriority.h"
#include "server/mbstask.inl"



static void add ( MbTask *task,
                  MbObject *object );



/* mbs_task_class_init:
 */
static void mbs_task_class_init ( LObjectClass *cls )
{
  MB_TASK_CLASS(cls)->add = add;
}



/* mbs_task_new_group:
 */
MbObject *mbs_task_new_group ( MbObject *game,
                               const gchar *name )
{
  MbObject *task = MB_OBJECT(l_object_new_give(MBS_CLASS_TASK,
                                               "game", l_object_ref(game),
                                               "name", l_string_new(name),
                                               "isgroup", l_int_new(1),
                                               "priority", mbs_priority_new(game, 5),
                                               NULL));
  return task;
}



/* mbs_task_new:
 */
MbObject *mbs_task_new ( MbObject *game,
                         const gchar *name,
                         MbPopFlags pop_flags,
                         MbsTaskFuncs *funcs )
{
  MbObject *task = MB_OBJECT(l_object_new_give(MBS_CLASS_TASK,
                                               "game", l_object_ref(game),
                                               "name", l_string_new(name),
                                               "isgroup", l_int_new(0),
                                               "priority", mbs_priority_new(game, 5),
                                               NULL));
  MBS_TASK(task)->funcs = *funcs;
  mbs_task_set_ready(MBS_TASK(task), pop_flags);
  if (funcs->init)
    funcs->init(MBS_TASK(task));
  return task;
}



/* mbs_task_check:
 *
 * Sanity check.
 *
 */
void mbs_task_check ( MbsTask *task )
{
  guint ready_sum = 0;
  gint tp;
  if (MB_TASK_ISGROUP(task))
    {
      GList *l;
      guint ready_count[MB_POP_TYPE_COUNT] = { 0, };
      for (l = MB_TASK_CHILDREN(task); l; l = l->next)
        {
          MbsTask *child = MBS_TASK(l->data);
          mbs_task_check(child);
          for (tp = 0; tp < MB_POP_TYPE_COUNT; tp++)
            {
              if (child->ready_count[tp])
                {
                  ready_count[tp]++;
                  ready_sum++;
                }
            }
        }
      for (tp = 0; tp < MB_POP_TYPE_COUNT; tp++)
        if (task->ready_count[tp] != ready_count[tp])
          CL_ERROR("task '%s' : wrong ready count (tp %d, got %d, expected %d)",
                   MB_TASK_NAME(task), tp, task->ready_count[tp], ready_count[tp]);
    }
  else
    {
      for (tp = 0; tp < MB_POP_TYPE_COUNT; tp++)
        {
          ASSERT(task->ready_count[tp] <= 1);
          ready_sum += task->ready_count[tp];
        }
    }
  ASSERT(task->ready_sum == ready_sum);
}



static void _inc_ready ( MbsTask *task,
                         MbPopFlags flags )
{
  gint tp;
  MbPopFlags set_flags = 0;
  for (tp = 0; flags; tp++, flags >>= 1)
    {
      ASSERT(tp < MB_POP_TYPE_COUNT);
      if ((flags & 1))
        {
          if (!task->ready_count[tp])
            set_flags |= (1 << tp);
          task->ready_count[tp]++;
          task->ready_sum++;
        }
    }
  if (set_flags && MB_TASK_PARENT(task))
    _inc_ready(MBS_TASK(MB_TASK_PARENT(task)), set_flags);  
}



/* add:
 */
static void add ( MbTask *task,
                  MbObject *child )
{
  gint tp;
  MbPopFlags set_flags = 0;
  MB_TASK_CLASS(parent_class)->add(task, child);
  for (tp = 0; tp < MB_POP_TYPE_COUNT; tp++)
    {
      if (MBS_TASK(child)->ready_count[tp])
        set_flags |= (1 << tp);
    }
  if (set_flags)
    _inc_ready(MBS_TASK(task), set_flags);
}



/* mbs_task_set_ready:
 */
void mbs_task_set_ready ( MbsTask *task,
                          MbPopFlags flags )
{
  gint tp;
  MbPopFlags set_flags = 0;
  ASSERT(!MB_TASK_ISGROUP(task));
  for (tp = 0; flags; tp++, flags >>= 1)
    {
      ASSERT(tp < MB_POP_TYPE_COUNT);
      if ((flags & 1) && !task->ready_count[tp])
        {
          task->ready_count[tp] = 1;
          task->ready_sum++;
          set_flags |= (1 << tp);
        }
    }
  if (set_flags && MB_TASK_PARENT(task))
    _inc_ready(MBS_TASK(MB_TASK_PARENT(task)), set_flags);
}



/* mbs_task_ready:
 */
gboolean mbs_task_ready ( MbsTask *task )
{
  return task->ready_sum;
}



/* mbs_task_accept:
 */
gboolean mbs_task_accept ( MbsTask *task,
                           MbPopType pop_type )
{
  return task->ready_count[pop_type];
}



/* mbs_task_next_score:
 */
gint64 mbs_task_next_score ( MbsTask *task )
{
  return mbs_priority_next_score(MBS_PRIORITY(MB_TASK_PRIORITY(task)),
                                 MB_TASK_WORKERS(task));
}



static MbObject *_select ( MbsTask *task,
                           MbPopType pop_type )
{
  GList *l;
  MbsTask *found = NULL;
  gint64 found_score = 0;
  for (l = MB_TASK_CHILDREN(task); l; l = l->next)
    {
      MbsTask *child = l->data;
      if (mbs_task_accept(child, pop_type))
        {
          gint64 score = mbs_task_next_score(child);
          if ((!found) || score < found_score)
            {
              found = child;
              found_score = score;
            }
        }
    }
  ASSERT(found); /* ?? */
  if (MB_TASK_ISGROUP(found))
    return _select(found, pop_type);
  else
    return MB_OBJECT(found);
}



/* mbs_task_select:
 */
MbObject *mbs_task_select ( MbsTask *task,
                            MbPopType pop_type )
{
  ASSERT(MB_TASK_ISGROUP(task));
  if (!(mbs_task_accept(task, pop_type)))
    return NULL;
  return _select(task, pop_type);
}



/* _process:
 */
static void _process ( MbsTask *task )
{
  if (MB_TASK_ISGROUP(task))
    {
      GList *l;
      gint64 min_score = -1;
      for (l = MB_TASK_CHILDREN(task); l; l = l->next)
        {
          MbsTask *child = l->data;
          _process(child);
          if (mbs_task_ready(child))
            {
              gint64 score = MBS_TASK_SCORE(child);
              ASSERT(score >= 0);
              if (min_score < 0)
                min_score = score;
              else
                min_score = MIN(min_score, score);
            }
        }
      if (min_score > 0)
        {
          for (l = MB_TASK_CHILDREN(task); l; l = l->next)
            {
              MbsTask *child = l->data;
              if (mbs_task_ready(child))
                mbs_priority_adjust_score(MBS_PRIORITY(MB_TASK_PRIORITY(child)),
                                          -min_score);
            }
        }
    }
  else
    {
      if (task->funcs.process)
        task->funcs.process(task);
    }
  mbs_task_check(task);
  mbs_priority_update_score(MBS_PRIORITY(MB_TASK_PRIORITY(task)),
                            MB_TASK_WORKERS(task));
}



/* mbs_task_process:
 */
void mbs_task_process ( MbsTask *task )
{
  ASSERT(MB_TASK_ISGROUP(task));
  ASSERT(!MB_TASK_PARENT(task));
  _process(task);
  /* [FIXME] */
  mbs_priority_adjust_score(MBS_PRIORITY(MB_TASK_PRIORITY(task)),
                            -MBS_TASK_SCORE(task));
}
