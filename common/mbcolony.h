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

  MbObject *owner;
};



/* MbColonyClass:
 */
struct _MbColonyClass
{
  MB_COLONY_CLASS_HEADER;
};



void mb_colony_set_owner ( MbColony *colony,
                           MbObject *player );



G_END_DECLS

#endif /* ifndef _MBCOLONY_H_ */
