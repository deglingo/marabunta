/* mbctaskproxy.c -
 */

#include "client/cliprivate.h"
#include "client/mbctaskproxy.h"
#include "client/mbcgameproxy.h"
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
                               gboolean group,
                               const gchar *name,
                               gint64 workers )
{
  MbcTaskProxy *task;
  ASSERT(game);
  ASSERT(MBC_IS_GAME_PROXY(game));
  task = MBC_TASK_PROXY(mbc_game_proxy_create_object(MBC_GAME_PROXY(game),
                                                     MBC_CLASS_TASK_PROXY,
                                                     id));
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
