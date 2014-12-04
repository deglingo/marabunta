/* mbsector.c -
 */

#include "mbsector.h"
#include "mbsector.inl"



/* mb_sector_new:
 */
MbSector *mb_sector_new ( void )
{
  return MB_SECTOR(l_object_new(MB_CLASS_SECTOR, NULL));
}
