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



/* mb_pop_tree_new:
 */
MbPopTree *mb_pop_tree_new ( void )
{
  MbPopTree *tree;
  tree = g_new0(MbPopTree, 1);
  return tree;
}



/* mb_pop_tree_add:
 */
void mb_pop_tree_add ( MbPopTree *tree,
                       MbPopType type,
                       guint birthdate,
                       gint64 count )
{
  tree->pop[type] += count;
}
