/* mbcworldproxy.c -
 */

#include "client/cliprivate.h"
#include "client/mbcworldproxy.h"
#include "client/mbcworldproxy.inl"



/* mbc_world_proxy_set_size:
 */
void mbc_world_proxy_set_size ( MbcWorldProxy *proxy,
                                guint width,
                                guint height )
{
  /* [fixme] delee old sectors */
  guint y;
  proxy->width = width;
  proxy->height = height;
  proxy->sectors = g_new0(MbcProxy **, proxy->height);
  for (y = 0; y < proxy->height; y++)
    {
      proxy->sectors[y] = g_new0(MbcProxy *, proxy->width);
    }
}



/* mbc_world_proxy:
 */
void mbc_world_proxy_add_sector ( MbcWorldProxy *world,
                                  MbcProxy *sector )
{
  ASSERT(MBC_IS_SECTOR_PROXY(sector));
  ASSERT(!MBC_SECTOR_PROXY(sector)->world);
  ASSERT(MBC_SECTOR_PROXY(sector)->x < world->width);
  ASSERT(MBC_SECTOR_PROXY(sector)->y < world->height);
  ASSERT(!world->sectors[MBC_SECTOR_PROXY(sector)->y][MBC_SECTOR_PROXY(sector)->x]);
  /* [fixme] ref ? */
  MBC_SECTOR_PROXY(sector)->world = MBC_PROXY(world);
  world->sectors[MBC_SECTOR_PROXY(sector)->y][MBC_SECTOR_PROXY(sector)->x] = sector;
}
