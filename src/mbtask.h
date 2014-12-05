/* mbtask.h -
 */

#ifndef _MBTASK_H_
#define _MBTASK_H_

#include "mbbase.h"
#include "mbpoptype.h"
#include "mbtask-def.h"

G_BEGIN_DECLS



/* MbTask:
 */
struct _MbTask
{
  MB_TASK_INSTANCE_HEADER;

  struct _MbColony *colony;
  MbTask *parent;
  gchar *name;
  MbPopFlags pop_flags;
};



/* MbTaskClass:
 */
struct _MbTaskClass
{
  MB_TASK_CLASS_HEADER;
};



MbTask *mb_task_new ( LObjectClass *cls,
                      MbTask *parent,
                      const gchar *name,
                      MbPopFlags pop_flags );



G_END_DECLS

#endif /* ifndef _MBTASK_H_ */
