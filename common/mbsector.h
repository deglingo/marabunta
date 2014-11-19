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

  /* set by MbWorld */
  MbObject *world;
  guint x;
  guint y;
};



/* MbSectorClass:
 */
struct _MbSectorClass
{
  MB_SECTOR_CLASS_HEADER;
};



#define MB_SECTOR_X(s) (MB_SECTOR(s)->x)
#define MB_SECTOR_Y(s) (MB_SECTOR(s)->y)



G_END_DECLS

#endif /* ifndef _MBSECTOR_H_ */