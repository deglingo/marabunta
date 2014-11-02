/* mbpoptree.c -
 */

#include "common/mbpoptree.h"



/* mb_pop_tree_new:
 */
MbPopTree *mb_pop_tree_new ( void )
{
  MbPopTree *tree;
  tree = g_new0(MbPopTree, 1);
  return tree;
}
