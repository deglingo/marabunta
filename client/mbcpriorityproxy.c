/* mbcpriorityproxy.c -
 */

#include "client/cliprivate.h"
#include "client/mbcpriorityproxy.h"
#include "client/mbcgameproxy.h"
#include "client/mbcpriorityproxy.inl"



/* mbc_priority_proxy_new:
 */
MbcProxy *mbc_priority_proxy_new ( MbcProxy *game,
                                   guint id,
                                   MbPriorityValue value )
{
  MbcPriorityProxy *prio;
  ASSERT(MBC_IS_GAME_PROXY(game));
  prio = MBC_PRIORITY_PROXY(mbc_game_proxy_create_object(MBC_GAME_PROXY(game),
                                                         MBC_CLASS_PRIORITY_PROXY,
                                                         id));
  prio->value = value;
  return MBC_PROXY(prio);
}
