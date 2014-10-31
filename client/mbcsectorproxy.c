/* mbcsectorproxy.c -
 */

#include "client/mbcsectorproxy.h"
#include "client/mbcsectorproxy.inl"



/* mbc_sector_proxy_new:
 */
MbcSectorProxy *mbc_sector_proxy_new ( void )
{
  MbcSectorProxy *sp;
  sp = MBC_SECTOR_PROXY(l_object_new(MBC_CLASS_SECTOR_PROXY, NULL));
  sp->colony = mbc_colony_proxy_new();
  return sp;
}
