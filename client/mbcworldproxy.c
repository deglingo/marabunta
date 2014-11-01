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
  return wp;
}



/* mbc_world_proxy_set_size:
 */
void mbc_world_proxy_set_size ( MbcWorldProxy *proxy,
                                guint width,
                                guint height )
{
  /* [fixme] delee old sectors */
  guint x, y;
  proxy->width = width;
  proxy->height = height;
  proxy->sectors = g_new0(MbcSectorProxy **, proxy->height);
  for (y = 0; y < proxy->height; y++)
    {
      proxy->sectors[y] = g_new0(MbcSectorProxy *, proxy->width);
      for (x = 0; x < proxy->width; x++)
        {
          proxy->sectors[y][x] = mbc_sector_proxy_new();
        }
    }
}
