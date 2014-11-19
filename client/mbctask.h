/* mbctask.h -
 */

#ifndef _MBCTASK_H_
#define _MBCTASK_H_

#include "client/mbcbase.h"
#include "client/mbctask-def.h"

G_BEGIN_DECLS



/* MbcTask:
 */
struct _MbcTask
{
  MBC_TASK_INSTANCE_HEADER;
};



/* MbcTaskClass:
 */
struct _MbcTaskClass
{
  MBC_TASK_CLASS_HEADER;
};



MbObject *mbc_task_new_group ( MbObject *game,
                               MbObjectID id,
                               const gchar *name );
MbObject *mbc_task_new ( MbObject *game,
                         MbObjectID id,
                         const gchar *name );



G_END_DECLS

#endif /* ifndef _MBCTASK_H_ */
