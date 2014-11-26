/* mbscolony.c -
 */

#include "server/srvprivate.h"
#include "server/mbscolony.h"
#include "server/mbstask.h"
#include "server/mbspriority.h"
#include "server/mbsroom.h"
#include "server/mbscolony.inl"



static void set_sector ( MbColony *colony,
                         MbObject *sector );
static void add_room ( MbColony *colony,
                       MbObject *room );



static void t_spawn_process ( MbsTask *task )
{
  if (MB_TASK_WORKERS(task) > 0)
    {
      MbsColony *colony = MBS_COLONY(MB_TASK_COLONY(task));
      guint date = MB_GAME_FRAME_COUNT(MB_OBJECT_GAME(colony));
      gint count = g_random_int_range(0, 10 * MB_TASK_WORKERS(task));
      mbs_pop_tree_add(colony->adj_tree, MB_POP_EGG, date, count);
    }
}



typedef struct _FoodData
{
  MbObject *rsc_food;
}
  FoodData;



static void t_food_init ( MbsTask *task )
{
  FoodData *data;
  data = task->data = g_new0(FoodData, 1);
  data->rsc_food = mb_game_lookup_resource(MB_GAME(MB_OBJECT_GAME(task)), "food");
}



static void t_food_process ( MbsTask *task )
{
  if (MB_TASK_WORKERS(task) > 0)
    {
      mb_colony_add_stock(MB_COLONY(MB_TASK_COLONY(task)),
                          MB_OBJECT_ID(((FoodData *) task->data)->rsc_food),
                          MB_TASK_WORKERS(task) * 5);
    }
}



/* MineData:
 */
typedef struct _MineData
{
  GList *veins;
}
  MineData;

#define MINE_DATA(task) ((MineData *)(MBS_TASK(task)->data))



static void t_mine_init ( MbsTask *task )
{
  GList *l;
  gint64 total = 0;
  task->data = g_new0(MineData, 1);
  for (l = MB_SECTOR(MB_COLONY_SECTOR(MB_TASK_COLONY(task)))->veins; l; l = l->next)
    {
      if (MB_VEIN_RESOURCE(l->data) == MB_TASK_RESOURCE(task))
        MINE_DATA(task)->veins = g_list_append(MINE_DATA(task)->veins,
                                               l_object_ref(l->data));
      total += MB_VEIN_QTTY(l->data);
    }
  CL_DEBUG("mine task '%s' : %" G_GINT64_FORMAT,
           MB_RESOURCE_NAME(MB_TASK_RESOURCE(task)),
           total);
}



static void t_mine_check ( MbsTask *task )
{
  if (!MINE_DATA(task)->veins)
    {
      mbs_task_set_ready(task, 0);
    }
}



static void t_mine_process ( MbsTask *task )
{
  if (MB_TASK_WORKERS(task) > 0)
    {
      gint64 prod = MB_TASK_WORKERS(task);
      gint64 real_prod = 0;
      while (MINE_DATA(task)->veins && prod > 0)
        {
          MbObject *vein = MINE_DATA(task)->veins->data;
          if (MB_VEIN_QTTY(vein) > prod)
            {
              real_prod += prod;
              MB_VEIN(vein)->qtty -= prod;
              prod = 0;
            }
          else
            {
              real_prod += MB_VEIN(vein)->qtty;
              prod -= MB_VEIN(vein)->qtty;
              MB_VEIN(vein)->qtty = 0;
              MINE_DATA(task)->veins = g_list_delete_link(MINE_DATA(task)->veins,
                                                          MINE_DATA(task)->veins);
              l_object_unref(vein);
            }
        }
      mb_colony_add_stock(MB_COLONY(MB_TASK_COLONY(task)),
                          MB_OBJECT_ID(MB_TASK_RESOURCE(task)),
                          real_prod);
    }
}



/* BuildData:
 */
typedef struct _BuildData
{
  int dummy;
}
  BuildData;

#define BUILD_DATA(task) ((BuildData *)(MBS_TASK(task)))



/* t_build_init:
 */
static void t_build_init ( MbsTask *task )
{
}



/* t_build_check:
 */
static void t_build_check ( MbsTask *task )
{
}



/* t_build_process:
 */
static void t_build_process ( MbsTask *task )
{
}



/* mbs_colony_class_init:
 */
static void mbs_colony_class_init ( LObjectClass *cls )
{
  MB_COLONY_CLASS(cls)->set_sector = set_sector;
  MB_COLONY_CLASS(cls)->add_room = add_room;
}



/* mbs_colony_new:
 */
