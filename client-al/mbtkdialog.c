/* mbtkdialog.c -
 */

#include "client-al/alprivate.h"
#include "client-al/mbtkdialog.h"
#include "client-al/mbtkmapview.h"
#include "client-al/mbtkpoptable.h"
#include "client-al/mbtkcolonyview.h"
#include "client-al/mbtkdialog.inl"



/* Private:
 */
typedef struct _Private
{
  MbcProxy *game;
  MbcProxy *sector;
  AltkWidget *top_box;
  AltkWidget *sim_time_label;
  AltkWidget *map_view;
  AltkWidget *pop_table;
  AltkWidget *colony_view;
}
  Private;

#define PRIVATE(dlg) ((Private *)(MBTK_DIALOG(dlg)->private))



/* _create_header:
 */
static AltkWidget *_create_header ( AltkWidget *dlg )
{
  Private *priv = PRIVATE(dlg);
  AltkWidget *box, *lbl;
  box = altk_box_new(ALTK_HORIZONTAL);
  lbl = L_TRASH_OBJECT(altk_label_new("Time:"));
  altk_box_pack_start(ALTK_BOX(box), lbl, 0);
  priv->sim_time_label = lbl = L_TRASH_OBJECT(altk_label_new("0"));
  altk_box_pack_start(ALTK_BOX(box), lbl, 0);
  return box;
}



/* _create_map_view:
 */
static AltkWidget *_create_map_view ( AltkWidget *dlg )
{
  Private *priv = PRIVATE(dlg);
  AltkWidget *frame, *view;
  frame = altk_frame_new("map");
  priv->map_view = view = L_TRASH_OBJECT
    (mbtk_map_view_new());
  ALTK_CONTAINER_ADD(frame, view);
  return frame;
}



/* _create_pop_table:
 */
static AltkWidget *_create_pop_table ( AltkWidget *dlg )
{
  Private *priv = PRIVATE(dlg);
  priv->pop_table = mbtk_pop_table_new();
  return priv->pop_table;
}



/* _create_side_panel:
 */
static AltkWidget *_create_side_panel ( AltkWidget *dlg )
{
  AltkWidget *box;
  box = altk_box_new(ALTK_VERTICAL);
  altk_box_pack_start(ALTK_BOX(box),
                      L_TRASH_OBJECT(_create_map_view(dlg)),
                      0);
  altk_box_pack_start(ALTK_BOX(box),
                      L_TRASH_OBJECT(_create_pop_table(dlg)),
                      0);
  return box;
}



/* _create_colony_view:
 */
static AltkWidget *_create_colony_view ( AltkWidget *dlg )
{
  Private *priv = PRIVATE(dlg);
  priv->colony_view = mbtk_colony_view_new();
  return priv->colony_view;
}



/* _create_body:
 */
static AltkWidget *_create_body ( AltkWidget *dlg )
{
  AltkWidget *box;
  box = altk_box_new(ALTK_HORIZONTAL);
  altk_box_pack_start(ALTK_BOX(box),
                      L_TRASH_OBJECT(_create_side_panel(dlg)),
                      0);
  altk_box_pack_start(ALTK_BOX(box),
                      L_TRASH_OBJECT(_create_colony_view(dlg)),
                      0);
  return box;
}



/* mbtk_dialog_new:
 */
AltkWidget *mbtk_dialog_new ( AltkDisplay *display )
{
  AltkWidget *dlg = ALTK_WIDGET(l_object_new(MBTK_CLASS_DIALOG, NULL));
  Private *priv;
  /* [fixme] instance init */
  MBTK_DIALOG(dlg)->private = priv = g_new0(Private, 1);
  altk_dialog_set_display(ALTK_DIALOG(dlg), display);
  l_trash_push();
  priv->top_box = L_TRASH_OBJECT
    (altk_box_new(ALTK_VERTICAL));
  ALTK_CONTAINER_ADD(dlg, priv->top_box);
  altk_box_pack_start(ALTK_BOX(priv->top_box),
                      L_TRASH_OBJECT(_create_header(dlg)),
                      0);
  altk_box_pack_start(ALTK_BOX(priv->top_box),
                      L_TRASH_OBJECT(_create_body(dlg)),
                      0);
  l_trash_pop();
  return dlg;
}



static void _sim_time_notify ( MbcGameProxy *proxy,
                            AltkWidget *label )
{
  gchar text[256];
  sprintf(text, "%d", proxy->sim_time);
  altk_label_set_text(ALTK_LABEL(label), text);
}



static void _unset_sector ( MbtkDialog *dialog )
{
  CL_ERROR("[TODO]");
}



static void _set_sector ( MbtkDialog *dialog,
                          MbcProxy *sector )
{
  Private *priv = PRIVATE(dialog);
  ASSERT(!priv->sector);
  priv->sector = l_object_ref(sector);
  if (MBC_SECTOR_PROXY(sector)->colony)
    {
      mbtk_pop_table_set_colony(MBTK_POP_TABLE(priv->pop_table),
                                MBC_SECTOR_PROXY(sector)->colony);
      mbtk_colony_view_set_colony(MBTK_COLONY_VIEW(priv->colony_view),
                                  MBC_SECTOR_PROXY(sector)->colony);
    }
}



static void mbtk_dialog_set_sector ( MbtkDialog *dialog,
                                     guint x,
                                     guint y )
{
  Private *priv = PRIVATE(dialog);
  MbcProxy *world;
  MbcProxy *sector;
  ASSERT(priv->game);
  world = MBC_GAME_PROXY(priv->game)->world;
  ASSERT(x < MBC_WORLD_PROXY(world)->width);
  ASSERT(y < MBC_WORLD_PROXY(world)->height);
  sector = MBC_WORLD_PROXY(world)->sectors[y][x];
  if (priv->sector == sector)
    return;
  if (priv->sector)
    _unset_sector(dialog);
  _set_sector(dialog, sector);
}



/* mbtk_dialog_setup:
 */
void mbtk_dialog_setup ( MbtkDialog *dialog,
                         MbcGameProxy *game_proxy )
{
  Private *priv = PRIVATE(dialog);
  ASSERT(!priv->game); /* [todo] delete */
  priv->game = l_object_ref(game_proxy);
  /* map view */
  mbtk_map_view_setup(MBTK_MAP_VIEW(priv->map_view), game_proxy);
  /* sim time label */
  l_signal_connect(L_OBJECT(game_proxy),
                   "notify", g_quark_from_string("sim_time"),
                   (LSignalHandler) _sim_time_notify,
                   priv->sim_time_label,
                   NULL);
  /* /\* pop table *\/ */
  /* { */
  /*   MbcProxy *sector = MBC_WORLD_PROXY(game_proxy->world)->sectors[0][0]; */
  /*   MbcProxy *colony = MBC_SECTOR_PROXY(sector)->colony; */
  /*   l_signal_connect(L_OBJECT(colony), */
  /*                    "pop_notify", */
  /*                    0, */
  /*                    (LSignalHandler) _pop_notify, */
  /*                    dialog, */
  /*                    NULL); */
  /* } */
  mbtk_dialog_set_sector(dialog, 0, 0);
}
