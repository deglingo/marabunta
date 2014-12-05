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

  struct _MbGame *game; /* set by MbGame */
  guint width;
  guint height;
  struct _MbSector ***sectors;
};



/* MbWorldClass:
 */
struct _MbWorldClass
{
  MB_WORLD_CLASS_HEADER;
};



#define MB_WORLD_SECTOR(w, x, y) (MB_WORLD(w)->sectors[y][x])

MbWorld *mb_world_new ( guint width,
                        guint height );
void mb_world_update ( MbWorld *world );



G_END_DECLS

#endif /* ifndef _MBWORLD_H_ */
