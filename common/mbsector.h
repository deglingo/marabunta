/* mbsector.h -
 */

#ifndef _MBSECTOR_H_
#define _MBSECTOR_H_

#include "common/mbobject.h"
#include "common/mbsector-def.h"

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



G_END_DECLS

#endif /* ifndef _MBSECTOR_H_ */
