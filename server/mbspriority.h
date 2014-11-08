/* mbspriority.h -
 */

#ifndef _MBSPRIORITY_H_
#define _MBSPRIORITY_H_

#include "server/mbscontrol.h"
#include "server/mbspriority-def.h"

G_BEGIN_DECLS

struct _MbsGame;



/* MbsPriority:
 */
struct _MbsPriority
{
  MBS_PRIORITY_INSTANCE_HEADER;

  struct _MbsGame *game;
  MbPriorityValue value;
};



/* MbsPriorityClass:
 */
struct _MbsPriorityClass
{
  MBS_PRIORITY_CLASS_HEADER;
};



MbsPriority *mbs_priority_new ( struct _MbsGame *game,
                                MbPriorityValue value );
void mbs_priority_set_value ( MbsPriority *priority,
                              MbPriorityValue value );
gint64 mbs_priority_get_score_factor ( MbsPriority *priority );



G_END_DECLS

#endif /* ifndef _MBSPRIORITY_H_ */
