/* mbtkdialog.c -
 */

#include "client-al/alprivate.h"
#include "client-al/mbtkdialog.h"
#include "client-al/mbtkdialog.inl"



/* Private:
 */
typedef struct _Private
{
  MbObject *game;
  AltkWidget *frame_count_label;
}
  Private;

#define PRIVATE(dlg) ((Private *)(MBTK_DIALOG(dlg)->private))



/* mbtk_dialog_init:
 */
static void mbtk_dialog_init ( LObject *obj )
{
  MBTK_DIALOG(obj)->private = g_new0(Private, 1);
}



/* _create_dialog:
 */
static void _create_dialog ( AltkWidget *dlg )
{
  Private *priv = PRIVATE(dlg);
  AltkWidget *box, *label;
  l_trash_push();
  box = L_TRASH_OBJECT
    (altk_box_new(ALTK_HORIZONTAL));
  ALTK_CONTAINER_ADD(dlg, box);
  label = L_TRASH_OBJECT
    (altk_label_new("Time:"));
  ALTK_BOX_ADD(box, label, 0);
  priv->frame_count_label = L_TRASH_OBJECT
    (altk_label_new("0"));
  ALTK_BOX_ADD(box, priv->frame_count_label, 0);
  l_trash_pop();
}



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
  _create_dialog(dlg);
  return dlg;
}



/* _on_frame_count_notify:
 */
static void _on_frame_count_notify ( MbcGame *game,
                                     AltkWidget *dialog )
{
  gchar text[32];
  sprintf(text, "%d", MB_GAME_FRAME_COUNT(game));
  altk_label_set_text(ALTK_LABEL(PRIVATE(dialog)->frame_count_label), text);
}



/* mbtk_dialog_setup:
 */
void mbtk_dialog_setup ( MbtkDialog *dialog,
                         MbObject *game )
{
  Private *priv = PRIVATE(dialog);
  ASSERT(MBC_IS_GAME(game));
  ASSERT(!priv->game); /* [todo] reset */
  priv->game = l_object_ref(game);
  l_signal_connect(L_OBJECT(game),
                   "notify",
                   g_quark_from_string("frame_count"),
                   (LSignalHandler) _on_frame_count_notify,
                   dialog,
                   NULL);
}
