/* mbroom.h -
 */

#ifndef _MBROOM_H_
#define _MBROOM_H_

#include "mbbase.h"
#include "mbroom-def.h"

G_BEGIN_DECLS



/* MbRoom:
 */
struct _MbRoom
{
  MB_ROOM_INSTANCE_HEADER;

  MbRoomType type;
  struct _MbColony *colony; /* set by MbColony */
  struct _MbTask *t_build; /* set by MbColony */
};



/* MbRoomClass:
 */
struct _MbRoomClass
{
  MB_ROOM_CLASS_HEADER;
};



MbRoom *mb_room_new ( MbRoomType type );



G_END_DECLS

#endif /* ifndef _MBROOM_H_ */
