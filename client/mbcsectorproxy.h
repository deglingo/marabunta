/* mbcsectorproxy.h -
 */

#ifndef _MBCSECTORPROXY_H_
#define _MBCSECTORPROXY_H_

#include "client/mbcproxy.h"
#include "client/mbcsectorproxy-def.h"

G_BEGIN_DECLS



/* MbcSectorProxy:
 */
struct _MbcSectorProxy
{
  MBC_SECTOR_PROXY_INSTANCE_HEADER;

  MbcProxy *world;
  guint x;
  guint y;
  MbcProxy *colony;
};



/* MbcSectorProxyClass:
 */
struct _MbcSectorProxyClass
{
  MBC_SECTOR_PROXY_CLASS_HEADER;
};



MbcProxy *mbc_sector_proxy_new ( MbcProxy *game,
                                 guint id,
                                 guint x,
                                 guint y );
void mbc_sector_proxy_add_colony ( MbcSectorProxy *sector,
                                   MbcProxy *colony );



G_END_DECLS

#endif /* ifndef _MBCSECTORPROXY_H_ */
