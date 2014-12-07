/* mbtask.c -
 */

#include "mbtask.h"
#include "mbpriority.h"
#include "mbgame.h"
#include "mbcolony.h"
#include "mbsector.h"
#include "mbworld.h"
#include "mbtask.inl"



/* Properties:
 */
enum
  {
    PROP_0,
    PROP_WORKERS,
    N_PROPS,
  };

static LParamSpec *pspecs[N_PROPS];



static void t_spawn_process ( MbTask *task );



/* mb_task_class_init:
 */
static void mb_task_class_init ( LObjectClass *cls )
{
  /* [FIXME] gint64 !! */
  pspecs[PROP_WORKERS] =
    l_param_spec_int("workers",
                     0);

  l_object_class_install_properties(cls, N_PROPS, pspecs);
}



/* _task_new:
 */
static MbTask *_task_new ( MbTask *parent,
                           const gchar *name,
                           gboolean isgroup )
{
  MbTask *task = MB_TASK(l_object_new(MB_CLASS_TASK, NULL));
  task->name = g_strdup(name);
  task->isgroup = isgroup;
  task->priority = mb_priority_new(50);
  if (parent)
    {
      ASSERT(parent->isgroup);
      task->parent = parent;
      l_object_ref(task);
      parent->children = g_list_append(parent->children, task);
      task->colony = parent->colony; /* [fixme] ref ? */
    }
  return task;
}



/* mb_task_new_top:
 */
MbTask *mb_task_new_top ( struct _MbColony *colony,
                          const gchar *name )
{
  MbTask *task = _task_new(NULL, name, TRUE);
  task->colony = colony; /* [fixme] ref ? */
  return task;
}



/* mb_task_new_group:
 */
MbTask *mb_task_new_group ( MbTask *parent,
                            const gchar *name )
{
  MbTask *task = _task_new(parent, name, TRUE);
  return task;
}



/* mb_task_new:
 */
MbTask *mb_task_new ( MbTask *parent,
                      const gchar *name,
                      MbPopFlags pop_flags,
                      MbTaskFuncs *funcs,
                      gpointer data )
{
  MbTask *task = _task_new(parent, name, FALSE);
  MbTask *p;
  task->pop_flags = pop_flags;
  for (p = task->parent; p; p = p->parent)
    p->pop_flags |= pop_flags;
  task->funcs = *funcs;
  if (task->funcs.init)
    task->funcs.init(task, data);
  return task;
}



/* mb_task_new_spawn:
 */
MbTask *mb_task_new_spawn ( MbTask *parent )
{
  MbTaskFuncs funcs = { NULL, NULL, t_spawn_process, };
  return mb_task_new(parent, "spawn", MB_POP_FLAG_ADULT_QUEEN, &funcs, NULL);
}



/* mb_task_new_food:
 */
MbTask *mb_task_new_food ( MbTask *parent )
{
  MbTaskFuncs funcs = { NULL, };
  return mb_task_new(parent, "food", MB_POP_FLAG_ADULT_WORKER, &funcs, NULL);
}



/* mb_task_new_build:
 */
MbTask *mb_task_new_build ( MbTask *parent,
                            struct _MbRoom *room )
{
  MbTaskFuncs funcs = { NULL, };
  return mb_task_new(parent, "build", MB_POP_FLAG_ADULT_WORKER, &funcs, room);
}



/* _select:
 */
static MbTask *_select ( MbTask *task,
                         MbPopFlags pop_flag )
{
  if (!(task->pop_flags & pop_flag))
    return NULL;
  if (task->isgroup)
    {
      GList *l;
      MbTask *found = NULL;
      gint64 found_score = 0;
      for (l = task->children; l; l = l->next)
        {
          MbTask *sel = _select(l->data, pop_flag);
          if (sel)
            {
              gint64 score = mb_priority_next_score(sel->priority, sel->workers);
              if ((!found) || score < found_score)
                {
                  found = sel;
                  found_score = score;
                }
            }
        }
      return found;
    }
  else
    {
      /* [FIXME] check ready */
      return task;
    }
}



/* mb_task_select:
 */
MbTask *mb_task_select ( MbTask *task,
                         MbPopType pop_type )
{
  ASSERT(task->isgroup && !task->parent);
  return _select(task, (1 << pop_type));
}



/* _process:
 */
static void _process ( MbTask *task )
{
  if (task->isgroup)
    {
      GList *l;
      for (l = task->children; l; l = l->next)
        {
          _process(l->data);
        }
    }
  else
    {
      if (task->funcs.process)
        task->funcs.process(task);
      /* else */
      /*   CL_DEBUG("[TODO] process task %s: %" G_GINT64_FORMAT, */
      /*            task->name, task->workers); */
    }
}



/* mb_task_process:
 */
void mb_task_process ( MbTask *task )
{
  ASSERT(task->isgroup && !task->parent);
  _process(task);
}



/* mb_task_add_workers:
 */
void mb_task_add_workers ( MbTask *task,
                           gint64 workers )
{
  ASSERT(!task->isgroup);
  if (!workers)
    return;
  while (task)
    {
      task->workers += workers;
      l_object_notify(L_OBJECT(task), pspecs[PROP_WORKERS]);
      task = task->parent;
    }
}



/* t_spawn_process:
 */
static void t_spawn_process ( MbTask *task )
{
  if (task->workers)
    {
      MbGame *game = task->colony->sector->world->game;
      gint64 count = task->workers * 10;
      mb_colony_adjust_pop(task->colony, MB_POP_EGG, game->sim_time, count);
    }
}
