/* mbtkdialog.c -
 */

#include "mbtkdialog.h"
#include "mbtkdialog.inl"



/* mbtk_dialog_new:
 */
AltkWidget *mbtk_dialog_new ( AltkDisplay *display )
{
  AltkWidget *dlg = ALTK_WIDGET(l_object_new(ALTK_CLASS_DIALOG,
                                             NULL));
  altk_dialog_set_display(ALTK_DIALOG(dlg), display);
  return dlg;
}
