/* mbspriority.h -
 */

#ifndef _MBSPRIORITY_H_
#define _MBSPRIORITY_H_

#include "server/mbsbase.h"
#include "server/mbspriority-def.h"

G_BEGIN_DECLS



/* MbsPriority:
 */
struct _MbsPriority
{
  MBS_PRIORITY_INSTANCE_HEADER;
};



/* MbsPriorityClass:
 */
struct _MbsPriorityClass
{
  MBS_PRIORITY_CLASS_HEADER;
};



MbObject *mbs_priority_new ( MbObject *game,
                             MbPriorityValue value );



G_END_DECLS

#endif /* ifndef _MBSPRIORITY_H_ */