/* mbcsector.h -
 */

#ifndef _MBCSECTOR_H_
#define _MBCSECTOR_H_

#include "client/mbcbase.h"
#include "client/mbcsector-def.h"

G_BEGIN_DECLS



/* MbcSector:
 */
struct _MbcSector
{
  MBC_SECTOR_INSTANCE_HEADER;
};



/* MbcSectorClass:
 */
struct _MbcSectorClass
{
  MBC_SECTOR_CLASS_HEADER;
};



MbObject *mbc_sector_new ( MbObject *game,
                           MbObjectID id );



G_END_DECLS

#endif /* ifndef _MBCSECTOR_H_ */
