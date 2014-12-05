/* mbcolony.c -
 */

#include "mbcolony.h"
#include "mbsector.h"
#include "mbworld.h"
#include "mbgame.h"
#include "mbpoptree.h"
#include "mbtask.h"
#include "mbtaskgroup.h"
#include "mbtaskspawn.h"
#include "mbpriority.h"
#include "mbcolony.inl"



/* Signals:
 */
enum
  {
    SIG_POP_NOTIFY,
    SIG_COUNT,
  };

static LSignalID signals[SIG_COUNT];



/* Hatch:
 */
enum
  {
    HATCH_QUEEN = 0,
    HATCH_WORKER,
    HATCH_SOLDIER,
  };

static const MbPopType HATCH_TYPE[3] =
  {
    MB_POP_LARVAE_QUEEN,
    MB_POP_LARVAE_WORKER,
    MB_POP_LARVAE_SOLDIER,
  };



/* Update vtable:
 */
struct update_data
{
  MbColony *colony;
  guint birthdate_mask;
};

typedef void (* UpdateFunc) ( MbPopUnit *unit,
                              struct update_data *data );

static void _update_egg ( MbPopUnit *unit,
                          struct update_data *data );
static void _update_lq ( MbPopUnit *unit,
                         struct update_data *data );
static void _update_lw ( MbPopUnit *unit,
                         struct update_data *data );
static void _update_ls ( MbPopUnit *unit,
                         struct update_data *data );
static void _update_aq ( MbPopUnit *unit,
                         struct update_data *data );
static void _update_aw ( MbPopUnit *unit,
                         struct update_data *data );
static void _update_as ( MbPopUnit *unit,
                         struct update_data *data );

static const UpdateFunc UPDATE_VTABLE[MB_POP_TYPE_COUNT] =
    {
      [MB_POP_EGG] = _update_egg,
      [MB_POP_LARVAE_QUEEN] = _update_lq,
      [MB_POP_LARVAE_WORKER] = _update_lw,
      [MB_POP_LARVAE_SOLDIER] = _update_ls,
      [MB_POP_ADULT_QUEEN] = _update_aq,
      [MB_POP_ADULT_WORKER] = _update_aw,
      [MB_POP_ADULT_SOLDIER] = _update_as,
    };



/* mb_colony_class_init:
 */
static void mb_colony_class_init ( LObjectClass *cls )
{
  signals[SIG_POP_NOTIFY] =
    l_signal_new(cls,
                 "pop_notify",
                 0,
                 0,
                 NULL,
                 NULL,
                 NULL,
                 NULL);
}



/* mb_colony_new:
 */
MbColony *mb_colony_new ( void )
{
  MbColony *col;
  col = MB_COLONY(l_object_new(MB_CLASS_COLONY, NULL));
  col->pop_tree = mb_pop_tree_new(col->pop);
  col->adj_tree = mb_pop_tree_new(col->adj_pop);
  /* [FIXME] */
  mb_pop_tree_add(col->pop_tree, MB_POP_ADULT_QUEEN, 0, 1);
  /* hatch priorities */
  col->hatch_priority[HATCH_QUEEN] = mb_priority_new(10);
  col->hatch_priority[HATCH_WORKER] = mb_priority_new(70);
  col->hatch_priority[HATCH_SOLDIER] = mb_priority_new(30);
  /* default tasks */
  col->t_top = mb_task_group_new_top(col, "top");
  col->t_spawn = mb_task_spawn_new(col->t_top, "spawn");
  l_object_unref(col->t_spawn);
  return col;
}



/* _select_hatch_type:
 */
static gint _select_hatch_type ( MbColony *colony )
{
  gint found = -1;
  gint64 found_score = 0;
  gint tp;
  for (tp = 0; tp < 3; tp++)
    {
      /* [fixme] next score ? */
      gint64 score = mb_priority_score(colony->hatch_priority[tp]);
      if ((found < 0) || score < found_score)
        {
          found = tp;
          found_score = score;
        }
    }
  return found;
}



/* _update_egg:
 */
static void _update_egg ( MbPopUnit *unit,
                          struct update_data *data )
{
  MbGame *game = data->colony->sector->world->game;
  guint age = game->sim_time - unit->birthdate;
  if (age > 50)
    {
      guint hatch = _select_hatch_type(data->colony);
      MbPopType type = HATCH_TYPE[hatch];
      mb_colony_adjust_pop(data->colony, unit->type, unit->birthdate, -unit->count);
      mb_colony_adjust_pop(data->colony, type, unit->birthdate, unit->count);
      mb_priority_update_score(data->colony->hatch_priority[hatch], unit->count);
    }
}



static void _update_lq ( MbPopUnit *unit,
                         struct update_data *data )
{
}



static void _update_lw ( MbPopUnit *unit,
                         struct update_data *data )
{
}



static void _update_ls ( MbPopUnit *unit,
                         struct update_data *data )
{
}



static void _update_aq ( MbPopUnit *unit,
                         struct update_data *data )
{
  MbTask *task;
  if (unit->task)
    mb_pop_unit_affect_task(unit, NULL);
  if ((task = mb_task_select(data->colony->t_top, MB_POP_ADULT_QUEEN)))
    mb_pop_unit_affect_task(unit, task);
}



static void _update_aw ( MbPopUnit *unit,
                         struct update_data *data )
{
}



static void _update_as ( MbPopUnit *unit,
                         struct update_data *data )
{
}



/* _pop_unit_update:
 */
static void _pop_unit_update ( MbPopUnit *unit,
                               gpointer data_ )
{
  struct update_data *data = data_;
  if ((unit->birthdate & 0xf) != data->birthdate_mask)
    return;
  UPDATE_VTABLE[unit->type](unit, data_);
}



/* mb_colony_update:
 */
void mb_colony_update ( MbColony *colony )
{
  struct update_data data;
  data.colony = colony;
  data.birthdate_mask = g_random_int_range(0, 0x10);
  mb_pop_tree_traverse(colony->pop_tree,
                       _pop_unit_update,
                       &data);
  mb_pop_tree_update(colony->pop_tree, colony->adj_tree);
  l_signal_emit(L_OBJECT(colony), signals[SIG_POP_NOTIFY], 0, NULL);
  mb_task_process(colony->t_top);
}



/* mb_colony_adjust_pop:
 */
void mb_colony_adjust_pop ( MbColony *colony,
                            MbPopType type,
                            guint birthdate,
                            gint64 count )
{
  mb_pop_tree_add(colony->adj_tree, type, birthdate, count);
}
