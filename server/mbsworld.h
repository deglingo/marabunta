/* mbsworld.h -
 */

#ifndef _MBSWORLD_H_
#define _MBSWORLD_H_

#include "server/mbsbase.h"
#include "server/mbssector.h"
#include "server/mbsworld-def.h"

G_BEGIN_DECLS



/* MbsWorld:
 */
struct _MbsWorld
{
  MBS_WORLD_INSTANCE_HEADER;

  guint width;
  guint height;
  MbsSector ***sectors;
};



/* MbsWorldClass:
 */
struct _MbsWorldClass
{
  MBS_WORLD_CLASS_HEADER;
};



MbsWorld *mbs_world_new ( guint width,
                          guint heigt );



G_END_DECLS

#endif /* ifndef _MBSWORLD_H_ */
