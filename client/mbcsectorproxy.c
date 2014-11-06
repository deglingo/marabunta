/* mbcsectorproxy.c -
 */

#include "client/cliprivate.h"
#include "client/mbcsectorproxy.h"
#include "client/mbcgameproxy.h"
#include "client/mbccolonyproxy.h"
#include "client/mbcsectorproxy.inl"



/* mbc_sector_proxy_new:
 */
MbcProxy *mbc_sector_proxy_new ( MbcProxy *game,
                                 guint id,
                                 guint x,
                                 guint y )
{
  MbcProxy *sector;
  ASSERT(MBC_IS_GAME_PROXY(game));
  sector = mbc_game_proxy_create_object(MBC_GAME_PROXY(game),
                                        MBC_CLASS_SECTOR_PROXY,
                                        id);
  MBC_SECTOR_PROXY(sector)->x = x;
  MBC_SECTOR_PROXY(sector)->y = y;
  return MBC_PROXY(sector);
}



/* mbc_sector_proxy_add_colony:
 */
void mbc_sector_proxy_add_colony ( MbcSectorProxy *sector,
                                   MbcProxy *colony )
{
  ASSERT(sector);
  ASSERT(!sector->colony);
  ASSERT(colony);
  ASSERT(MBC_IS_COLONY_PROXY(colony));
  ASSERT(!MBC_COLONY_PROXY(colony)->sector);
  sector->colony = colony;
  MBC_COLONY_PROXY(colony)->sector = MBC_PROXY(sector);
}
