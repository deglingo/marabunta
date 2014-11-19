/* mbccolony.h -
 */

#ifndef _MBCCOLONY_H_
#define _MBCCOLONY_H_

#include "client/mbcbase.h"
#include "client/mbccolony-def.h"

G_BEGIN_DECLS



/* MbcColony:
 */
struct _MbcColony
{
  MBC_COLONY_INSTANCE_HEADER;

  gint64 pop[MB_POP_TYPE_COUNT];
};



/* MbcColonyClass:
 */
struct _MbcColonyClass
{
  MBC_COLONY_CLASS_HEADER;
};



MbObject *mbc_colony_new ( MbObject *game,
                           MbObjectID id );
void mbc_colony_set_pop ( MbcColony *colony,
                          gint64 *pop );



G_END_DECLS

#endif /* ifndef _MBCCOLONY_H_ */
