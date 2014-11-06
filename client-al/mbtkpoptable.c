/* mbtkpoptable.c -
 */

#include "client-al/alprivate.h"
#include "client-al/mbtkpoptable.h"
#include "client-al/mbtkpoptable.inl"



/* Private:
 */
typedef struct _Private
{
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
  MbcProxy *colony;
}
  Private;

#define PRIVATE(table) ((Private *)(MBTK_POP_TABLE(table)->private))



/* mbtk_pop_table_init:
 */
static void mbtk_pop_table_init ( LObject *obj )
{
  MBTK_POP_TABLE(obj)->private = g_new0(Private, 1);
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



/* mbtk_pop_table_new:
 */
AltkWidget *mbtk_pop_table_new ( void )
{
  Private *priv;
  AltkWidget *frame, *table;
  l_trash_push();
  frame = ALTK_WIDGET(MBTK_POP_TABLE_NEW(NULL));
  priv = PRIVATE(frame);
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
  priv->pop_eggs = _pop_value(table, 1, 0, 4, 1);
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
  l_trash_pop();
  return frame;
}



/* set_pop_label:
 */
static void set_pop_label ( AltkWidget *label,
                            gint64 pop )
{
  gchar text[12]; /* ?? */
  if (pop < 1000)
    sprintf(text, "%dU", (gint) pop);
  else if (pop < 1000000)
    sprintf(text, "%.2fK", ((gdouble) pop) / 1000.0);
  else
    sprintf(text, "%.2fM", ((gdouble) pop) / 1000000.0);    
  altk_label_set_text(ALTK_LABEL(label), text);
}



/* _on_pop_notify:
 */
static void _on_pop_notify ( MbcColonyProxy *proxy,
                             AltkWidget *table )
{
  Private *priv = PRIVATE(table);
  set_pop_label(priv->pop_eggs, proxy->pop[MB_POP_EGG]);
  set_pop_label(priv->pop_lq, proxy->pop[MB_POP_LARVAE_QUEEN]);
  set_pop_label(priv->pop_lw, proxy->pop[MB_POP_LARVAE_WORKER]);
  set_pop_label(priv->pop_ls, proxy->pop[MB_POP_LARVAE_SOLDIER]);
  set_pop_label(priv->pop_aq, proxy->pop[MB_POP_ADULT_QUEEN]);
  set_pop_label(priv->pop_aw, proxy->pop[MB_POP_ADULT_WORKER]);
  set_pop_label(priv->pop_as, proxy->pop[MB_POP_ADULT_SOLDIER]);
}



/* mbtk_pop_table_set_colony:
 */
void mbtk_pop_table_set_colony ( MbtkPopTable *table,
                                 MbcProxy *colony )
{
  Private *priv = PRIVATE(table);
  ASSERT(MBC_IS_COLONY_PROXY(colony));
  ASSERT(!priv->colony);
  priv->colony = l_object_ref(colony);
  l_signal_connect(L_OBJECT(colony),
                   "pop_notify", 0,
                   (LSignalHandler) _on_pop_notify,
                   table,
                   NULL);
}
