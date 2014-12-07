/* mbtask.h -
 */

#ifndef _MBTASK_H_
#define _MBTASK_H_

#include "mbbase.h"
#include "mbpoptype.h"
#include "mbtask-def.h"

G_BEGIN_DECLS



/* MbTaskFuncs:
 */
typedef struct _MbTaskFuncs
{
  void (* init) ( MbTask *task,
                  gpointer data );

  gboolean (* check) ( MbTask *task );

  void (* process) ( MbTask *task );

  void (* finalize) ( MbTask *task );
}
  MbTaskFuncs;



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
  /* final task */
  MbPopType pop_type;
  MbTaskFuncs funcs;
  gpointer user_data;
  /* group stuff */
  gboolean isgroup;
  GList *children;
};



/* MbTaskClass:
 */
struct _MbTaskClass
{
  MB_TASK_CLASS_HEADER;
};



MbTask *mb_task_new_top ( struct _MbColony *colony,
                          const gchar *name );
MbTask *mb_task_new_group ( MbTask *parent,
                            const gchar *name );
MbTask *mb_task_new ( MbTask *parent,
                      const gchar *name,
                      MbPopType pop_type,
                      MbTaskFuncs *funcs,
                      gpointer data );
MbTask *mb_task_new_spawn ( MbTask *parent );
MbTask *mb_task_new_food ( MbTask *parent );
MbTask *mb_task_new_build ( MbTask *parent,
                            struct _MbRoom *room );

MbTask *mb_task_select ( MbTask *task,
                         MbPopType pop_type );
void mb_task_process ( MbTask *task );
void mb_task_add_workers ( MbTask *task,
                           gint64 workers );



G_END_DECLS

#endif /* ifndef _MBTASK_H_ */
