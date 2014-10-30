/* mbcgameproxy.c -
 */

#include "client/cliprivate.h"
#include "client/mbcgameproxy.h"
#include "client/mbcgameproxy.inl"



/* mbc_game_proxy_new:
 */
MbcGameProxy *mbc_game_proxy_new ( void )
{
  MbcGameProxy *gp;
  gp = MBC_GAME_PROXY(l_object_new(MBC_CLASS_GAME_PROXY, NULL));
  return gp;
}



/* mbc_game_proxy:
 */
void mbc_game_proxy_process_update ( MbcGameProxy *proxy,
                                     MbMessage *message )
{
  CL_DEBUG("[TODO] %d", message->frame);
}
