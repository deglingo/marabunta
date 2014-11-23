/* mbalmain.c -
 */

#include "client-al/alprivate.h"
#include "client-al/mbalapp.h"



/* main:
 */
int main ( gint argc,
           gchar **argv )
{
  MbApp *app;
  gint r;
  /* [fixme] */
  l_signal_debug = TRUE;
  altk_init();
  app = mbal_app_new();
  r = mb_app_main(app, argc, argv);
  l_object_unref(app);
  return r;
}
