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
  gfloat x;
  gfloat y;
  gfloat width;
  gfloat height;
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
#define MB_ROOM_X(room) (MB_ROOM(room)->x)
#define MB_ROOM_Y(room) (MB_ROOM(room)->y)
#define MB_ROOM_WIDTH(room) (MB_ROOM(room)->width)
#define MB_ROOM_HEIGHT(room) (MB_ROOM(room)->height)

void mb_room_set_colony ( MbRoom *room,
                          MbObject *colony );



G_END_DECLS

#endif /* ifndef _MBROOM_H_ */
