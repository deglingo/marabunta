/* mbcworld.h -
 */

#ifndef _MBCWORLD_H_
#define _MBCWORLD_H_

#include "client/mbcbase.h"
#include "client/mbcworld-def.h"

G_BEGIN_DECLS



/* MbcWorld:
 */
struct _MbcWorld
{
  MBC_WORLD_INSTANCE_HEADER;
};



/* MbcWorldClass:
 */
struct _MbcWorldClass
{
  MBC_WORLD_CLASS_HEADER;
};



MbObject *mbc_world_new ( MbObject *game,
                          MbObjectID id,
                          guint width,
                          guint height );



G_END_DECLS

#endif /* ifndef _MBCWORLD_H_ */
