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
};



/* MbSectorClass:
 */
struct _MbSectorClass
{
  MB_SECTOR_CLASS_HEADER;
};



MbSector *mb_sector_new ( void );



G_END_DECLS

#endif /* ifndef _MBSECTOR_H_ */
