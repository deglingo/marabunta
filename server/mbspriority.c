/* mbspriority.c -
 */

#include "server/srvprivate.h"
#include "server/mbspriority.h"
#include "server/mbspriority.inl"



/* mbs_priority_new:
 */
MbObject *mbs_priority_new ( MbObject *game,
                             MbPriorityValue value )
{
  return MB_OBJECT(l_object_new_give(MBS_CLASS_PRIORITY,
                                     "game", l_object_ref(game),
                                     "value", l_int_new(value),
                                     NULL));
}



/* mbs_priority_update_score:
 */
void mbs_priority_update_score ( MbsPriority *priority,
                                 gint64 workers )
{
  /* score <- score + workers / priority.value */
  priority->score.score += (workers + priority->score.remainder) / MB_PRIORITY_VALUE(priority);
  priority->score.remainder = (workers + priority->score.remainder) % MB_PRIORITY_VALUE(priority);
}



/* mbs_priority_adjust_score:
 */
void mbs_priority_adjust_score ( MbsPriority *priority,
                                 gint64 score )
{
  priority->score.score += score;
  ASSERT(priority->score.score >= 0);
}



/* mbs_priority_next_score:
 */
gint64 mbs_priority_next_score ( MbsPriority *priority,
                                 gint64 workers )
{
  return (workers + priority->score.remainder) / MB_PRIORITY_VALUE(priority);
}