MbObject *mbs_colony_new ( MbObject *game )
{
  MbObject *col = MB_OBJECT(l_object_new(MBS_CLASS_COLONY,
                                         "game", game,
                                         NULL));
  MbObject *t_top, *t_spawn, *t_work, *t_farm, *t_food, *t_mine, *t_build;
  MbsTaskFuncs t_spawn_funcs = { NULL, NULL, t_spawn_process };
  MbsTaskFuncs t_food_funcs = { t_food_init, NULL, t_food_process };
  MbObject *room;
  /* create the pop trees */
  MBS_COLONY(col)->pop_tree = mbs_pop_tree_new(MB_COLONY(col)->pop);
  MBS_COLONY(col)->adj_tree = mbs_pop_tree_new(MBS_COLONY(col)->adj_pop);
  /* hatch priorities */
  MBS_COLONY(col)->hatch_priority_queen =
    mbs_priority_new(game, 1);
  MBS_COLONY(col)->hatch_priority_worker =
    mbs_priority_new(game, 7);
  MBS_COLONY(col)->hatch_priority_soldier =
    mbs_priority_new(game, 2);
  /* [FIXME] */
  mbs_pop_tree_add(MBS_COLONY(col)->pop_tree,
                   MB_POP_ADULT_QUEEN,
                   0, 1);
  /* room = mbs_room_new(game, MB_ROOM_TYPE_FARM); */
  /* mb_colony_add_room(MB_COLONY(col), room); */
  /* l_object_unref(room); */
  /* room = mbs_room_new(game, MB_ROOM_TYPE_MINE); */
  /* mb_colony_add_room(MB_COLONY(col), room); */
  /* l_object_unref(room); */
  /* create the default tasks */
  /* [fixme] is it the right place for this ? */
  t_top = mbs_task_new_group(game, "top");
  mb_colony_set_top_task(MB_COLONY(col), t_top);
  l_object_unref(t_top);
  /* spawn */
  t_spawn = mbs_task_new(game, "spawn", MB_POP_FLAG_ADULT_QUEEN, NULL, &t_spawn_funcs);
  mb_task_add(MB_TASK(t_top), t_spawn);
  l_object_unref(t_spawn);
  /* work */
  t_work = mbs_task_new_group(game, "work");
  mb_task_add(MB_TASK(t_top), t_work);
  l_object_unref(t_work);
  /* build */
  t_build = mbs_task_new_group(game, "build");
  mb_task_add(MB_TASK(t_work), t_build);
  l_object_unref(t_build);
  /* farm */
  t_farm = mbs_task_new_group(game, "farm");
  mb_task_add(MB_TASK(t_work), t_farm);
  l_object_unref(t_farm);
  /* food */
  t_food = mbs_task_new(game, "food", MB_POP_FLAG_ADULT_WORKER,
                        mb_game_lookup_resource(MB_GAME(game), "food"),
                        &t_food_funcs);
  mb_task_add(MB_TASK(t_farm), t_food);
  l_object_unref(t_food);
  /* mine */
  t_mine = mbs_task_new_group(game, "mine");
  mb_task_add(MB_TASK(t_work), t_mine);
  l_object_unref(t_mine);
  /* create the default rooms */
  {
    MbRoomType type = mb_game_lookup_room_type(MB_GAME(game), "royal-chamber");
    ASSERT(type);
    room = mbs_room_new(game, type);
    MB_ROOM(room)->x = 0.5;
    MB_ROOM(room)->y = 0.5;
    MB_ROOM(room)->width = 0.1;
    MB_ROOM(room)->height = 0.1;
    mb_colony_add_room(MB_COLONY(col), room);
    l_object_unref(room);
  }
  return col;
}



/* set_sector:
 */
static void set_sector ( MbColony *colony,
                         MbObject *sector )
{
  MbObject *t_mine;
  GList *l;
  MbsTaskFuncs funcs = { t_mine_init, t_mine_check, t_mine_process };
  MB_COLONY_CLASS(parent_class)->set_sector(colony, sector);
  /* create the mine tasks */
  t_mine = mb_task_find(MB_TASK(MB_COLONY_TOP_TASK(colony)), "work/mine");
  ASSERT(t_mine);
  for (l = MB_SECTOR(sector)->veins; l; l = l->next)
    {
      MbObject *vein = l->data;
      MbObject *task;
      CL_TRACE("%s", MB_RESOURCE_NAME(MB_VEIN_RESOURCE(vein)));
      task = mb_task_get_child(MB_TASK(t_mine),
                               MB_RESOURCE_NAME(MB_VEIN_RESOURCE(vein)));
      if (task)
        continue;
      task = mbs_task_new(MB_OBJECT_GAME(sector),
                          MB_RESOURCE_NAME(MB_VEIN_RESOURCE(vein)),
                          MB_POP_FLAG_ADULT_WORKER,
                          MB_VEIN_RESOURCE(vein),
                          &funcs);
      mb_task_add(MB_TASK(t_mine), task);
      l_object_unref(task);
    }
}



