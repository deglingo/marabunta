/* mbcresourceproxy.h -
 */

#ifndef _MBCRESOURCEPROXY_H_
#define _MBCRESOURCEPROXY_H_

#include "client/mbcproxy.h"
#include "client/mbcresourceproxy-def.h"

G_BEGIN_DECLS



/* MbcResourceProxy:
 */
struct _MbcResourceProxy
{
  MBC_RESOURCE_PROXY_INSTANCE_HEADER;

  gchar *name;
};



/* MbcResourceProxyClass:
 */
struct _MbcResourceProxyClass
{
  MBC_RESOURCE_PROXY_CLASS_HEADER;
};



MbcProxy *mbc_resource_proxy_new ( MbcProxy *game,
                                   guint id,
                                   const gchar *name );



G_END_DECLS

#endif /* ifndef _MBCRESOURCEPROXY_H_ */
