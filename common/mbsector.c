/* mbsector.c -
 */

#include "common/private.h"
#include "common/mbsector.h"
#include "common/mbcolony.h"
#include "common/mbvein.h"
#include "common/mbsector.inl"



/* mb_sector_set_colony:
 */
void mb_sector_set_colony ( MbSector *sector,
                            MbObject *colony )
{
  ASSERT(MB_IS_COLONY(colony));
  ASSERT(!sector->colony);
  ASSERT(!MB_COLONY_SECTOR(colony));
  sector->colony = l_object_ref(colony);
  MB_COLONY(colony)->sector = MB_OBJECT(sector);
}



/* mb_sector_add_vein:
 */
void mb_sector_add_vein ( MbSector *sector,
                          MbObject *vein )
{
  ASSERT(MB_IS_VEIN(vein));
  ASSERT(!MB_VEIN_SECTOR(vein));
  l_object_ref(vein);
  sector->veins = g_list_append(sector->veins, vein);
  MB_VEIN(vein)->sector = MB_OBJECT(sector);
}
