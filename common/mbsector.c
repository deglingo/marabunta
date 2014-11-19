/* mbsector.c -
 */

#include "common/private.h"
#include "common/mbsector.h"
#include "common/mbcolony.h"
#include "common/mbsector.inl"



/* mb_sector_set_colony:
 */
void mb_sector_set_colony ( MbSector *sector,
                            MbObject *colony )
{
  ASSERT(MB_IS_COLONY(colony));
  ASSERT(!sector->colony);
  sector->colony = l_object_ref(colony);
}
