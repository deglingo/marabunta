/* mbssector.c -
 */

#include "server/mbssector.h"
#include "server/mbssector.inl"



/* mbs_sector_new:
 */
MbObject *mbs_sector_new ( MbObject *game )
{
  return MB_OBJECT(l_object_new(MB_CLASS_SECTOR, NULL));
}
