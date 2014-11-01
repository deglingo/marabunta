/* mbccolonyproxy.h -
 */

#ifndef _MBCCOLONYPROXY_H_
#define _MBCCOLONYPROXY_H_

#include "client/mbcbase.h"
#include "client/mbccolonyproxy-def.h"

G_BEGIN_DECLS



/* MbcColonyProxy:
 */
struct _MbcColonyProxy
{
  MBC_COLONY_PROXY_INSTANCE_HEADER;

  gint owner;
};



/* MbcColonyProxyClass:
 */
struct _MbcColonyProxyClass
{
  MBC_COLONY_PROXY_CLASS_HEADER;
};



MbcColonyProxy *mbc_colony_proxy_new ( gint owner );



G_END_DECLS

#endif /* ifndef _MBCCOLONYPROXY_H_ */
