/* mbcsectorproxy.h -
 */

#ifndef _MBCSECTORPROXY_H_
#define _MBCSECTORPROXY_H_

#include "client/mbcbase.h"
#include "client/mbccolonyproxy.h"
#include "client/mbcsectorproxy-def.h"

G_BEGIN_DECLS



/* MbcSectorProxy:
 */
struct _MbcSectorProxy
{
  MBC_SECTOR_PROXY_INSTANCE_HEADER;

  MbcColonyProxy *colony;
};



/* MbcSectorProxyClass:
 */
struct _MbcSectorProxyClass
{
  MBC_SECTOR_PROXY_CLASS_HEADER;
};



MbcSectorProxy *mbc_sector_proxy_new ( void );
void mbc_sector_proxy_create_colony ( MbcSectorProxy *proxy,
                                      MbsPlayerID owner );



G_END_DECLS

#endif /* ifndef _MBCSECTORPROXY_H_ */
