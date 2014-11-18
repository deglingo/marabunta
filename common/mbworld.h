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
};



/* MbWorldClass:
 */
struct _MbWorldClass
{
  MB_WORLD_CLASS_HEADER;
};



MbObject *mb_world_new ( MbObject *game,
                         MbObjectID id );



#endif
