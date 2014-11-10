/* mbspoptree.c -
 */

#include "server/srvprivate.h"
#include "server/mbspoptree.h"



/* mbs_pop_unit_new:
 */
static MbsPopUnit *mbs_pop_unit_new ( MbPopType type,
                                      guint birthdate,
                                      gint64 count )
{
  MbsPopUnit *unit;
  unit = g_new0(MbsPopUnit, 1);
  unit->type = type;
  unit->birthdate = birthdate;
  unit->count = count;
  return unit;
}



/* mbs_pop_unit_free:
 */
static void mbs_pop_unit_free ( MbsPopUnit *unit )
{
  g_free(unit);
}



/* mbs_pop_unit_check:
 */
static inline gint mbs_pop_unit_check ( MbsPopUnit *unit,
                                        MbPopType type,
                                        guint birthdate )
{
  if (unit->type < type)
    return -1;
  else if (unit->type > type)
    return 1;
  else if (unit->birthdate < birthdate)
    return -1;
  else if (unit->birthdate > birthdate)
    return 1;
  else
    return 0;
}



/* mbs_pop_tree_new:
 */
MbsPopTree *mbs_pop_tree_new ( void )
{
  MbsPopTree *tree;
  tree = g_new0(MbsPopTree, 1);
  return tree;
}



static inline MbsPopUnit *lookup_unit ( MbsPopTree *tree,
                                        MbPopType type,
                                        guint birthdate )
{
  MbsPopUnit *unit;
  if (!tree->root)
    return NULL;
  unit = tree->root;
  while (unit->type != MB_POP_NONE)
    {
      gint d = mbs_pop_unit_check(unit, type, birthdate);
      if (d < 0)
        unit = unit->left;
      else if (d > 0)
        unit = unit->right;
      else
        return unit;
    }
  return NULL;
}



static void insert_unit ( MbsPopTree *tree,
                          MbsPopUnit *unit )
{
  tree->units = g_list_prepend(tree->units, unit);
}



/* mbs_pop_tree_add:
 */
void mbs_pop_tree_add ( MbsPopTree *tree,
                        MbPopType type,
                        guint birthdate,
                        gint64 count )
{
  MbsPopUnit *unit;
  if ((unit = lookup_unit(tree, type, birthdate)))
    {
      unit->count += count;
    }
  else
    {
      unit = mbs_pop_unit_new(type, birthdate, count);
      insert_unit(tree, unit);
    }
  tree->pop[type] += count;
  /* [fixme] ?? */
  if (unit->task)
    mbs_task_adjust_workers(unit->task, count);
}



/* mbs_pop_tree_traverse:
 */
void mbs_pop_tree_traverse ( MbsPopTree *tree,
                             MbsPopTreeTraverseFunc func,
                             gpointer data )
{
  GList *l;
  for (l = tree->units; l; l = l->next)
    {
      func(l->data, data);
    }
}



/* mbs_pop_tree_update:
 */
void mbs_pop_tree_update ( MbsPopTree *tree,
                           MbsPopTree *adj )
{
  GList *l;
  for (l = adj->units; l; l = l->next)
    {
      MbsPopUnit *unit = l->data;
      ASSERT(!unit->task);
      mbs_pop_tree_add(tree, unit->type, unit->birthdate, unit->count);
      adj->pop[unit->type] -= unit->count;
    }
  g_list_free_full(adj->units, (GDestroyNotify) mbs_pop_unit_free);
  adj->units = NULL;
}



/* mbs_pop_unit_affect_task:
 */
void mbs_pop_unit_affect_task ( MbsPopUnit *unit,
                                MbsTask *task )
{
  if (unit->task == task)
    return;
  if (unit->task)
    mbs_task_adjust_workers(unit->task, -unit->count);
  if ((unit->task = task))
    mbs_task_adjust_workers(task, unit->count);
}
