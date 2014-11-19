/* mbbase.h -
 */

#include "common/private.h"
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



/* mb_count_print:
 */
gchar *mb_count_print ( gint64 count,
                        gchar *dest )
{
  ASSERT(count >= 0); /* [todo] ? */
  if (count < 1000)
    sprintf(dest, "%dU", (gint) count);
  else if (count < 1000000)
    sprintf(dest, "%.2fK", (gdouble) count / (gdouble) 1000.0);
  else {
    ASSERT(count < 1000000000);
    sprintf(dest, "%.2fM", (gdouble) count / (gdouble) 1000000.0);
  }
  return dest;
}
