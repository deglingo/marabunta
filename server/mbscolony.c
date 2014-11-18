/* mbscolony.c -
 */

#include "server/srvprivate.h"
#include "server/mbscolony.h"
#include "server/mbssector.h"
#include "server/mbsworld.h"
#include "server/mbsgame.h"
#include "server/mbstask.h"
#include "server/mbscolony.inl"



/* StockNode:
 */
typedef struct _StockNode
{
  MbsObject *resource;
  gint64 qtty;
}
  StockNode;



/* stock_node_new:
 */
static StockNode *stock_node_new ( void )
{
  /* [FIXME] use a trash stack */
  return g_new0(StockNode, 1);
}



/* stock_node_free:
 */
static void stock_node_free ( StockNode *node )
{
  g_free(node);
}



/* FoodData:
 */
typedef struct _FoodData
{
  MbsObjectID food_id;
}
  FoodData;



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



static void t_food_init ( MbsTask *task )
{
  FoodData *data;
  MbsGame *game = task->colony->sector->world->game;
  data = task->data = g_new0(FoodData, 1);
  data->food_id = mbs_game_get_resource(game, "food")->id;
}



static gboolean t_food_check ( MbsTask *task )
{
  return TRUE;
}



static void t_food_process ( MbsTask *task )
{
  FoodData *data = task->data;
  mbs_colony_add_stock(MBS_COLONY(task->colony), data->food_id, task->workers);
}



static gboolean t_mine_check ( MbsTask *task )
{
  return TRUE;
}



static void t_mine_process ( MbsTask *task )
{
  /* CL_DEBUG("[TODO]"); */
}



/* mbs_colony_new:
 */
MbsColony *mbs_colony_new ( MbsSector *sector,
                            guint owner )
{
  MbsTaskFuncs t_spawn_funcs =
    {
      NULL,
      t_spawn_check,
      t_spawn_process,
    };
  MbsTaskFuncs t_food_funcs =
    {
      t_food_init,
      t_food_check,
      t_food_process,
    };
  MbsTaskFuncs t_mine_funcs =
    {
      NULL,
      t_mine_check,
      t_mine_process,
    };
  MbsColony *col = MBS_COLONY_NEW(NULL);
  col->stock = g_hash_table_new_full(NULL, NULL,
                                     NULL,
                                     (GDestroyNotify) stock_node_free);
  col->sector = sector;
  col->owner = owner;
  col->pop_tree = mbs_pop_tree_new();
  col->pop_adj = mbs_pop_tree_new();
  { /* setup tasks */
    MbsTask *t_work, *t_farm, *t_food, *t_mine, *t_mine1, *t_mine2;
    col->top_task = mbs_task_new_group(col, NULL, "top", MB_POP_FLAG_ALL);
    l_object_unref(mbs_task_new(col, col->top_task, "spawn", MB_POP_FLAG_ADULT_QUEEN, &t_spawn_funcs));
    t_work = mbs_task_new_group(col, col->top_task, "work", MB_POP_FLAG_ADULT_WORKER);
    l_object_unref(t_work);    
    t_farm = mbs_task_new_group(col, t_work, "farm", MB_POP_FLAG_ADULT_WORKER);
    l_object_unref(t_farm);
    t_food = mbs_task_new(col, t_farm, "food", MB_POP_FLAG_ADULT_WORKER, &t_food_funcs);
    l_object_unref(t_food);
    t_mine = mbs_task_new_group(col, t_work, "mine", MB_POP_FLAG_ADULT_WORKER);
    l_object_unref(t_mine);
    t_mine1 = mbs_task_new(col, t_mine, "mine1", MB_POP_FLAG_ADULT_WORKER, &t_mine_funcs);
    l_object_unref(t_mine1);
    t_mine2 = mbs_task_new(col, t_mine, "mine2", MB_POP_FLAG_ADULT_WORKER, &t_mine_funcs);
    l_object_unref(t_mine2);
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



/* mbs_colony_set_stock:
 */
gint64 mbs_colony_set_stock ( MbsColony *colony,
                              MbsObjectID rscid,
                              gint64 qtty )
{
  StockNode *node;
  if ((node = g_hash_table_lookup(colony->stock,
                                  GUINT_TO_POINTER(rscid))))
    {
      if (qtty == 0)
        g_hash_table_remove(colony->stock, GUINT_TO_POINTER(rscid));
      else
        node->qtty = qtty;
    }
  else
    {
      if (qtty != 0)
        {
          StockNode *node = stock_node_new();
          node->resource = mbs_game_get_resource_by_id(colony->sector->world->game, rscid);
          ASSERT(node->resource);
          node->qtty = qtty;
          g_hash_table_insert(colony->stock, GUINT_TO_POINTER(rscid), node);
        }
    }
  return qtty;
}



/* mbs_colony_add_stock:
 */
gint64 mbs_colony_add_stock ( MbsColony *colony,
                              MbsObjectID rscid,
                              gint64 qtty )
{
  /* [FIXME] optimization possible */
  return mbs_colony_set_stock(colony,
                              rscid,
                              mbs_colony_get_stock(colony, rscid) + qtty);
}



/* mbs_colony_get_stock:
 */
gint64 mbs_colony_get_stock ( MbsColony *colony,
                              MbsObjectID rscid )
{
  StockNode *node;
  if ((node = g_hash_table_lookup(colony->stock, GUINT_TO_POINTER(rscid))))
    return node->qtty;
  else
    return 0;
}
