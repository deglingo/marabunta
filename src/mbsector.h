/* mbsector.h -
 */

#ifndef _MBSECTOR_H_
#define _MBSECTOR_H_

#include "mbbase.h"
#include "mbsector-def.h"

G_BEGIN_DECLS



/* MbSector:
 */
struct _MbSector
{
  MB_SECTOR_INSTANCE_HEADER;

  struct _MbColony *colony;
};



/* MbSectorClass:
 */
struct _MbSectorClass
{
  MB_SECTOR_CLASS_HEADER;
};



MbSector *mb_sector_new ( void );
void mb_sector_set_colony ( MbSector *sector,
                            struct _MbColony *colony );
void mb_sector_update ( MbSector *sector );



G_END_DECLS

#endif /* ifndef _MBSECTOR_H_ */
