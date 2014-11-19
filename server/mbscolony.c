/* mbscolony.c -
 */

#include "server/mbscolony.h"
#include "server/mbstask.h"
#include "server/mbscolony.inl"



/* mbs_colony_new:
 */
MbObject *mbs_colony_new ( MbObject *game )
{
  MbObject *col = MB_OBJECT(l_object_new(MBS_CLASS_COLONY,
                                         "game", game,
                                         NULL));
  MbObject *t_top, *t_spawn;
  MbsTaskFuncs t_spawn_funcs = { NULL, };
  /* create the default tasks */
  /* [fixme] is it the right place for this ? */
  t_top = mbs_task_new_group(game, "top", MB_POP_FLAG_ALL);
  mb_colony_set_top_task(MB_COLONY(col), t_top);
  l_object_unref(t_top);
  /* spawn */
  t_spawn = mbs_task_new(game, "spawn", MB_POP_FLAG_ADULT_QUEEN, &t_spawn_funcs);
  mb_task_add(MB_TASK(t_top), t_spawn);
  l_object_unref(t_spawn);
  return col;
}
