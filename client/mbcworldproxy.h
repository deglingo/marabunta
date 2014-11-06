/* mbcworldproxy.h -
 */

#ifndef _MBCWORLDPROXY_H_
#define _MBCWORLDPROXY_H_

#include "client/mbcbase.h"
#include "client/mbcsectorproxy.h"
#include "client/mbcworldproxy-def.h"

G_BEGIN_DECLS



/* MbcWorldProxy:
 */
struct _MbcWorldProxy
{
  MBC_WORLD_PROXY_INSTANCE_HEADER;

  guint width;
  guint height;
  MbcProxy ***sectors;
};



/* MbcWorldProxyClass:
 */
struct _MbcWorldProxyClass
{
  MBC_WORLD_PROXY_CLASS_HEADER;
};



void mbc_world_proxy_set_size ( MbcWorldProxy *proxy,
                                guint width,
                                guint height );
void mbc_world_proxy_add_sector ( MbcWorldProxy *proxy,
                                  MbcProxy *sector );



G_END_DECLS

#endif /* ifndef _MBCWORLDPROXY_H_ */
