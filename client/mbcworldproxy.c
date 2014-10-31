/* mbcworldproxy.c -
 */

#include "client/mbcworldproxy.h"
#include "client/mbcworldproxy.inl"



/* mbc_world_proxy_new:
 */
MbcWorldProxy *mbc_world_proxy_new ( void )
{
  MbcWorldProxy *wp;
  wp = MBC_WORLD_PROXY(l_object_new(MBC_CLASS_WORLD_PROXY, NULL));
  /* [fixme] setup */
  {
    guint x, y;
    wp->width = 2;
    wp->height = 2;
    wp->sectors = g_new0(MbcSectorProxy **, wp->height);
    for (y = 0; y < wp->height; y++)
      {
        wp->sectors[y] = g_new0(MbcSectorProxy *, wp->width);
        for (x = 0; x < wp->width; x++)
          {
            wp->sectors[y][x] = mbc_sector_proxy_new();
          }
      }
  }
  return wp;
}
