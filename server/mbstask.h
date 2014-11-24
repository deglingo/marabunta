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

  void (* check) ( MbsTask *task );

  void (* process) ( MbsTask *task );
}
  MbsTaskFuncs;



/* MbsTask:
 */
struct _MbsTask
{
  MBS_TASK_INSTANCE_HEADER;

  MbsTaskFuncs funcs;
  guint ready_count[MB_POP_TYPE_COUNT];
  guint ready_sum;
  /* user data */
  gpointer data;
};



/* MbsTaskClass:
 */
struct _MbsTaskClass
{
  MBS_TASK_CLASS_HEADER;
};


/* debug */
#define MBS_TASK_CHECK_TREE(top) mbs_task_check_tree(MBS_TASK(top))

#define MBS_TASK_SCORE(task) (MBS_PRIORITY_SCORE(MB_TASK_PRIORITY(task)))

MbObject *mbs_task_new_group ( MbObject *game,
                               const gchar *name );
MbObject *mbs_task_new ( MbObject *game,
                         const gchar *name,
                         MbPopFlags pop_flags,
                         MbObject *resource,
                         MbsTaskFuncs *funcs );
void mbs_task_check_tree ( MbsTask *task );
void mbs_task_check ( MbsTask *task );
gboolean mbs_task_accept ( MbsTask *task,
                           MbPopType pop_type );
gint64 mbs_task_next_score ( MbsTask *task );
MbObject *mbs_task_select ( MbsTask *task,
                            MbPopType pop_type );
void mbs_task_set_ready ( MbsTask *task,
                          MbPopFlags flags );
gboolean mbs_task_ready ( MbsTask *task );
void mbs_task_process ( MbsTask *task );



G_END_DECLS

#endif /* ifndef _MBSTASK_H_ */
