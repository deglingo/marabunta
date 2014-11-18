/* mbtkdialog.c -
 */

#include "client-al/alprivate.h"
#include "client-al/mbtkdialog.h"
#include "client-al/mbtkdialog.inl"



/* Private:
 */
typedef struct _Private
{
  int dummy;
}
  Private;

#define PRIVATE(dlg) ((Private *)(MBTK_DIALOG(dlg)->private))



/* mbtk_dialog_new:
 */
AltkWidget *mbtk_dialog_new ( AltkDisplay *display )
{
  AltkWidget *dlg = ALTK_WIDGET(l_object_new(MBTK_CLASS_DIALOG, NULL));
  /* Private *priv; */
  /* [fixme] instance init */
  /* MBTK_DIALOG(dlg)->private = priv = g_new0(Private, 1); */
  altk_dialog_set_display(ALTK_DIALOG(dlg), display);
  altk_dialog_set_size_hints(ALTK_DIALOG(dlg), ALTK_SIZE_HINT_MAXIMIZED);
  return dlg;
}
