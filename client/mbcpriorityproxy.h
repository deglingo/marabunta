/* mbcpriorityproxy.h -
 */

#ifndef _MBCPRIORITYPROXY_H_
#define _MBCPRIORITYPROXY_H_

#include "client/mbcproxy.h"
#include "client/mbcpriorityproxy-def.h"

G_BEGIN_DECLS



/* MbcPriorityProxy:
 */
struct _MbcPriorityProxy
{
  MBC_PRIORITY_PROXY_INSTANCE_HEADER;

  MbPriorityValue value;
};



/* MbcPriorityProxyClass:
 */
struct _MbcPriorityProxyClass
{
  MBC_PRIORITY_PROXY_CLASS_HEADER;
};



MbcProxy *mbc_priority_proxy_new ( MbcProxy *game,
                                   guint id,
                                   MbPriorityValue value );



G_END_DECLS

#endif /* ifndef _MBCPRIORITYPROXY_H_ */
