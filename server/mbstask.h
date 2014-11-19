/* mbstask.h -
 */

#ifndef _MBSTASK_H_
#define _MBSTASK_H_

#include "server/mbsbase.h"
#include "server/mbstask-def.h"

G_BEGIN_DECLS



/* MbsTask:
 */
struct _MbsTask
{
  MBS_TASK_INSTANCE_HEADER;
};



/* MbsTaskClass:
 */
struct _MbsTaskClass
{
  MBS_TASK_CLASS_HEADER;
};



G_END_DECLS

#endif /* ifndef _MBSTASK_H_ */
