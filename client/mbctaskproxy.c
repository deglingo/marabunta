/* mbctaskproxy.c -
 */

#include "client/cliprivate.h"
#include "client/mbctaskproxy.h"
#include "client/mbcgameproxy.h"
#include "client/mbcpriorityproxy.h"
#include "client/mbctaskproxy.inl"



/* properties:
 */
enum
  {
    PROP_0,
    PROP_WORKERS,
    PROP_COUNT,
  };

static LParamSpec *pspecs[PROP_COUNT] = { NULL, };



/* mbc_task_proxy_class_init:
 */
static void mbc_task_proxy_class_init ( LObjectClass *cls )
{
  /* [FIXME] int64, long ? */
  pspecs[PROP_WORKERS] = l_param_spec_int("workers", 0);

  l_object_class_install_properties(cls, PROP_COUNT, pspecs);
}



/* mbc_task_proxy_new:
 */
MbcProxy *mbc_task_proxy_new ( MbcProxy *game,
                               guint id,
                               MbcProxy *priority,
                               gboolean group,
                               const gchar *name,
                               gint64 workers )
{
  MbcTaskProxy *task;
  ASSERT(game);
  ASSERT(MBC_IS_GAME_PROXY(game));
  ASSERT(priority);
  ASSERT(MBC_IS_PRIORITY_PROXY(priority));
  task = MBC_TASK_PROXY(mbc_game_proxy_create_object(MBC_GAME_PROXY(game),
                                                     MBC_CLASS_TASK_PROXY,
                                                     id));
  task->priority = priority;
  task->isgroup = group;
  task->name = g_strdup(name);
  task->workers = workers;
  return MBC_PROXY(task);
}



/* mbc_task_proxy_add_child:
 */
void mbc_task_proxy_add_child ( MbcTaskProxy *task,
                                MbcProxy *child )
{
  ASSERT(task);
  ASSERT(task->colony);
  ASSERT(task->isgroup);
  ASSERT(child);
  ASSERT(MBC_IS_TASK_PROXY(child));
  ASSERT(!MBC_TASK_PROXY(child)->colony);
  ASSERT(!MBC_TASK_PROXY(child)->parent);
  
  MBC_TASK_PROXY(child)->colony = task->colony;
  MBC_TASK_PROXY(child)->parent = MBC_PROXY(task);
  task->children = g_list_append(task->children, child);
}



/* mbc_task_proxy_set_workers:
 */
void mbc_task_proxy_set_workers ( MbcTaskProxy *task,
                                  gint64 workers )
{
  if (workers == task->workers)
    return;
  task->workers = workers;
  l_object_notify(L_OBJECT(task), pspecs[PROP_WORKERS]);
}



static MbcTaskProxy *_find ( MbcTaskProxy *task,
                             const gchar *name )
{
  GList *l;
  for (l = task->children; l; l = l->next)
    {
      if (!strcmp(MBC_TASK_PROXY(l->data)->name, name))
        return l->data;
    }
  return NULL;
}



/* mbc_task_proxy_find:
 */
MbcTaskProxy *mbc_task_proxy_find ( MbcTaskProxy *task,
                                    const gchar *path )
{
  gchar comp[MBS_TASK_MAX_NAME+1];
  const gchar *p = path;
  ASSERT(path);
  while (mb_task_path_next(&p, comp))
    {
      if (!(task = _find(task, comp)))
        return NULL;
    }
  return task;
}
