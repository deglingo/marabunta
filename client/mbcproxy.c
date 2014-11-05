/* mbcproxy.c -
 */

#include "client/mbcproxy.h"
#include "client/mbcproxy.inl"



/* mbc_proxy_new:
 */
MbcProxy *mbc_proxy_new ( LObjectClass *cls,
                          guint id )
{
  MbcProxy *proxy;
  proxy = MBC_PROXY(l_object_new(cls, NULL));
  proxy->id = id;
  return proxy;
}
