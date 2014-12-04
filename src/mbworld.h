/* mbworld.h -
 */

#ifndef _MBWORLD_H_
#define _MBWORLD_H_

#include "mbbase.h"
#include "mbworld-def.h"

G_BEGIN_DECLS



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



MbWorld *mb_world_new ( guint width,
                        guint height );



G_END_DECLS

#endif /* ifndef _MBWORLD_H_ */
