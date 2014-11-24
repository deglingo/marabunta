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
  MbTask *parent;
  GList *children;
  gint64 workers;
  MbObject *priority;
  MbObject *resource;
};



/* MbTaskClass:
 */
struct _MbTaskClass
{
  MB_TASK_CLASS_HEADER;

  void (* set_colony) ( MbTask *task,
                        MbObject *colony );
  
  void (* add) ( MbTask *task,
                 MbObject *child );

  void (* add_workers) ( MbTask *task,
                         gint64 workers );
};



#define MB_TASK_PARENT(task) (MB_TASK(task)->parent)
#define MB_TASK_CHILDREN(task) (MB_TASK(task)->children)
#define MB_TASK_COLONY(task) (MB_TASK(task)->colony)
#define MB_TASK_NAME(task) (MB_TASK(task)->name)
#define MB_TASK_ISGROUP(task) (MB_TASK(task)->isgroup)
#define MB_TASK_WORKERS(task) (MB_TASK(task)->workers)
#define MB_TASK_PRIORITY(task) (MB_TASK(task)->priority)
#define MB_TASK_RESOURCE(task) (MB_TASK(task)->resource)

void mb_task_set_colony ( MbTask *task,
                          MbObject *colony );
void mb_task_add ( MbTask *task,
                   MbObject *child );
MbObject *mb_task_get_child ( MbTask *task,
                              const gchar *name );
MbObject *mb_task_find ( MbTask *task,
                         const gchar *path );
void mb_task_add_workers ( MbTask *task,
                           gint64 workers );



G_END_DECLS

#endif /* ifndef _MBTASK_H_ */
