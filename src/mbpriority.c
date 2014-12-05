/* mbpriority.c -
 */

#include "mbpriority.h"
#include "mbpriority.inl"



/* mb_priority_new:
 */
MbPriority *mb_priority_new ( MbPriorityValue value )
{
  MbPriority *prio;
  prio = MB_PRIORITY(l_object_new(MB_CLASS_PRIORITY, NULL));
  prio->value = value;
  return prio;
}



/* mb_priority_score:
 */
gint64 mb_priority_score ( MbPriority *priority )
{
  return priority->score;
}



/* mb_priority_update_score:
 */
void mb_priority_update_score ( MbPriority *priority,
                                gint64 workers )
{
  priority->score += (workers + priority->remainder) / priority->value;
  priority->remainder = (workers + priority->remainder) % priority->value;
}



/* mb_priority_adjust_score:
 */
void mb_priority_adjust_score ( MbPriority *priority,
                                gint64 adj )
{
  priority->score += adj;
}
