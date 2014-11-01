/* mbccolonyproxy.c -
 */

#include "client/mbccolonyproxy.h"
#include "client/mbccolonyproxy.inl"



/* mbc_colony_proxy_new:
 */
MbcColonyProxy *mbc_colony_proxy_new ( gint owner )
{
  MbcColonyProxy *cp;
  cp = MBC_COLONY_PROXY(l_object_new(MBC_CLASS_COLONY_PROXY, NULL));
  cp->owner = owner;
  return cp;
}
