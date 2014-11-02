/* mbscolony.c -
 */

#include "server/mbscolony.h"
#include "server/mbssector.h"
#include "server/mbscolony.inl"



/* mbs_colony_new:
 */
MbsColony *mbs_colony_new ( MbsSector *sector,
                            guint owner )
{
  MbsColony *col = MBS_COLONY_NEW(NULL);
  col->sector = sector;
  col->owner = owner;
  col->pop_tree = mb_pop_tree_new();
  return col;
}
