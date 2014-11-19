/* mbccolony.h -
 */

#ifndef _MBCCOLONY_H_
#define _MBCCOLONY_H_

#include "client/mbcbase.h"
#include "client/mbccolony-def.h"

G_BEGIN_DECLS



/* MbcColony:
 */
struct _MbcColony
{
  MBC_COLONY_INSTANCE_HEADER;
};



/* MbcColonyClass:
 */
struct _MbcColonyClass
{
  MBC_COLONY_CLASS_HEADER;
};



MbObject *mbc_colony_new ( MbObject *game,
                           MbObjectID id );



G_END_DECLS

#endif /* ifndef _MBCCOLONY_H_ */
