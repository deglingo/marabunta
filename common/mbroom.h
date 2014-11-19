/* mbroom.h -
 */

#ifndef _MBROOM_H_
#define _MBROOM_H_

#include "common/mbobject.h"
#include "common/mbroom-def.h"

G_BEGIN_DECLS



/* MbRoom:
 */
struct _MbRoom
{
  MB_ROOM_INSTANCE_HEADER;

  MbRoomType type;
  MbObject *colony;
};



/* MbRoomClass:
 */
struct _MbRoomClass
{
  MB_ROOM_CLASS_HEADER;
};



#define MB_ROOM_TYPE(room) (MB_ROOM(room)->type)



G_END_DECLS

#endif /* ifndef _MBROOM_H_ */
