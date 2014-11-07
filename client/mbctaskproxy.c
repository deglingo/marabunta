/* mbctaskproxy.c -
 */

#include "client/cliprivate.h"
#include "client/mbctaskproxy.h"
#include "client/mbcgameproxy.h"
#include "client/mbctaskproxy.inl"



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
