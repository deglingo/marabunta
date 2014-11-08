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
void mbc_priority_proxy_set_value ( MbcPriorityProxy *priority,
                                    MbPriorityValue value );
void mbc_priority_proxy_request_set_value ( MbcPriorityProxy *proxy,
                                            MbPriorityValue value );
void mbc_priority_proxy_request_inc_value ( MbcPriorityProxy *proxy );
void mbc_priority_proxy_request_dec_value ( MbcPriorityProxy *proxy );



G_END_DECLS

#endif /* ifndef _MBCPRIORITYPROXY_H_ */
