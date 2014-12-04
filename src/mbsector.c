/* mbsector.c -
 */

#include "mbsector.h"
#include "mbcolony.h"
#include "mbsector.inl"



/* mb_sector_new:
 */
MbSector *mb_sector_new ( void )
{
  return MB_SECTOR(l_object_new(MB_CLASS_SECTOR, NULL));
}



/* mb_sector_set_colony:
 */
void mb_sector_set_colony ( MbSector *sector,
                            struct _MbColony *colony )
{
  ASSERT(!sector->colony);
  ASSERT(!colony->sector);
  sector->colony = l_object_ref(colony);
  colony->sector = sector;
}
