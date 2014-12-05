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
