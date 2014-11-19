/* mbpriority.h -
 */

#ifndef _MBPRIORITY_H_
#define _MBPRIORITY_H_

#include "common/mbobject.h"
#include "common/mbpriority-def.h"

G_BEGIN_DECLS



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



#define MB_PRIORITY_SCORE_FACTOR(p) (mb_priority_get_score_factor(MB_PRIORITY(p)))

void mb_priority_set_value ( MbPriority *priority,
                             MbPriorityValue value );
gint mb_priority_get_score_factor ( MbPriority *priority );



G_END_DECLS

#endif /* ifndef _MBPRIORITY_H_ */