/* add_room:
 */
static void add_room ( MbColony *colony,
                       MbObject *room )
{
  MbObject *t_build, *t_room;
  MbObject *game = MB_OBJECT_GAME(colony);
  const MbRoomTypeInfo *info = mb_game_get_room_type_info(MB_GAME(game), MB_ROOM_TYPE(room));
  MbsTaskFuncs t_build_funcs = {
    t_build_init,
    t_build_check,
    t_build_process,
  };
  MB_COLONY_CLASS(parent_class)->add_room(colony, room);
  t_build = mb_task_find(MB_TASK(MB_COLONY_TOP_TASK(colony)),
                         "work/build");
  ASSERT(t_build);
  t_room = mbs_task_new(MB_OBJECT_GAME(colony),
                        info->nick,
                        MB_POP_FLAG_ADULT_WORKER,
                        NULL,
                        &t_build_funcs);
  mb_task_add(MB_TASK(t_build), t_room);
}



/* mbs_colony_get_pop:
 */
void mbs_colony_get_pop ( MbsColony *colony,
                          gint64 *pop )
{
  mbs_pop_tree_get_pop(colony->pop_tree, pop);
}



/* mbs_colony_select_task:
 */
MbObject *mbs_colony_select_task ( MbsColony *colony,
                                   MbPopType pop_type )
{
  return mbs_task_select(MBS_TASK(MB_COLONY_TOP_TASK(colony)), pop_type);
}



static MbCast _select_hatch_cast ( MbsColony *colony,
                                   MbObject **prio )
{
  if (MBS_PRIORITY_SCORE(colony->hatch_priority_queen) <= MBS_PRIORITY_SCORE(colony->hatch_priority_worker) &&
      MBS_PRIORITY_SCORE(colony->hatch_priority_queen) <= MBS_PRIORITY_SCORE(colony->hatch_priority_soldier))
    {
      *prio = colony->hatch_priority_queen;
      return MB_CAST_QUEEN;
    }
  else if (MBS_PRIORITY_SCORE(colony->hatch_priority_worker) <= MBS_PRIORITY_SCORE(colony->hatch_priority_queen) &&
           MBS_PRIORITY_SCORE(colony->hatch_priority_worker) <= MBS_PRIORITY_SCORE(colony->hatch_priority_soldier))
    {
      *prio = colony->hatch_priority_worker;
      return MB_CAST_WORKER;
    }
  else
    {
      *prio = colony->hatch_priority_soldier;
      return MB_CAST_SOLDIER;
    }
}



/* _update_egg:
 */
static void _update_egg ( MbsColony *colony,
                          MbsPopUnit *unit )
{
  guint date = MB_GAME_FRAME_COUNT(MB_OBJECT_GAME(colony));
  guint age = date - unit->birthdate;
  if (age > 500)
    {
      MbObject *priority;
      MbCast pop_cast = _select_hatch_cast(colony, &priority);
      MbPopType pop_type = mb_pop_type(pop_cast, MB_MATURITY_LARVAE);
      mbs_pop_tree_add(colony->adj_tree, unit->type, unit->birthdate, -unit->count);
      mbs_pop_tree_add(colony->adj_tree, pop_type, unit->birthdate, unit->count);
      mbs_priority_update_score(MBS_PRIORITY(priority),
                                unit->count);
    }
}



/* _update_lq:
 */
static void _update_lq ( MbsColony *colony,
                         MbsPopUnit *unit )
{
  guint date = MB_GAME_FRAME_COUNT(MB_OBJECT_GAME(colony));
  guint age = date - unit->birthdate;
  if (age > 1000)
    {
      mbs_pop_tree_add(colony->adj_tree, MB_POP_LARVAE_QUEEN, unit->birthdate, -unit->count);
      /* no new queens for now */
      /* mbs_pop_tree_add(colony->adj_tree, MB_POP_ADULT_QUEEN, unit->birthdate, unit->count); */
    }
}



/* _update_lw:
 */
