/* mbcroom.h -
 */

#ifndef _MBCROOM_H_
#define _MBCROOM_H_

#include "client/mbcbase.h"
#include "client/mbcroom-def.h"

G_BEGIN_DECLS



/* MbcRoom:
 */
struct _MbcRoom
{
  MBC_ROOM_INSTANCE_HEADER;
};



/* MbcRoomClass:
 */
struct _MbcRoomClass
{
  MBC_ROOM_CLASS_HEADER;
};



MbObject *mbc_room_new ( MbObject *game,
                         guint id,
                         MbRoomType type );



G_END_DECLS

#endif /* ifndef _MBCROOM_H_ */
