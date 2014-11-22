/* mbtkdialog.c -
 */

#include "client-al/alprivate.h"
#include "client-al/mbtkdialog.h"
#include "client-al/mbtkmapview.h"
#include "client-al/mbtkpoptable.h"
#include "client-al/mbtkcolonyview.h"
#include "client-al/mbtkinfopanel.h"
#include "client-al/mbtkdialog.inl"



/* Private:
 */
typedef struct _Private
{
  MbObject *game;
  MbObject *sector;
  AltkWidget *frame_count_label;
  AltkWidget *map_view;
  AltkWidget *pop_table;
  AltkWidget *colony_view;
  AltkWidget *info_panel;
}
  Private;

#define PRIVATE(dlg) ((Private *)(MBTK_DIALOG(dlg)->private))



/* mbtk_dialog_init:
 */
static void mbtk_dialog_init ( LObject *obj )
{
  MBTK_DIALOG(obj)->private = g_new0(Private, 1);
}



/* _create_header:
 */
static AltkWidget *_create_header ( AltkWidget *dlg )
{
  Private *priv = PRIVATE(dlg);
  AltkWidget *box, *label;
  box = altk_box_new(ALTK_HORIZONTAL);
  label = L_TRASH_OBJECT
    (altk_label_new("Time:"));
  ALTK_BOX_ADD(box, label, 0);
  priv->frame_count_label = L_TRASH_OBJECT
    (altk_label_new("0"));
  ALTK_BOX_ADD(box, priv->frame_count_label, 0);
  return box;
}



/* _create_side_panel:
 */
static AltkWidget *_create_side_panel ( AltkWidget *dlg )
{
  Private *priv = PRIVATE(dlg);
  AltkWidget *frame, *box;
  frame = altk_frame_new("");
  /* top box */
  box = L_TRASH_OBJECT
    (altk_box_new(ALTK_VERTICAL));
  ALTK_CONTAINER_ADD(frame, box);
  /* map view */
  priv->map_view = L_TRASH_OBJECT(mbtk_map_view_new());
  ALTK_BOX_ADD(box,
               priv->map_view,
               ALTK_PACK_ANCHOR_LEFT);
  /* pop table */
  priv->pop_table = L_TRASH_OBJECT(mbtk_pop_table_new());
  ALTK_BOX_ADD(box,
               priv->pop_table,
               ALTK_PACK_FILL);
  /* info panel */
  priv->info_panel = L_TRASH_OBJECT
    (mbtk_info_panel_new());
  ALTK_BOX_ADD(box,
               priv->info_panel, ALTK_PACK_EXPAND_FILL);
  return frame;
}



/* _create_body:
 */
static AltkWidget *_create_body ( AltkWidget *dlg )
{
  Private *priv = PRIVATE(dlg);
  AltkWidget *box, *frame;
  /* top box */
  box = altk_box_new(ALTK_HORIZONTAL);
  /* side panel */
  ALTK_BOX_ADD(box,
               L_TRASH_OBJECT(_create_side_panel(dlg)),
               ALTK_PACK_VEXPAND_FILL);
  /* colony view */
  frame = L_TRASH_OBJECT
    (altk_frame_new(""));
  ALTK_BOX_ADD(box, frame, ALTK_PACK_EXPAND_FILL);
  priv->colony_view = L_TRASH_OBJECT
    (mbtk_colony_view_new());
  ALTK_CONTAINER_ADD(frame, priv->colony_view);
  return box;
}



/* _create_dialog:
 */
static void _create_dialog ( AltkWidget *dlg )
{
  /* Private *priv = PRIVATE(dlg); */
  AltkWidget *box;
  /* top box */
  box = L_TRASH_OBJECT
    (altk_box_new(ALTK_VERTICAL));
  ALTK_CONTAINER_ADD(dlg, box);
  /* header */
  ALTK_BOX_ADD(box,
               L_TRASH_OBJECT(_create_header(dlg)),
               ALTK_PACK_HEXPAND);
  /* body */
  ALTK_BOX_ADD(box,
               L_TRASH_OBJECT(_create_body(dlg)),
               ALTK_PACK_EXPAND_FILL);
}



/* mbtk_dialog_new:
 */
AltkWidget *mbtk_dialog_new ( AltkDisplay *display )
{
  AltkWidget *dlg;
  dlg = ALTK_WIDGET(l_object_new(MBTK_CLASS_DIALOG, NULL));
  altk_dialog_set_display(ALTK_DIALOG(dlg), display);
  altk_dialog_set_size_hints(ALTK_DIALOG(dlg), ALTK_SIZE_HINT_MAXIMIZED);
  /* create the dialog */
  l_trash_push();
  _create_dialog(dlg);
  l_trash_pop();
  return dlg;
}



/* _on_frame_count_notify:
 */
static void _on_frame_count_notify ( MbGame *game,
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
  ASSERT(MB_IS_GAME(game));
  ASSERT(!priv->game); /* [todo] reset */
  priv->game = l_object_ref(game);
  l_signal_connect(L_OBJECT(game),
                   "notify:frame_count",
                   (LSignalHandler) _on_frame_count_notify,
                   dialog,
                   NULL);
  mbtk_map_view_set_world(MBTK_MAP_VIEW(priv->map_view),
                          MB_GAME_WORLD(game));
  mbtk_dialog_set_sector(dialog, 0, 0);
}



/* mbtk_dialog_set_sector:
 */
void mbtk_dialog_set_sector ( MbtkDialog *dialog,
                              guint x,
                              guint y )
{
  Private *priv = PRIVATE(dialog);
  MbObject *sector;
  ASSERT(x < MB_WORLD_WIDTH(MB_GAME_WORLD(priv->game)));
  ASSERT(y < MB_WORLD_HEIGHT(MB_GAME_WORLD(priv->game)));
  ASSERT(!priv->sector); /* [todo] */
  sector = MB_WORLD_SECTOR(MB_GAME_WORLD(priv->game), x, y);
  priv->sector = l_object_ref(sector);
  if (MB_SECTOR_COLONY(sector))
    {
      mbtk_pop_table_set_colony(MBTK_POP_TABLE(priv->pop_table),
                                MB_SECTOR_COLONY(sector));
      mbtk_colony_view_set_colony(MBTK_COLONY_VIEW(priv->colony_view),
                                  MB_SECTOR_COLONY(sector));
      mbtk_info_panel_set_colony(MBTK_INFO_PANEL(priv->info_panel),
                                 MB_SECTOR_COLONY(sector));
    }
  else
    {
      mbtk_pop_table_set_colony(MBTK_POP_TABLE(priv->pop_table),
                                NULL);
      mbtk_colony_view_set_colony(MBTK_COLONY_VIEW(priv->colony_view),
                                  NULL);
      mbtk_info_panel_set_colony(MBTK_INFO_PANEL(priv->info_panel),
                                 NULL);
    }
}
