/* mbsroom.h -
 */

#ifndef _MBSROOM_H_
#define _MBSROOM_H_

#include "server/mbsbase.h"
#include "server/mbsroom-def.h"

G_BEGIN_DECLS



/* MbsRoom:
 */
struct _MbsRoom
{
  MBS_ROOM_INSTANCE_HEADER;
};



/* MbsRoomClass:
 */
struct _MbsRoomClass
{
  MBS_ROOM_CLASS_HEADER;
};



MbObject *mbs_room_new ( MbObject *game,
                         MbRoomType type );



G_END_DECLS

#endif /* ifndef _MBSROOM_H_ */
