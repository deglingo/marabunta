/* mbctask.c -
 */

#include "client/mbctask.h"
#include "client/mbctask.inl"



/* mbc_task_new_group:
 */
MbObject *mbc_task_new_group ( MbObject *game,
                               MbObjectID id,
                               const gchar *name,
                               MbObject *priority )
{
  return MB_OBJECT(l_object_new_give(MBC_CLASS_TASK,
                                     "id", l_int_new(id),
                                     "game", l_object_ref(game),
                                     "name", l_string_new(name),
                                     "isgroup", l_int_new(1),
                                     "priority", l_object_ref(priority),
                                     NULL));
}



/* mbc_task_new:
 */
MbObject *mbc_task_new ( MbObject *game,
                         MbObjectID id,
                         const gchar *name,
                         MbObject *resource,
                         MbObject *priority )
{
  return MB_OBJECT(l_object_new_give(MBC_CLASS_TASK,
                                     "id", l_int_new(id),
                                     "game", l_object_ref(game),
                                     "name", l_string_new(name),
                                     "resource", (resource ? l_object_ref(resource) : l_none_ref()),
                                     "isgroup", l_int_new(0),
                                     "priority", l_object_ref(priority),
                                     NULL));
}
