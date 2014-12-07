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
  gint64 score;
  gint64 remainder;
};



/* MbPriorityClass:
 */
struct _MbPriorityClass
{
  MB_PRIORITY_CLASS_HEADER;
};



MbPriority *mb_priority_new ( MbPriorityValue value );
gint64 mb_priority_score ( MbPriority *priority );
gint64 mb_priority_next_score ( MbPriority *priority,
                                gint64 workers );
gint64 mb_priority_update_score ( MbPriority *priority,
                                  gint64 workers );
void mb_priority_adjust_score ( MbPriority *priority,
                                gint64 adj );
void mb_priority_request_set_value ( MbPriority *priority,
                                     MbPriorityValue value );



G_END_DECLS

#endif /* ifndef _MBPRIORITY_H_ */
