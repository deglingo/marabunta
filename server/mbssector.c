/* mbssector.c -
 */

#include "server/srvprivate.h"
#include "server/mbssector.h"
#include "server/mbssector.inl"



/* mbs_sector_new:
 */
MbsSector *mbs_sector_new ( guint x,
                            guint y )
{
  MbsSector *sector;
  sector = MBS_SECTOR_NEW(NULL);
  sector->x = x;
  sector->y = y;
  return sector;
}



/* mbs_sector_create_colony:
 */
void mbs_sector_create_colony ( MbsSector *sector,
                                guint player )
{
  ASSERT(!sector->colony);
  sector->colony = mbs_colony_new(sector, player);
}
