/* mbtkdialog.c -
 */

#include "client-al/mbtkdialog.h"
#include "client-al/mbtkmapview.h"
#include "client-al/mbtkdialog.inl"



/* Private:
 */
typedef struct _Private
{
  AltkWidget *top_box;
  AltkWidget *sim_time_label;
  AltkWidget *map_view;
  AltkWidget *pop_eggs;
  AltkWidget *pop_lq;
  AltkWidget *pop_aq;
  AltkWidget *pop_tq;
  AltkWidget *pop_lw;
  AltkWidget *pop_aw;
  AltkWidget *pop_tw;
  AltkWidget *pop_ls;
  AltkWidget *pop_as;
  AltkWidget *pop_ts;
  AltkWidget *pop_tl;
  AltkWidget *pop_ta;
  AltkWidget *pop_tt;
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



/* _pop_label:
 */
static void _pop_label ( AltkWidget *table,
                         const gchar *text,
                         gint top,
                         gint left,
                         gint bottom,
                         gint right )
{
  AltkWidget *label = L_TRASH_OBJECT(altk_label_new(text));
  altk_table_attach(ALTK_TABLE(table), label,
                    top, left,
                    bottom, right,
                    0);
}



/* _pop_value:
 */
static AltkWidget *_pop_value ( AltkWidget *table,
                                gint top,
                                gint left,
                                gint bottom,
                                gint right )
{
  AltkWidget *label = L_TRASH_OBJECT(altk_label_new("0"));
  altk_table_attach(ALTK_TABLE(table), label,
                    top, left,
                    bottom, right,
                    0);
  return label;
}



/* _create_pop_table:
 */
static AltkWidget *_create_pop_table ( AltkWidget *dlg )
{
  Private *priv = PRIVATE(dlg);
  AltkWidget *frame, *table;
  frame = altk_frame_new("pop");
  table = L_TRASH_OBJECT(altk_table_new());
  ALTK_CONTAINER_ADD(frame, table);
  _pop_label(table, "E", 0, 0, 1, 1);
  _pop_label(table, "L", 0, 2, 1, 3);
  _pop_label(table, "A", 0, 3, 1, 4);
  _pop_label(table, "T", 0, 4, 1, 5);
  _pop_label(table, "Q", 1, 1, 2, 2);
  _pop_label(table, "W", 2, 1, 3, 2);
  _pop_label(table, "S", 3, 1, 4, 2);
  _pop_label(table, "T", 4, 1, 5, 2);
  priv->pop_eggs = _pop_value(table, 1, 0, 4, 2);
  priv->pop_lq   = _pop_value(table, 1, 2, 2, 3);
  priv->pop_aq   = _pop_value(table, 1, 3, 2, 4);
  priv->pop_tq   = _pop_value(table, 1, 4, 2, 5);
  priv->pop_lw   = _pop_value(table, 2, 2, 3, 3);
  priv->pop_aw   = _pop_value(table, 2, 3, 3, 4);
  priv->pop_tw   = _pop_value(table, 2, 4, 3, 5);
  priv->pop_ls   = _pop_value(table, 3, 2, 4, 3);
  priv->pop_as   = _pop_value(table, 3, 3, 4, 4);
  priv->pop_ts   = _pop_value(table, 3, 4, 4, 5);
  priv->pop_tl   = _pop_value(table, 4, 2, 5, 3);
  priv->pop_ta   = _pop_value(table, 4, 3, 5, 4);
  priv->pop_tt   = _pop_value(table, 4, 4, 5, 5);
  return frame;
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



/* _create_body:
 */
static AltkWidget *_create_body ( AltkWidget *dlg )
{
  AltkWidget *box;
  box = altk_box_new(ALTK_HORIZONTAL);
  altk_box_pack_start(ALTK_BOX(box),
                      L_TRASH_OBJECT(_create_side_panel(dlg)),
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



static void set_pop_label ( AltkWidget *label,
                            gint64 pop )
{
  gchar text[12]; /* ?? */
  if (pop < 1000)
    sprintf(text, "%dU", (gint) pop);
  else if (pop < 1000000)
    sprintf(text, "%.1fK", ((gdouble) pop) / 1000.0);
  else
    sprintf(text, "%.1fM", ((gdouble) pop) / 1000000.0);    
  altk_label_set_text(ALTK_LABEL(label), text);
}



static void _pop_notify ( MbcColonyProxy *proxy,
                          AltkWidget *dialog )
{
  Private *priv = PRIVATE(dialog);
  set_pop_label(priv->pop_eggs, proxy->pop[MB_POP_EGG]);
}



/* mbtk_dialog_setup:
 */
void mbtk_dialog_setup ( MbtkDialog *dialog,
                         MbcGameProxy *game_proxy )
{
  Private *priv = PRIVATE(dialog);
  /* map view */
  mbtk_map_view_setup(MBTK_MAP_VIEW(priv->map_view), game_proxy);
  /* sim time label */
  l_signal_connect(L_OBJECT(game_proxy),
                   "notify", g_quark_from_string("sim_time"),
                   (LSignalHandler) _sim_time_notify,
                   priv->sim_time_label,
                   NULL);
  /* pop table */
  {
    MbcSectorProxy *sector = game_proxy->world->sectors[0][0];
    MbcColonyProxy *colony = sector->colony;
    l_signal_connect(L_OBJECT(colony),
                     "pop_notify",
                     0,
                     (LSignalHandler) _pop_notify,
                     dialog,
                     NULL);
  }
}
