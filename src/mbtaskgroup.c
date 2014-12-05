/* mbtaskgroup.c -
 */

#include "mbtaskgroup.h"
#include "mbtaskgroup.inl"



static MbTask *_select ( MbTask *task,
                         MbPopType pop_type );
static void _process ( MbTask *task );



/* mb_task_group_class_init:
 */
static void mb_task_group_class_init ( LObjectClass *cls )
{
  MB_TASK_CLASS(cls)->select = _select;
  MB_TASK_CLASS(cls)->process = _process;
}



/* mb_task_group_new_top:
 */
MbTask *mb_task_group_new_top ( struct _MbColony *colony,
                                const gchar *name )
{
  MbTask *task;
  task = mb_task_new(MB_CLASS_TASK_GROUP,
                     NULL,
                     name,
                     0);
  task->colony = colony; /* [FIXME] ref ? */
  return task;
}



/* _select:
 */
static MbTask *_select ( MbTask *task,
                         MbPopType pop_type )
{
  GList *l;
  MbTask *found = NULL;
  gint64 found_score = 0;
  for (l = MB_TASK_GROUP(task)->children; l; l = l->next)
    {
      MbTask *sel;
      if ((sel = mb_task_select(l->data, pop_type)))
        {
          gint64 score = mb_task_next_score(sel);
          if ((!found) || score < found_score)
            {
              found = sel;
              found_score = score;
            }
        }
    }
  return found;
}



/* _process:
 */
static void _process ( MbTask *task )
{
  GList *l;
  for (l = MB_TASK_GROUP(task)->children; l; l = l->next)
    mb_task_process(l->data);
}
