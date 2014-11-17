/* mbstask.c -
 */

#include "server/srvprivate.h"
#include "server/mbstask.h"
#include "server/mbscolony.h"
#include "server/mbssector.h"
#include "server/mbsworld.h"
#include "server/mbstask.inl"



#define ACCEPT(task, type) (MBS_TASK(task)->accept_flags & (1 << (type)))
#define READY(task)        (MBS_TASK(task)->isgroup ? MBS_TASK(task)->children_ready : MBS_TASK(task)->ready)



static MbsTask *_task_new ( struct _MbsColony *colony,
                            MbsTask *parent,
                            const gchar *name,
                            MbPopFlags accept_flags,
                            MbsTaskFuncs *funcs,
                            gboolean isgroup )
{
  MbsTask *task = MBS_TASK_NEW(NULL);
  task->colony = colony;
  task->name = g_strdup(name);
  task->accept_flags = accept_flags;
  if (parent)
    {
      ASSERT(parent->isgroup);
      task->parent = parent;
      parent->children = g_list_append(parent->children,
                                       l_object_ref(task));
    }
  if (funcs)
    task->funcs = *funcs;
  task->isgroup = isgroup;
  task->priority = mbs_priority_new(colony->sector->world->game, 5);
  if (!isgroup)
    mbs_task_set_ready(task, TRUE);
  return task;
}



/* mbs_task_new:
 */
MbsTask *mbs_task_new ( struct _MbsColony *colony,
                        MbsTask *parent,
                        const gchar *name,
                        MbPopFlags accept_flags,
                        MbsTaskFuncs *funcs )
{
  return _task_new(colony, parent, name, accept_flags, funcs, FALSE);
}



/* mbs_task_new_group:
 */
MbsTask *mbs_task_new_group ( struct _MbsColony *colony,
                              MbsTask *parent,
                              const gchar *name,
                              MbPopFlags accept_flags )
{
  return _task_new(colony, parent, name, accept_flags, NULL, TRUE);
}



/* mbs_task_set_ready:
 */
void mbs_task_set_ready ( MbsTask *task,
                          gboolean ready )
{
  ASSERT(!task->isgroup);
  if (task->ready && !ready)
    {
      task->ready = FALSE;
      while ((task = task->parent))
        {
          ASSERT(task->children_ready > 0);
          ASSERT(task->ready);
          task->children_ready--;
          if (task->children_ready == 0) {
            task->ready = FALSE;
          } else {
            break;
          }
        }
    }
  else if (ready && !task->ready)
    {
      task->ready = TRUE;
      while ((task = task->parent))
        {
          task->children_ready++;
          if (task->children_ready == 1) {
            ASSERT(!task->ready);
            task->ready = TRUE;
          } else {
            ASSERT(task->ready);
            break;
          }
        }
    }
}



/* _select:
 */
static MbsTask *_select ( MbsTask *task,
                          MbPopType type )
{
  if (!(ACCEPT(task, type) && READY(task)))
      return NULL;
  if (task->isgroup)
    {
      MbsTask *found = NULL;
      gint64 found_score = 0;
      GList *l;
      for (l = task->children; l; l = l->next)
        {
          MbsTask *child = l->data;
          MbsTask *sel = _select(child, type);
          gint64 score;
          if (!sel)
            continue;
          score = mbs_task_get_next_score(child);
          if ((!found) || score < found_score)
            {
              found = sel;
              found_score = score;
            }
        }
      return found;
    }
  else
    {
      return task;
    }
}



/* mbs_task_select:
 */
MbsTask *mbs_task_select ( MbsTask *task,
                           MbPopType type )
{
  ASSERT(task->isgroup && !task->parent);
  return _select(task, type);
}



static void _process ( MbsTask *task )
{
  if (task->isgroup)
    {
      GList *l;
      gboolean ready_flag = FALSE;
      gint64 min_score;
      for (l = task->children; l; l = l->next)
        {
          MbsTask *child = l->data;
          _process(child);
          if (child->ready)
            {
              if (ready_flag) {
                min_score = MIN(min_score, child->score);
              } else {
                min_score = child->score;
                ready_flag = TRUE;
              }
            }
        }
      /* adjust all scores */
      if (ready_flag && min_score > 0)
        {
          for (l = task->children; l; l = l->next)
            MBS_TASK(l->data)->score -= min_score;
        }
    }
  else
    {
      task->funcs.process(task);
      if (task->funcs.ready(task))
        mbs_task_set_ready(task, TRUE);
      else
        mbs_task_set_ready(task, FALSE);
    }
  /* [fixme] top task score is never adjusted! */
  task->score += task->workers * mbs_priority_get_score_factor(task->priority);
}



/* mbs_task_process:
 */
void mbs_task_process ( MbsTask *task )
{
  ASSERT(task->isgroup && !task->parent);
  _process(task);
}



/* mbs_task_get_score:
 */
gint64 mbs_task_get_score ( MbsTask *task )
{
  return task->score;
}



/* mbs_task_get_next_score:
 */
gint64 mbs_task_get_next_score ( MbsTask *task )
{
  /* [FIXME] * prio !! */
  return task->score + task->workers * mbs_priority_get_score_factor(task->priority);
}



/* mbs_task_adjust_workers:
 */
void mbs_task_adjust_workers ( MbsTask *task,
                               gint64 count )
{
  ASSERT(!task->isgroup);
  task->workers += count;
  ASSERT(task->workers >= 0);
  /* fix the ready flag */
  if (task->funcs.ready(task))
    mbs_task_set_ready(task, TRUE);
  else
    mbs_task_set_ready(task, FALSE);
  /* adjust parents */
  while ((task = task->parent))
    {
      task->workers += count;
      ASSERT(task->workers >= 0);
    }
}
