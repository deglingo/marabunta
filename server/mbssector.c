/* mbssector.c -
 */

#include "server/srvprivate.h"
#include "server/mbssector.h"
#include "server/mbssector.inl"



/* mbs_sector_new:
 */
MbsSector *mbs_sector_new ( void )
{
  MbsSector *sector;
  sector = MBS_SECTOR_NEW(NULL);
  return sector;
}



/* mbs_sector_create_colony:
 */
void mbs_sector_create_colony ( MbsSector *sector,
                                guint player )
{
  ASSERT(!sector->colony);
  sector->colony = mbs_colony_new(player);
}
