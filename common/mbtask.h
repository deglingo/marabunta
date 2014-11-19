/* mbtask.h -
 */

#ifndef _MBTASK_H_
#define _MBTASK_H_

#include "common/mbobject.h"
#include "common/mbpoptype.h"
#include "common/mbtask-def.h"

G_BEGIN_DECLS



/* MbTask:
 */
struct _MbTask
{
  MB_TASK_INSTANCE_HEADER;

  const gchar *name;
  gboolean group;
  MbPopFlags pop_flags;
  MbTask *parent;
  GList *children;
};



/* MbTaskClass:
 */
struct _MbTaskClass
{
  MB_TASK_CLASS_HEADER;
};



void mb_task_add_workers ( MbTask *task,
                           gint64 count );
void mb_task_add ( MbTask *task,
                   MbObject *child );



G_END_DECLS

#endif /* ifndef _MBTASK_H_ */
