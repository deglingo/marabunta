/* mbspriority.c -
 */

#include "server/mbspriority.h"
#include "server/mbspriority.inl"



/* mbs_priority_new:
 */
MbsPriority *mbs_priority_new ( MbPriorityValue value )
{
  MbsPriority *prio = MBS_PRIORITY_NEW(NULL);
  prio->value = value;
  return prio;
}
