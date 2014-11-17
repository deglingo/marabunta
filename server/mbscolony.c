/* mbscolony.c -
 */

#include "server/srvprivate.h"
#include "server/mbscolony.h"
#include "server/mbssector.h"
#include "server/mbsworld.h"
#include "server/mbsgame.h"
#include "server/mbstask.h"
#include "server/mbscolony.inl"



static gboolean t_spawn_check ( MbsTask *task )
{
  return TRUE;
}



static void t_spawn_process ( MbsTask *task )
{
  MbsGame *game = task->colony->sector->world->game;
  mbs_colony_adjust_pop(task->colony,
                        MB_POP_EGG,
                        game->frame,
                        10);
}



/* mbs_colony_new:
 */
MbsColony *mbs_colony_new ( MbsSector *sector,
                            guint owner )
{
  MbsTaskFuncs t_spawn_funcs =
    {
      t_spawn_check,
      t_spawn_process,
    };
  MbsColony *col = MBS_COLONY_NEW(NULL);
  col->sector = sector;
  col->owner = owner;
  col->pop_tree = mbs_pop_tree_new();
  col->pop_adj = mbs_pop_tree_new();
  { /* setup tasks */
    MbsTask *t_work;
    col->top_task = mbs_task_new_group(col, NULL, "top", MB_POP_FLAG_ALL);
    l_object_unref(mbs_task_new(col, col->top_task, "spawn", MB_POP_FLAG_ADULT_QUEEN, &t_spawn_funcs));
    t_work = mbs_task_new_group(col, col->top_task, "work", MB_POP_FLAG_ADULT_WORKER);
    l_object_unref(t_work);
  }
  col->prio_pop_queen = mb_priority_new(1);
  col->prio_pop_worker = mb_priority_new(7);
  col->prio_pop_soldier = mb_priority_new(3);
  return col;
}



/* mbs_colony_select_task:
 */
MbsTask *mbs_colony_select_task ( MbsColony *colony,
                                  MbPopType pop_type )
{
  return mbs_task_select(colony->top_task, pop_type);
}



/* mbs_colony_adjust_pop:
 */
void mbs_colony_adjust_pop ( MbsColony *colony,
                             MbPopType type,
                             guint birthdate,
                             gint64 count )
{
  mbs_pop_tree_add(colony->pop_adj, type, birthdate, count);
}



/* mbs_colony_update_pop_tree:
 */
void mbs_colony_update_pop_tree ( MbsColony *colony )
{
  mbs_pop_tree_update(colony->pop_tree, colony->pop_adj);
}



/* _get_best_score:
 */
MbPopType _get_best_score ( MbsColony *colony,
                            gint64 *score )
{
  if (colony->score_pop_queen < colony->score_pop_worker)
    {
      if (colony->score_pop_queen < colony->score_pop_soldier) {
        if (score)
          *score = colony->score_pop_queen;
        return MB_POP_LARVAE_QUEEN;
      }
    }
  else
    {
      if (colony->score_pop_worker < colony->score_pop_soldier) {
        if (score)
          *score = colony->score_pop_worker;
        return MB_POP_LARVAE_WORKER;
      }
    }
  if (score)
    *score = colony->score_pop_soldier;
  return MB_POP_LARVAE_SOLDIER;
}



/* mbs_colony_select_hatch_cast:
 */
MbPopType mbs_colony_select_hatch_cast ( MbsColony *colony )
{
  return _get_best_score(colony, NULL);
}



/* mbs_colonyèadjust_hatch_score:
 */
void mbs_colony_adjust_hatch_score ( MbsColony *colony,
                                     MbPopType pop_type,
                                     gint64 score )
{
  switch (pop_type)
    {
    case MB_POP_LARVAE_QUEEN:
      colony->score_pop_queen += score * (10 - colony->prio_pop_queen->level);
      break;
    case MB_POP_LARVAE_WORKER:
      colony->score_pop_worker += score * (10 - colony->prio_pop_worker->level);
      break;
    case MB_POP_LARVAE_SOLDIER:
      colony->score_pop_soldier += score * (10 - colony->prio_pop_soldier->level);
      break;
    default:
      CL_ERROR("invalid pop_type: %d", pop_type);
    }
}



/* mbs_colony_adjust_hatch_scores:
 */
void mbs_colony_adjust_hatch_scores ( MbsColony *colony )
{
  gint64 score;
  /* [FIXME] ignore those which reached a maximum ? */
  _get_best_score(colony, &score);
  colony->score_pop_queen -= score;  
  colony->score_pop_worker -= score;  
  colony->score_pop_soldier -= score;  
}



/* mbs_colony_create_room:
 */
void mbs_colony_create_room ( MbsColony *colony,
                              MbRoomType type )
{
  ASSERT(!colony->rooms[type]);
  colony->rooms[type] = mbs_room_new(type);
}
