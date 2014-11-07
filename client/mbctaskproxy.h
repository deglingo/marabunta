/* mbctaskproxy.h -
 */

#ifndef _MBCTASKPROXY_H_
#define _MBCTASKPROXY_H_

#include "client/mbcproxy.h"
#include "client/mbctaskproxy-def.h"

G_BEGIN_DECLS



/* MbcTaskProxy:
 */
struct _MbcTaskProxy
{
  MBC_TASK_PROXY_INSTANCE_HEADER;

  MbcProxy *colony;
  MbcProxy *parent;
  gchar *name;
  gboolean isgroup;
  GList *children;
  gint64 workers;
};



/* MbcTaskProxyClass:
 */
struct _MbcTaskProxyClass
{
  MBC_TASK_PROXY_CLASS_HEADER;
};



MbcProxy *mbc_task_proxy_new ( MbcProxy *game,
                               guint id,
                               gboolean group,
                               const gchar *name,
                               gint64 workers );
void mbc_task_proxy_add_child ( MbcTaskProxy *task,
                                MbcProxy *child );
void mbc_task_proxy_set_workers ( MbcTaskProxy *task,
                                  gint64 workers );



G_END_DECLS

#endif /* ifndef _MBCTASKPROXY_H_ */
