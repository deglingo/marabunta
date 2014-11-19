/* mbworld.h -
 */

#ifndef _MBWORLD_H_
#define _MBWORLD_H_

#include "common/mbobject.h"
#include "common/mbworld-def.h"



/* MbWorld:
 */
struct _MbWorld
{
  MB_WORLD_INSTANCE_HEADER;

  guint width;
  guint height;
  MbObject ***sectors;
};



/* MbWorldClass:
 */
struct _MbWorldClass
{
  MB_WORLD_CLASS_HEADER;
};



#define MB_WORLD_WIDTH(w) (MB_WORLD(w)->width)
#define MB_WORLD_HEIGHT(w) (MB_WORLD(w)->height)
#define MB_WORLD_SECTOR(w, x, y) (MB_WORLD(w)->sectors[y][x])

MbObject *mb_world_new ( MbObject *game,
                         MbObjectID id );
void mb_world_add_sector ( MbWorld *world,
                           MbObject *sector,
                           guint x,
                           guint y );



#endif
