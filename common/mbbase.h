/* mbbase.h -
 */

#ifndef _MBBASE_H_
#define _MBBASE_H_

#include <los.h>



/* [fixme] */
#define MBS_TASK_MAX_NAME 16
#define MB_RESOURCE_MAX_NAME 16

/* [fixme] */
typedef gint MbPriorityValue;

gchar *mb_task_path_next ( const gchar **path,
                           gchar *dest );



#endif
