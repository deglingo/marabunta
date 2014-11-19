/* mbpriority.h -
 */

#ifndef _MBPRIORITY_H_
#define _MBPRIORITY_H_

#include "common/mbobject.h"
#include "common/mbpriority-def.h"

G_BEGIN_DECLS



/* MbScore:
 */
typedef struct _MbScore
{
  gint64 score;
  gint64 remainder;
}
  MbScore;



/* MbPriority:
 */
struct _MbPriority
{
  MB_PRIORITY_INSTANCE_HEADER;

  MbPriorityValue value;
  MbScore score;
};



/* MbPriorityClass:
 */
struct _MbPriorityClass
{
  MB_PRIORITY_CLASS_HEADER;
};



#define MB_PRIORITY_SCORE(p) (MB_PRIORITY(p)->score.score)

void mb_priority_set_value ( MbPriority *priority,
                             MbPriorityValue value );
void mb_priority_adjust_score ( MbPriority *priority,
                                gint64 workers );



G_END_DECLS

#endif /* ifndef _MBPRIORITY_H_ */
