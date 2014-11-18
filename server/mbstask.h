/* mbstask.h -
 */

#ifndef _MBSTASK_H_
#define _MBSTASK_H_

#include "server/mbsobject.h"
#include "server/mbspriority.h"
#include "server/mbstask-def.h"

G_BEGIN_DECLS

struct _MbsColony;



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

  struct _MbsColony *colony;
  MbsTask *parent;
  gchar *name;
  MbPopFlags accept_flags;
  gboolean ready;
  MbsTaskFuncs funcs;
  gboolean isgroup;
  GList *children;
  guint children_ready;
  MbsPriority *priority;
  gint64 score;
  gint64 workers;
  /* user data */
  gpointer data;
};



/* MbsTaskClass:
 */
struct _MbsTaskClass
{
  MBS_TASK_CLASS_HEADER;
};



MbsTask *mbs_task_new ( struct _MbsColony *colony,
                        MbsTask *parent,
                        const gchar *name,
                        MbPopFlags accept_flags,
                        MbsTaskFuncs *funcs );
MbsTask *mbs_task_new_group ( struct _MbsColony *colony,
                              MbsTask *parent,
                              const gchar *name,
                              MbPopFlags accept_flags );
void mbs_task_set_ready ( MbsTask *task,
                          gboolean ready );
MbsTask *mbs_task_select ( MbsTask *task,
                           MbPopType type );
void mbs_task_process ( MbsTask *task );
gint64 mbs_task_get_score ( MbsTask *task );
gint64 mbs_task_get_next_score ( MbsTask *task );
void mbs_task_adjust_workers ( MbsTask *task,
                               gint64 count );



G_END_DECLS

#endif /* ifndef _MBSTASK_H_ */
