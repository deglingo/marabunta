/* mbcgameproxy.h -
 */

#ifndef _MBCGAMEPROXY_H_
#define _MBCGAMEPROXY_H_

#include "client/mbcbase.h"
#include "client/mbcgameproxy-def.h"

G_BEGIN_DECLS



/* MbcGameProxy:
 */
struct _MbcGameProxy
{
  MBC_GAME_PROXY_INSTANCE_HEADER;
};



/* MbcGameProxyClass:
 */
struct _MbcGameProxyClass
{
  MBC_GAME_PROXY_CLASS_HEADER;
};



MbcGameProxy *mbc_game_proxy_new ( void );
void mbc_game_proxy_process_update ( MbcGameProxy *proxy,
                                     MbMessage *message );



G_END_DECLS

#endif /* ifndef _MBCGAMEPROXY_H_ */
