/* mbbase.h -
 */

#include "common/mbbase.h"

#include <string.h>



/* mb_path_task_next:
 */
gchar *mb_task_path_next ( const gchar **path,
                           gchar *dest )
{
  const gchar *p = *path;
  const gchar *f;
  if (!p[0])
    return NULL;
  if ((f = strchr(p, '/')))
    {
      gint len = f - p;
      memcpy(dest, p, len);
      dest[len] = 0;
      *path = f + 1;
    }
  else
    {
      gint len = strlen(p);
      memcpy(dest, p, len);
      dest[len] = 0;
      *path = p + len;
    }
  return dest;
}
