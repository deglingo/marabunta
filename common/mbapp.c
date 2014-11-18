/* mbapp.c -
 */

#include "common/mbapp.h"
#include "common/mbapp.inl"



/* mb_app_main:
 */
gint mb_app_main ( MbApp *app,
                   gint argc,
                   gchar **argv )
{
  gint r;
  r = MB_APP_GET_CLASS(app)->run(app);
  return r;
}
