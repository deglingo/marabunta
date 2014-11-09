/* mbcroomproxy.h -
 */

#ifndef _MBCROOMPROXY_H_
#define _MBCROOMPROXY_H_

#include "client/mbcproxy.h"
#include "client/mbcroomproxy-def.h"

G_BEGIN_DECLS



/* MbcRoomProxy:
 */
struct _MbcRoomProxy
{
  MBC_ROOM_PROXY_INSTANCE_HEADER;

  MbRoomType type;
  MbcProxy *colony;
};



/* MbcRoomProxyClass:
 */
struct _MbcRoomProxyClass
{
  MBC_ROOM_PROXY_CLASS_HEADER;
};



MbcProxy *mbc_room_proxy_new ( MbcProxy *game,
                               guint id,
                               MbRoomType type );



G_END_DECLS

#endif /* ifndef _MBCROOMPROXY_H_ */
