/* mbtkdialog.c -
 */

#include "mbtkdialog.h"
#include "mbtkpoptable.h"
#include "mbtksectorview.h"
#include "mbtkinfopanel.h"
#include "mbtkdialog.inl"



/* Private:
 */
typedef struct _Private
{
  LSignalHandlerGroup *sig_group;
  MbGame *game;
  MbSector *sector;
  AltkWidget *time_label;
  AltkWidget *pop_table;
  AltkWidget *info_panel;
  AltkWidget *sector_view;
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



/* _create_side_panel:
 */
static AltkWidget *_create_side_panel ( AltkWidget *dlg )
{
  Private *priv = PRIVATE(dlg);
  AltkWidget *box;
  box = altk_box_new(ALTK_VERTICAL);
  priv->pop_table = L_TRASH_OBJECT
    (mbtk_pop_table_new());
  ALTK_BOX_ADD(box, priv->pop_table, 0);
  priv->info_panel = L_TRASH_OBJECT
    (mbtk_info_panel_new());
  ALTK_BOX_ADD(box, priv->info_panel, ALTK_PACK_EXPAND_FILL);
  return box;
}



/* _create_sector_view:
 */
static AltkWidget *_create_sector_view ( AltkWidget *dlg )
{
  Private *priv = PRIVATE(dlg);
  AltkWidget *frame;
  frame = altk_frame_new("");
  priv->sector_view = L_TRASH_OBJECT
    (mbtk_sector_view_new());
  ALTK_CONTAINER_ADD(frame, priv->sector_view);
  return frame;
}



/* _create_body:
 */
static AltkWidget *_create_body ( AltkWidget *dlg )
{
  /* Private *priv = PRIVATE(dlg); */
  AltkWidget *box;
  box = altk_box_new(ALTK_HORIZONTAL);
  ALTK_BOX_ADD(box,
               L_TRASH_OBJECT(_create_side_panel(dlg)),
               ALTK_PACK_VEXPAND_FILL);
  ALTK_BOX_ADD(box,
               L_TRASH_OBJECT(_create_sector_view(dlg)),
               ALTK_PACK_EXPAND_FILL);
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
  ALTK_BOX_ADD(top_box,
               L_TRASH_OBJECT(_create_body(dlg)),
               ALTK_PACK_EXPAND_FILL);
}



/* mbtk_dialog_new:
 */
AltkWidget *mbtk_dialog_new ( AltkDisplay *display )
{
  AltkWidget *dlg = ALTK_WIDGET(l_object_new(MBTK_CLASS_DIALOG,
                                             NULL));
  altk_dialog_set_display(ALTK_DIALOG(dlg), display);
  altk_wm_set_top_widget_size_hints(dlg, ALTK_SIZE_HINT_MAXIMIZED);
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
  mbtk_dialog_set_sector(dialog, game->world->sectors[0][0]);
}



/* mbtk_dialog_set_sector:
 */
void mbtk_dialog_set_sector ( MbtkDialog *dialog,
                              struct _MbSector *sector )
{
  Private *priv = PRIVATE(dialog);
  ASSERT(!priv->sector); /* [todo] */
  priv->sector = l_object_ref(sector);
  mbtk_pop_table_set_colony(MBTK_POP_TABLE(priv->pop_table),
                            sector->colony);
  mbtk_sector_view_set_sector(MBTK_SECTOR_VIEW(priv->sector_view),
                              sector);
  mbtk_info_panel_set_sector(MBTK_INFO_PANEL(priv->info_panel),
                             sector);
}
