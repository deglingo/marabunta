/* mbcgameproxy.h -
 */

#ifndef _MBCGAMEPROXY_H_
#define _MBCGAMEPROXY_H_

#include "client/mbcproxy.h"
#include "client/mbcworldproxy.h"
#include "client/mbcgameproxy-def.h"

G_BEGIN_DECLS



/* MbcGameProxy:
 */
struct _MbcGameProxy
{
  MBC_GAME_PROXY_INSTANCE_HEADER;

  guint sim_time;

  /* world proxy */
  MbcWorldProxy *world;
};



/* MbcGameProxyClass:
 */
struct _MbcGameProxyClass
{
  MBC_GAME_PROXY_CLASS_HEADER;
};



MbcGameProxy *mbc_game_proxy_new ( guint id );
void mbc_game_proxy_reset ( MbcGameProxy *proxy );
void mbc_game_proxy_set_sim_time ( MbcGameProxy *proxy,
                                   guint sim_time );
/* void mbc_game_proxy_started ( MbcGameProxy *proxy ); */



G_END_DECLS

#endif /* ifndef _MBCGAMEPROXY_H_ */
