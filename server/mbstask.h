/* mbstask.h -
 */

#ifndef _MBSTASK_H_
#define _MBSTASK_H_

#include "server/mbsbase.h"
#include "server/mbstask-def.h"

G_BEGIN_DECLS



/* MbsTaskFuncs:
 */
typedef struct _MbsTaskFuncs
{
  void (* init) ( MbsTask *task );

  gboolean (* ready) ( MbsTask *task );

  void (* process) ( MbsTask *task );
}
  MbsTaskFuncs;



/* MbsTask:
 */
struct _MbsTask
{
  MBS_TASK_INSTANCE_HEADER;

  MbsTaskFuncs funcs;
};



/* MbsTaskClass:
 */
struct _MbsTaskClass
{
  MBS_TASK_CLASS_HEADER;
};



MbObject *mbs_task_new_group ( MbObject *game,
                               const gchar *name,
                               MbPopFlags pop_flags );
MbObject *mbs_task_new ( MbObject *game,
                         const gchar *name,
                         MbPopFlags pop_flags,
                         MbsTaskFuncs *funcs );



G_END_DECLS

#endif /* ifndef _MBSTASK_H_ */
