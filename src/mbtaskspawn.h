/* mbtaskspawn.h -
 */

#ifndef _MBTASKSPAWN_H_
#define _MBTASKSPAWN_H_

#include "mbtask.h"
#include "mbtaskspawn-def.h"

G_BEGIN_DECLS



/* MbTaskSpawn:
 */
struct _MbTaskSpawn
{
  MB_TASK_SPAWN_INSTANCE_HEADER;
};



/* MbTaskSpawnClass:
 */
struct _MbTaskSpawnClass
{
  MB_TASK_SPAWN_CLASS_HEADER;
};



MbTask *mb_task_spawn_new ( MbTask *parent,
                            const gchar *name );



G_END_DECLS

#endif /* ifndef _MBTASKSPAWN_H_ */
