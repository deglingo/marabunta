/* mbstask.c -
 */

#include "server/mbstask.h"
#include "server/mbstask.inl"



/* mbs_task_new_group:
 */
MbObject *mbs_task_new_group ( MbObject *game,
                               const gchar *name,
                               MbPopFlags pop_flags )
{
  return MB_OBJECT(l_object_new_give(MBS_CLASS_TASK,
                                     "game", l_object_ref(game),
                                     "name", l_string_new(name),
                                     "pop_flags", l_int_new(pop_flags),
                                     "isgroup", l_int_new(1),
                                     NULL));
}



/* mbs_task_new:
 */
MbObject *mbs_task_new ( MbObject *game,
                         const gchar *name,
                         MbPopFlags pop_flags,
                         MbsTaskFuncs *funcs )
{
  MbObject *task = MB_OBJECT(l_object_new_give(MBS_CLASS_TASK,
                                               "game", l_object_ref(game),
                                               "name", l_string_new(name),
                                               "pop_flags", l_int_new(pop_flags),
                                               "isgroup", l_int_new(0),
                                               NULL));
  MBS_TASK(task)->funcs = *funcs;
  return task;
}
