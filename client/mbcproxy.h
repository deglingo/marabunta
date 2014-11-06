/* mbcproxy.h -
 */

#ifndef _MBCPROXY_H_
#define _MBCPROXY_H_

#include "client/mbcbase.h"
#include "client/mbcproxy-def.h"

G_BEGIN_DECLS



/* MbcProxy:
 */
struct _MbcProxy
{
  MBC_PROXY_INSTANCE_HEADER;

  guint id;
  /* set by MbcGameProxy */
  MbcProxy *game;
};



/* MbcProxyClass:
 */
struct _MbcProxyClass
{
  MBC_PROXY_CLASS_HEADER;
};



MbcProxy *mbc_proxy_new ( LObjectClass *cls,
                          guint id );



G_END_DECLS

#endif /* ifndef _MBCPROXY_H_ */
