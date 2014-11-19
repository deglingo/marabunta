/* mbssector.c -
 */

#include "server/mbssector.h"
#include "server/mbscolony.h"
#include "server/mbssector.inl"



/* mbs_sector_new:
 */
MbObject *mbs_sector_new ( MbObject *game )
{
  return MB_OBJECT(l_object_new(MB_CLASS_SECTOR, NULL));
}



/* mbs_sector_update:
 */
void mbs_sector_update ( MbsSector *sector )
{
  if (MB_SECTOR_COLONY(sector))
    mbs_colony_update(MBS_COLONY(MB_SECTOR_COLONY(sector)));
}