static void _update_lw ( MbsColony *colony,
                         MbsPopUnit *unit )
{
  guint date = MB_GAME_FRAME_COUNT(MB_OBJECT_GAME(colony));
  guint age = date - unit->birthdate;
  if (age > 1000)
    {
      mbs_pop_tree_add(colony->adj_tree, MB_POP_LARVAE_WORKER, unit->birthdate, -unit->count);
      mbs_pop_tree_add(colony->adj_tree, MB_POP_ADULT_WORKER, unit->birthdate, unit->count);
    }
}



/* _update_ls:
 */
static void _update_ls ( MbsColony *colony,
                         MbsPopUnit *unit )
{
  guint date = MB_GAME_FRAME_COUNT(MB_OBJECT_GAME(colony));
  guint age = date - unit->birthdate;
  if (age > 1000)
    {
      mbs_pop_tree_add(colony->adj_tree, MB_POP_LARVAE_SOLDIER, unit->birthdate, -unit->count);
      mbs_pop_tree_add(colony->adj_tree, MB_POP_ADULT_SOLDIER, unit->birthdate, unit->count);
    }
}



/* _update_aq:
 */
static void _update_aq ( MbsColony *colony,
                         MbsPopUnit *unit )
{
  MbObject *task;
  task = mbs_colony_select_task(colony, MB_POP_ADULT_QUEEN);
  mbs_pop_unit_affect_task(unit, task);
  /* CL_DEBUG("affect aq: %p (%s) -> %" G_GINT64_FORMAT, */
  /*          task, (task ? MB_TASK_NAME(task) : ""), (task ? MB_TASK_WORKERS(task) : 0)); */
}



/* _update_aw:
 */
static void _update_aw ( MbsColony *colony,
                         MbsPopUnit *unit )
{
  MbObject *task;
  task = mbs_colony_select_task(colony, MB_POP_ADULT_WORKER);
  mbs_pop_unit_affect_task(unit, task);
}



/* _update_as:
 */
static void _update_as ( MbsColony *colony,
                         MbsPopUnit *unit )
{
  /* [TODO] */
}



struct update_data
{
  MbsColony *colony;
  guint birthdate_mask;
};



/* _update_pop_unit:
 */
static void _update_pop_unit ( MbsPopUnit *unit,
                               struct update_data *data )
{
  if ((unit->birthdate & 0xf) != data->birthdate_mask)
    return;
  switch (unit->type)
    {
    case MB_POP_EGG:
      _update_egg(data->colony, unit);
      break;
    case MB_POP_LARVAE_QUEEN:
      _update_lq(data->colony, unit);
      break;
    case MB_POP_LARVAE_WORKER:
      _update_lw(data->colony, unit);
      break;
    case MB_POP_LARVAE_SOLDIER:
      _update_ls(data->colony, unit);
      break;
    case MB_POP_ADULT_QUEEN:
      _update_aq(data->colony, unit);
      break;
    case MB_POP_ADULT_WORKER:
      _update_aw(data->colony, unit);
      break;
    case MB_POP_ADULT_SOLDIER:
      _update_as(data->colony, unit);
      break;
    default:
      CL_DEBUG("[TODO] type %d", unit->type);
    }
}



/* mbs_colony_update:
 */
void mbs_colony_update ( MbsColony *colony )
{
  gint64 min_score;
  struct update_data data;
  data.colony = colony;
  data.birthdate_mask = g_random_int_range(0, 0x10);
  /* [FIXME] debug only */
  MBS_TASK_CHECK_TREE(MB_COLONY_TOP_TASK(colony));
  /* update pop units */
  mbs_pop_tree_traverse(colony->pop_tree,
                        (MbsPopTreeTraverseFunc) _update_pop_unit,
                        &data);
  /* adjust hatch scores */
  min_score = MIN(MBS_PRIORITY_SCORE(colony->hatch_priority_queen),
                  MBS_PRIORITY_SCORE(colony->hatch_priority_worker));
  min_score = MIN(min_score,
                  MBS_PRIORITY_SCORE(colony->hatch_priority_soldier));
  MBS_PRIORITY(colony->hatch_priority_queen)->score.score -= min_score;
  MBS_PRIORITY(colony->hatch_priority_worker)->score.score -= min_score;
  MBS_PRIORITY(colony->hatch_priority_soldier)->score.score -= min_score;
  /* [FIXME] post update */
  mbs_pop_tree_update(colony->pop_tree, colony->adj_tree);
  mb_colony_pop_notify(MB_COLONY(colony));
  /* process all tasks */
  mbs_task_process(MBS_TASK(MB_COLONY_TOP_TASK(colony)));
  /* [FIXME] debug only */
  MBS_TASK_CHECK_TREE(MB_COLONY_TOP_TASK(colony));
}
