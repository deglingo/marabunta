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

  MbObject *colony;
  const gchar *name;
  gboolean isgroup;
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



#define MB_TASK_PARENT(task) (MB_TASK(task)->parent)
#define MB_TASK_CHILDREN(task) (MB_TASK(task)->children)
#define MB_TASK_COLONY(task) (MB_TASK(task)->colony)
#define MB_TASK_NAME(task) (MB_TASK(task)->name)
#define MB_TASK_ISGROUP(task) (MB_TASK(task)->isgroup)

void mb_task_add_workers ( MbTask *task,
                           gint64 count );
void mb_task_add ( MbTask *task,
                   MbObject *child );
MbObject *mb_task_get_child ( MbTask *task,
                              const gchar *name );
MbObject *mb_task_find ( MbTask *task,
                         const gchar *path );



G_END_DECLS

#endif /* ifndef _MBTASK_H_ */
