/* mbpoptree.c -
 */

#include "common/private.h"
#include "common/mbpoptree.h"



/* MB_MATURITY_TABLE:
 */
const MbMaturity MB_MATURITY_TABLE[MB_POP_TYPE_COUNT] =
  {
    [MB_POP_EGG] = MB_MATURITY_EGG,
    [MB_POP_LARVAE_QUEEN] = MB_MATURITY_LARVAE,
    [MB_POP_LARVAE_WORKER] = MB_MATURITY_LARVAE,
    [MB_POP_LARVAE_SOLDIER] = MB_MATURITY_LARVAE,
    [MB_POP_ADULT_QUEEN] = MB_MATURITY_ADULT,
    [MB_POP_ADULT_WORKER] = MB_MATURITY_ADULT,
    [MB_POP_ADULT_SOLDIER] = MB_MATURITY_ADULT,
  };



/* MB_CAST_TABLE:
 */
const MbCast MB_CAST_TABLE[MB_POP_TYPE_COUNT] =
  {
    [MB_POP_EGG] = MB_CAST_EGG, /* ?? */
    [MB_POP_LARVAE_QUEEN] = MB_CAST_QUEEN,
    [MB_POP_LARVAE_WORKER] = MB_CAST_WORKER,
    [MB_POP_LARVAE_SOLDIER] = MB_CAST_SOLDIER,
    [MB_POP_ADULT_QUEEN] = MB_CAST_QUEEN,
    [MB_POP_ADULT_WORKER] = MB_CAST_WORKER,
    [MB_POP_ADULT_SOLDIER] = MB_CAST_SOLDIER,
  };



/* mb_pop_type:
 */
MbPopType mb_pop_type ( MbCast pop_cast,
                        MbMaturity pop_maturity )
{
  gint tp;
  /* [fixme] maybe a switch is more efficient ? */
  for (tp = 0; tp < MB_POP_TYPE_COUNT; tp++) {
    if (MB_POP_CAST(tp) == pop_cast && MB_POP_MATURITY(tp) == pop_maturity)
      return tp;
  }
  CL_ERROR("invalid pop type: cast=%d, maturity=%d", pop_cast, pop_maturity);
  return -1;
}



/* mb_pop_unit_new:
 */
static MbPopUnit *mb_pop_unit_new ( MbPopType type,
                                    guint birthdate,
                                    gint64 count )
{
  MbPopUnit *unit;
  unit = g_new0(MbPopUnit, 1);
  unit->type = type;
  unit->birthdate = birthdate;
  unit->count = count;
  return unit;
}



/* mb_pop_unit_free:
 */
static void mb_pop_unit_free ( MbPopUnit *unit )
{
  g_free(unit);
}



/* mb_pop_tree_new:
 */
MbPopTree *mb_pop_tree_new ( void )
{
  MbPopTree *tree;
  tree = g_new0(MbPopTree, 1);
  return tree;
}



static MbPopUnit *lookup_unit ( MbPopTree *tree,
                                MbPopType type,
                                guint birthdate )
{
  GList *l;
  for (l = tree->units; l; l = l->next)
    {
      MbPopUnit *u = l->data;
      if (u->type == type && u->birthdate == birthdate)
        return u;
    }
  return NULL;
}



static void insert_unit ( MbPopTree *tree,
                          MbPopUnit *unit )
{
  tree->units = g_list_prepend(tree->units, unit);
}



/* mb_pop_tree_add:
 */
void mb_pop_tree_add ( MbPopTree *tree,
                       MbPopType type,
                       guint birthdate,
                       gint64 count )
{
  MbPopUnit *unit;
  if ((unit = lookup_unit(tree, type, birthdate)))
    {
      unit->count += count;
    }
  else
    {
      unit = mb_pop_unit_new(type, birthdate, count);
      insert_unit(tree, unit);
    }
  tree->pop[type] += count;
}



/* mb_pop_tree_traverse:
 */
void mb_pop_tree_traverse ( MbPopTree *tree,
                            MbPopTreeTraverseFunc func,
                            gpointer data )
{
  GList *l;
  for (l = tree->units; l; l = l->next)
    {
      func(l->data, data);
    }
}



/* mb_pop_tree_update:
 */
void mb_pop_tree_update ( MbPopTree *tree,
                          MbPopTree *adj )
{
  GList *l;
  for (l = adj->units; l; l = l->next)
    {
      MbPopUnit *unit = l->data;
      /* [FIXME] should adjust task workers count!! */
      mb_pop_tree_add(tree, unit->type, unit->birthdate, unit->count);
      adj->pop[unit->type] -= unit->count;
    }
  g_list_free_full(adj->units, (GDestroyNotify) mb_pop_unit_free);
  adj->units = NULL;
}
