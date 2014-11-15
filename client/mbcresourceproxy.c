/* mbcresourceproxy.c -
 */

#include "client/cliprivate.h"
#include "client/mbcresourceproxy.h"
#include "client/mbcgameproxy.h"
#include "client/mbcresourceproxy.inl"



/* mbc_resource_proxy_new:
 */
MbcProxy *mbc_resource_proxy_new ( MbcProxy *game,
                                   guint id,
                                   const gchar *name )
{
  MbcResourceProxy *rsc;
  ASSERT(MBC_IS_GAME_PROXY(game));
  rsc = MBC_RESOURCE_PROXY(mbc_game_proxy_create_object(MBC_GAME_PROXY(game),
                                                        MBC_CLASS_RESOURCE_PROXY,
                                                        id));
  rsc->name = g_strdup(name);
  return MBC_PROXY(rsc);
}
