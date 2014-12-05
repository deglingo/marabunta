/* mbpriority.h -
 */

#ifndef _MBPRIORITY_H_
#define _MBPRIORITY_H_

#include "mbbase.h"
#include "mbpriority-def.h"

G_BEGIN_DECLS



typedef guint MbPriorityValue;



/* MbPriority:
 */
struct _MbPriority
{
  MB_PRIORITY_INSTANCE_HEADER;

  MbPriorityValue value;
};



/* MbPriorityClass:
 */
struct _MbPriorityClass
{
  MB_PRIORITY_CLASS_HEADER;
};



MbPriority *mb_priority_new ( MbPriorityValue value );



G_END_DECLS

#endif /* ifndef _MBPRIORITY_H_ */
