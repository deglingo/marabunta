/* mbcolony.h -
 */

#ifndef _MBCOLONY_H_
#define _MBCOLONY_H_

#include "common/mbobject.h"
#include "common/mbcolony-def.h"

G_BEGIN_DECLS



/* MbColony:
 */
struct _MbColony
{
  MB_COLONY_INSTANCE_HEADER;

  /* set by MbSector */
  MbObject *sector;
  /* owner player */
  MbObject *owner;
};



/* MbColonyClass:
 */
struct _MbColonyClass
{
  MB_COLONY_CLASS_HEADER;
};



#define MB_COLONY_SECTOR(c) (MB_COLONY(c)->sector)
#define MB_COLONY_OWNER(c) (MB_COLONY(c)->owner)

void mb_colony_set_owner ( MbColony *colony,
                           MbObject *player );



G_END_DECLS

#endif /* ifndef _MBCOLONY_H_ */
