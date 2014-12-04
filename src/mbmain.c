/* mbmain.c -
 */

#include "mbbase.h"



/* main:
 */
gint main ()
{
  AltkWidget *dlg;
  AltkDisplay *display;
  altk_init();
  CL_DEBUG("hello!");
  display = altk_display_new();
  dlg = altk_dialog_new(display);
  altk_display_open(display);
  altk_widget_show_all(dlg);
  altk_main();
  return 0;
}
