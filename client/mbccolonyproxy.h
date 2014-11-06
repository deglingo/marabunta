/* mbccolonyproxy.h -
 */

#ifndef _MBCCOLONYPROXY_H_
#define _MBCCOLONYPROXY_H_

#include "client/mbcproxy.h"
#include "client/mbccolonyproxy-def.h"

G_BEGIN_DECLS



/* MbcColonyProxy:
 */
struct _MbcColonyProxy
{
  MBC_COLONY_PROXY_INSTANCE_HEADER;

  MbcProxy *sector;
  gint owner;
  gint64 pop[MB_POP_TYPE_COUNT];
};



/* MbcColonyProxyClass:
 */
struct _MbcColonyProxyClass
{
  MBC_COLONY_PROXY_CLASS_HEADER;
};



MbcProxy *mbc_colony_proxy_new ( MbcProxy *game,
                                 guint id,
                                 gint owner );
void mbc_colony_proxy_set_pop ( MbcColonyProxy *proxy,
                                gint64 *pop );



G_END_DECLS

#endif /* ifndef _MBCCOLONYPROXY_H_ */
