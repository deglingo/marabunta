/* mbstask.h -
 */

#ifndef _MBSTASK_H_
#define _MBSTASK_H_

#include "server/mbsobject.h"
#include "server/mbstask-def.h"

G_BEGIN_DECLS

struct _MbsColony;



/* MbsTaskFuncs:
 */
typedef struct _MbsTaskFuncs
{
  gboolean (* check) ( MbsTask *task,
                       MbPopType pop_type );
  
  void (* process) ( MbsTask *task );
}
  MbsTaskFuncs;



/* MbsTask:
 */
struct _MbsTask
{
  MBS_TASK_INSTANCE_HEADER;

  struct _MbsColony *colony;
  MbsTask *parent;
  GList *children;
  gchar *name;
  MbsTaskFuncs funcs;
  gboolean isgroup;
  gint64 score;
  gint64 workers;
};



/* MbsTaskClass:
 */
struct _MbsTaskClass
{
  MBS_TASK_CLASS_HEADER;
};



extern const MbsTaskFuncs MBS_TASK_GROUP_FUNCS;

MbsTask *mbs_task_new ( struct _MbsColony *colony,
                        MbsTask *parent,
                        const gchar *name,
                        MbsTaskFuncs *funcs );
MbsTask *mbs_task_new_group ( struct _MbsColony *colony,
                              MbsTask *parent,
                              const gchar *name,
                              MbsTaskFuncs *funcs );
gboolean mbs_task_check ( MbsTask *task,
                          MbPopType pop_type );
void mbs_task_process ( MbsTask *task );
gint64 mbs_task_get_score ( MbsTask *task );
gint64 mbs_task_get_next_score ( MbsTask *task );
void mbs_task_adjust_workers ( MbsTask *task,
                               gint64 count );



G_END_DECLS

#endif /* ifndef _MBSTASK_H_ */
