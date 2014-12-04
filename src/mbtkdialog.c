/* mbtkdialog.c -
 */

#include "mbtkdialog.h"
#include "mbgame.h"
#include "mbtkdialog.inl"



/* Private:
 */
typedef struct _Private
{
  LSignalHandlerGroup *sig_group;
  MbGame *game;
  AltkWidget *time_label;
}
  Private;

#define PRIVATE(dlg) ((Private *)(MBTK_DIALOG(dlg)->private))



/* mbtk_dialog_init:
 */
static void mbtk_dialog_init ( LObject *obj )
{
  MBTK_DIALOG(obj)->private = g_new0(Private, 1);
  PRIVATE(obj)->sig_group = l_signal_handler_group_new();
}



/* _create_header:
 */
static AltkWidget *_create_header ( AltkWidget *dlg )
{
  Private *priv = PRIVATE(dlg);
  AltkWidget *box, *time;
  box = altk_box_new(ALTK_HORIZONTAL);
  time = L_TRASH_OBJECT
    (altk_label_new("Time:"));
  ALTK_BOX_ADD(box, time, 0);
  priv->time_label = L_TRASH_OBJECT
    (altk_label_new(""));
  ALTK_BOX_ADD(box, priv->time_label, 0);
  return box;
}



/* _create_dialog:
 */
static void _create_dialog ( AltkWidget *dlg )
{
  /* Private *priv = PRIVATE(dlg); */
  AltkWidget *top_box;
  /* top box */
  top_box = L_TRASH_OBJECT
    (altk_box_new(ALTK_VERTICAL));
  ALTK_CONTAINER_ADD(dlg, top_box);
  /* header */
  ALTK_BOX_ADD(top_box,
               L_TRASH_OBJECT(_create_header(dlg)),
               ALTK_PACK_HEXPAND_FILL);
  /* body */
  /* ALTK_BOX_ADD(top_box, */
  /*              L_TRASH_OBJECT(_create_body(dlg)), */
  /*              ALTK_PACK_HEXPAND_FILL); */
}



/* mbtk_dialog_new:
 */
AltkWidget *mbtk_dialog_new ( AltkDisplay *display )
{
  AltkWidget *dlg = ALTK_WIDGET(l_object_new(MBTK_CLASS_DIALOG,
                                             NULL));
  altk_dialog_set_display(ALTK_DIALOG(dlg), display);
  l_trash_push();
  _create_dialog(dlg);
  l_trash_pop();
  return dlg;
}



/* _on_sim_time_notify:
 */
static void _on_sim_time_notify ( MbGame *game,
                                  MbtkDialog *dialog )
{
  Private *priv = PRIVATE(dialog);
  gchar text[16];
  sprintf(text, "%d", game->sim_time);
  altk_label_set_text(ALTK_LABEL(priv->time_label), text);
}



/* mbtk_dialog_set_game:
 */
void mbtk_dialog_set_game ( MbtkDialog *dialog,
                            struct _MbGame *game )
{
  Private *priv = PRIVATE(dialog);
  ASSERT(!priv->game); /* [todo] */
  priv->game = l_object_ref(game);
  l_signal_handler_group_add
    (priv->sig_group,
     l_signal_connect(L_OBJECT(game),
                      "notify:sim_time",
                      (LSignalHandler) _on_sim_time_notify,
                      dialog,
                      NULL));
}
