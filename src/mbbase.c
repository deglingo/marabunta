/* mbbase.c -
 */

#include "mbbase.h"



/* mb_count_print:
 */
gchar *mb_count_print ( gint64 count,
                        gchar *dest )
{
  ASSERT(count >= 0); /* [TODO] */
  if (count < 1000)
    sprintf(dest, "%dU", (gint) count);
  else if (count < 1000000)
    sprintf(dest, "%.2fK", ((gdouble) count) / 1000.0);
  else
    CL_ERROR("[TODO] %" G_GINT64_FORMAT, count);
  return dest;
}
