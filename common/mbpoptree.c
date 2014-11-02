/* mbpoptree.c -
 */

#include "common/mbpoptree.h"



/* MB_MATURITY_TABLE:
 */
const MbMaturity MB_MATURITY_TABLE[MB_POP_TYPE_COUNT] =
  {
    [MB_POP_EGG] = MB_MATURITY_EGG,
    [MB_POP_LARVAE_QUEEN] = MB_MATURITY_LARVAE,
    [MB_POP_LARVAE_WORKER] = MB_MATURITY_LARVAE,
    [MB_POP_LARVAE_SOLDIER] = MB_MATURITY_LARVAE,
    [MB_POP_ADULT_QUEEN] = MB_MATURITY_LARVAE,
    [MB_POP_ADULT_WORKER] = MB_MATURITY_LARVAE,
    [MB_POP_ADULT_SOLDIER] = MB_MATURITY_LARVAE,
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



/* mb_pop_unit_new:
 */
static MbPopUnit *mb_pop_unit_new ( MbPopType type,
                                    guint birthdate,
                                    gint64 count )
{
  MbPopUnit *unit;
  unit = g_new(MbPopUnit, 1);
  unit->type = type;
  unit->birthdate = birthdate;
  unit->count = count;
  return unit;
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
