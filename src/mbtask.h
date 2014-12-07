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
  struct _MbPriority *priority;
  gint64 workers;
};



/* MbTaskClass:
 */
struct _MbTaskClass
{
  MB_TASK_CLASS_HEADER;

  MbTask * (* select) ( MbTask *task,
                        MbPopType pop_type );

  void (* process) ( MbTask *task );
};



MbTask *mb_task_new ( LObjectClass *cls,
                      MbTask *parent,
                      const gchar *name,
                      MbPopFlags pop_flags );
MbTask *mb_task_select ( MbTask *task,
                         MbPopType pop_type );
void mb_task_process ( MbTask *task );
void mb_task_add_workers ( MbTask *task,
                           gint64 workers );
gint64 mb_task_next_score ( MbTask *task );



G_END_DECLS

#endif /* ifndef _MBTASK_H_ */
