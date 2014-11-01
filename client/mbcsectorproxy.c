/* mbcsectorproxy.c -
 */

#include "client/cliprivate.h"
#include "client/mbcsectorproxy.h"
#include "client/mbcsectorproxy.inl"



/* mbc_sector_proxy_new:
 */
MbcSectorProxy *mbc_sector_proxy_new ( void )
{
  return MBC_SECTOR_PROXY_NEW(NULL);
}



/* mbc_sector_proxy_create_colony:
 */
void mbc_sector_proxy_create_colony ( MbcSectorProxy *proxy,
                                      MbsPlayerID owner )
{
  ASSERT(!proxy->colony);
  proxy->colony = mbc_colony_proxy_new(owner);
}
