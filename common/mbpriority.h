/* mbpriority.h -
 */

#ifndef _MBPRIORITY_H_
#define _MBPRIORITY_H_

#include "common/mbbase.h"
#include "common/mbpriority-def.h"

G_BEGIN_DECLS



/* MbPriority:
 */
struct _MbPriority
{
  MB_PRIORITY_INSTANCE_HEADER;
};



/* MbPriorityClass:
 */
struct _MbPriorityClass
{
  MB_PRIORITY_CLASS_HEADER;
};



G_END_DECLS

#endif /* ifndef _MBPRIORITY_H_ */
