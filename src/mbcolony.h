/* mbcolony.h -
 */

#ifndef _MBCOLONY_H_
#define _MBCOLONY_H_

#include "mbbase.h"
#include "mbcolony-def.h"

G_BEGIN_DECLS



/* MbColony:
 */
struct _MbColony
{
  MB_COLONY_INSTANCE_HEADER;

  struct _MbSector *sector;
};



/* MbColonyClass:
 */
struct _MbColonyClass
{
  MB_COLONY_CLASS_HEADER;
};



MbColony *mb_colony_new ( void );
void mb_colony_update ( MbColony *colony );



G_END_DECLS

#endif /* ifndef _MBCOLONY_H_ */
