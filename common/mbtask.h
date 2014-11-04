/* mbtask.h -
 */

#ifndef _MBTASK_H_
#define _MBTASK_H_

#include "common/mbbase.h"
#include "common/mbtask-def.h"

G_BEGIN_DECLS



/* MbTask:
 */
struct _MbTask
{
  MB_TASK_INSTANCE_HEADER;

  MbTask *parent;
  GList *subtasks;
  const gchar *name;
};



/* MbTaskClass:
 */
struct _MbTaskClass
{
  MB_TASK_CLASS_HEADER;
};



MbTask *mb_task_new ( MbTask *parent,
                      const gchar *name );



G_END_DECLS

#endif /* ifndef _MBTASK_H_ */
