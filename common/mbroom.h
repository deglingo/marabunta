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

  gint type;
  MbObject *colony;
  MbObject *work_task;
};



/* MbRoomClass:
 */
struct _MbRoomClass
{
  MB_ROOM_CLASS_HEADER;
};



#define MB_ROOM_TYPE(room) (MB_ROOM(room)->type)
#define MB_ROOM_COLONY(room) (MB_ROOM(room)->colony)
#define MB_ROOM_WORK_TASK(room) (MB_ROOM(room)->work_task)
#define MB_ROOM_X(room) (0.5) /* [TODO] */
#define MB_ROOM_Y(room) (0.5) /* [TODO] */
#define MB_ROOM_WIDTH(room) (0.1) /* [TODO] */
#define MB_ROOM_HEIGHT(room) (0.1) /* [TODO] */

void mb_room_set_colony ( MbRoom *room,
                          MbObject *colony );



G_END_DECLS

#endif /* ifndef _MBROOM_H_ */
