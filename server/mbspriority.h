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

  MbScore score;
};



/* MbsPriorityClass:
 */
struct _MbsPriorityClass
{
  MBS_PRIORITY_CLASS_HEADER;
};



#define MBS_PRIORITY_SCORE(prio) (MBS_PRIORITY(prio)->score.score)

MbObject *mbs_priority_new ( MbObject *game,
                             MbPriorityValue value );
void mbs_priority_update_score ( MbsPriority *priority,
                                 gint64 workers );
void mbs_priority_adjust_score ( MbsPriority *priority,
                                 gint64 score );
gint64 mbs_priority_next_score ( MbsPriority *priority,
                                 gint64 workers );



G_END_DECLS

#endif /* ifndef _MBSPRIORITY_H_ */
