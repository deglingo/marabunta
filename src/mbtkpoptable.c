/* mbtkpoptable.c -
 */

#include "mbtkpoptable.h"
#include "mbtkpoptable.inl"



/* Private:
 */
typedef struct _Private
{
  LSignalHandlerGroup *sig_group;
  MbColony *colony;
  /* pop labels */
  AltkWidget *pop_label_egg;
  AltkWidget *pop_label_lq;
  AltkWidget *pop_label_lw;
  AltkWidget *pop_label_ls;
  AltkWidget *pop_label_aq;
  AltkWidget *pop_label_aw;
  AltkWidget *pop_label_as;
  /* totals */
  AltkWidget *pop_label_tl;
  AltkWidget *pop_label_ta;
  AltkWidget *pop_label_tq;
  AltkWidget *pop_label_tw;
  AltkWidget *pop_label_ts;
  AltkWidget *pop_label_tt;
}
  Private;

#define PRIVATE(table) ((Private *)(MBTK_POP_TABLE(table)->private))



/* mbtk_pop_table_init:
 */
static void mbtk_pop_table_init ( LObject *obj )
{
  MBTK_POP_TABLE(obj)->private = g_new0(Private, 1);
  PRIVATE(obj)->sig_group = l_signal_handler_group_new();
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
  /* Private *priv = PRIVATE(table); */
  AltkWidget *label;
  label = L_TRASH_OBJECT
    (altk_label_new(text));
  altk_table_attach(ALTK_TABLE(table),
                    label,
                    top,
                    left,
                    bottom,
                    right,
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
  AltkWidget *label;
  label = L_TRASH_OBJECT
    (altk_label_new("0"));
  altk_table_attach(ALTK_TABLE(table),
                    label,
                    top,
                    left,
                    bottom,
                    right,
                    0);
  return label;
}



/* _create_table:
 */
static void _create_table ( AltkWidget *table )
{
  Private *priv = PRIVATE(table);
  _pop_label(table, "E", 0, 0, 1, 1);
  _pop_label(table, "L", 0, 2, 1, 3);
  _pop_label(table, "A", 0, 3, 1, 4);
  _pop_label(table, "T", 0, 4, 1, 5);
  _pop_label(table, "Q", 1, 1, 2, 2);
  _pop_label(table, "W", 2, 1, 3, 2);
  _pop_label(table, "S", 3, 1, 4, 2);
  _pop_label(table, "T", 4, 1, 5, 2);
  priv->pop_label_egg = _pop_value(table, 1, 0, 5, 1);  
  priv->pop_label_lq = _pop_value(table, 1, 2, 2, 3);  
  priv->pop_label_lw = _pop_value(table, 2, 2, 3, 3);  
  priv->pop_label_ls = _pop_value(table, 3, 2, 4, 3);  
  priv->pop_label_tl = _pop_value(table, 4, 2, 5, 3);
  priv->pop_label_aq = _pop_value(table, 1, 3, 2, 4);  
  priv->pop_label_aw = _pop_value(table, 2, 3, 3, 4);  
  priv->pop_label_as = _pop_value(table, 3, 3, 4, 4);  
  priv->pop_label_ta = _pop_value(table, 4, 3, 5, 4);
  priv->pop_label_tq = _pop_value(table, 1, 4, 2, 5);  
  priv->pop_label_tw = _pop_value(table, 2, 4, 3, 5);  
  priv->pop_label_ts = _pop_value(table, 3, 4, 4, 5);  
  priv->pop_label_tt = _pop_value(table, 4, 4, 5, 5);
}



/* mbtk_pop_table_new:
 */
AltkWidget *mbtk_pop_table_new ( void )
{
  AltkWidget *table;
  table = ALTK_WIDGET(l_object_new(MBTK_CLASS_POP_TABLE, NULL));
  l_trash_push();
  _create_table(table);
  l_trash_pop();
  return table;
}



/* _set_pop_label:
 */
static void _set_pop_label ( AltkWidget *label,
                             gint64 count )
{
  gchar text[MB_COUNT_MAX_CHARS+1];
  mb_count_print(count, text);
  altk_label_set_text(ALTK_LABEL(label), text);
}



/* _on_pop_notify:
 */
static void _on_pop_notify ( MbColony *colony,
                             MbtkPopTable *table )
{
  Private *priv = PRIVATE(table);
  _set_pop_label(priv->pop_label_egg, colony->pop[MB_POP_EGG]);
  _set_pop_label(priv->pop_label_lq, colony->pop[MB_POP_LARVAE_QUEEN]);
  _set_pop_label(priv->pop_label_lw, colony->pop[MB_POP_LARVAE_WORKER]);
  _set_pop_label(priv->pop_label_ls, colony->pop[MB_POP_LARVAE_SOLDIER]);
  _set_pop_label(priv->pop_label_aq, colony->pop[MB_POP_ADULT_QUEEN]);
  _set_pop_label(priv->pop_label_aw, colony->pop[MB_POP_ADULT_WORKER]);
  _set_pop_label(priv->pop_label_as, colony->pop[MB_POP_ADULT_SOLDIER]);
  _set_pop_label(priv->pop_label_tl, mb_pop_total(colony->pop, MB_POP_FLAG_LARVAE));
  _set_pop_label(priv->pop_label_ta, mb_pop_total(colony->pop, MB_POP_FLAG_ADULT));
  _set_pop_label(priv->pop_label_tq, mb_pop_total(colony->pop, MB_POP_FLAG_QUEEN));
  _set_pop_label(priv->pop_label_tw, mb_pop_total(colony->pop, MB_POP_FLAG_WORKER));
  _set_pop_label(priv->pop_label_ts, mb_pop_total(colony->pop, MB_POP_FLAG_SOLDIER));
  _set_pop_label(priv->pop_label_tt, mb_pop_total(colony->pop, MB_POP_FLAG_ADULT | MB_POP_FLAG_LARVAE));
}



/* mbtk_pop_table_set_colony:
 */
void mbtk_pop_table_set_colony ( MbtkPopTable *table,
                                 struct _MbColony *colony )
{
  Private *priv = PRIVATE(table);
  if (colony == priv->colony)
    return;
  ASSERT(!priv->colony); /* [todo] */
  priv->colony = l_object_ref(colony);
  l_signal_handler_group_add
    (priv->sig_group,
     l_signal_connect(L_OBJECT(colony),
                      "pop_notify",
                      (LSignalHandler) _on_pop_notify,
                      table,
                      NULL));
  _on_pop_notify(colony, table);
}
