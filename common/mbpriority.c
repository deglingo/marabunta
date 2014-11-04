/* mbpriority.c -
 */

#include "common/mbpriority.h"
#include "common/mbpriority.inl"



/* mb_priority_new:
 */
MbPriority *mb_priority_new ( gint level )
{
  MbPriority *prio;
  prio = MB_PRIORITY_NEW(NULL);
  prio->level = level;
  return prio;
}
