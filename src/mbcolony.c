/* mbcolony.c -
 */

#include "mbcolony.h"
#include "mbpoptree.h"
#include "mbcolony.inl"



/* Signals:
 */
enum
  {
    SIG_POP_NOTIFY,
    SIG_COUNT,
  };

static LSignalID signals[SIG_COUNT];



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
  return col;
}



/* mb_colony_update:
 */
void mb_colony_update ( MbColony *colony )
{
  CL_DEBUG("[TODO] colony update");
}
