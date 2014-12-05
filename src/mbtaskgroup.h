/* mbtaskgroup.h -
 */

#ifndef _MBTASKGROUP_H_
#define _MBTASKGROUP_H_

#include "mbtask.h"
#include "mbtaskgroup-def.h"

G_BEGIN_DECLS



/* MbTaskGroup:
 */
struct _MbTaskGroup
{
  MB_TASK_GROUP_INSTANCE_HEADER;

  GList *children;
};



/* MbTaskGroupClass:
 */
struct _MbTaskGroupClass
{
  MB_TASK_GROUP_CLASS_HEADER;
};



MbTask *mb_task_group_new_top ( struct _MbColony *colony,
                                const gchar *name );



G_END_DECLS

#endif /* ifndef _MBTASKGROUP_H_ */
