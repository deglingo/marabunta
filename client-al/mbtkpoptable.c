/* mbtkpoptable.c -
 */

#include "client-al/alprivate.h"
#include "client-al/mbtkpoptable.h"
#include "client-al/mbtkpoptable.inl"



/* Value:
 */
typedef struct _Value
{
  MbPopType pop_type;
  AltkWidget *label;
}
  Value;



/* Total:
 */
typedef struct _Total
{
  MbPopFlags pop_flags;
  AltkWidget *label;
}
  Total;



/* Private:
 */
typedef struct _Private
{
  MbcColony *colony;
  AltkWidget *table;
  Value values[MB_POP_TYPE_COUNT];
  GList *totals;
}
  Private;

#define PRIVATE(table) ((Private *)(MBTK_POP_TABLE(table)->private))



/* _create_label:
 */
static void _create_label ( AltkWidget *table,
                            const gchar *text,
                            gint top,
                            gint left,
                            gint bottom,
                            gint right )
{
  Private *priv = PRIVATE(table);
  AltkWidget *label;
  label = L_TRASH_OBJECT
    (altk_label_new(text));
  altk_table_attach(ALTK_TABLE(priv->table),
                    label,
                    top,
                    left,
                    bottom,
                    right,
                    0);
}



/* _create_value:
 */
static void _create_value ( AltkWidget *table,
                            MbPopType pop_type,
                            gint top,
                            gint left,
                            gint bottom,
                            gint right )
{
  Private *priv = PRIVATE(table);
  Value *value = &priv->values[pop_type];
  ASSERT(!value->label);
  value->pop_type = pop_type;
  value->label = L_TRASH_OBJECT
    (altk_label_new("0"));
  altk_label_set_alignment(ALTK_LABEL(value->label),
                           ALTK_PACK_ANCHOR_RIGHT);
  altk_label_set_min_chars(ALTK_LABEL(value->label),
                           MB_COUNT_CHARS);
  altk_table_attach(ALTK_TABLE(priv->table),
                    value->label,
                    top,
                    left,
                    bottom,
                    right,
                    ALTK_PACK_HEXPAND_FILL);
}



/* _create_total:
 */
static void _create_total ( AltkWidget *table,
                            MbPopFlags pop_flags,
                            gint top,
                            gint left,
                            gint bottom,
                            gint right )
{
  Private *priv = PRIVATE(table);
  Total *total = g_new0(Total, 1);
  priv->totals = g_list_append(priv->totals, total);
  total->pop_flags = pop_flags;
  total->label = L_TRASH_OBJECT
    (altk_label_new("0"));
  altk_label_set_alignment(ALTK_LABEL(total->label),
                           ALTK_PACK_ANCHOR_RIGHT);
  altk_label_set_min_chars(ALTK_LABEL(total->label),
                           MB_COUNT_CHARS);
  altk_table_attach(ALTK_TABLE(priv->table),
                    total->label,
                    top,
                    left,
                    bottom,
                    right,
                    ALTK_PACK_HEXPAND_FILL);
}



/* _create:
 */
static void _create ( AltkWidget *table )
{
  Private *priv = PRIVATE(table);
  /* top table */
  priv->table = L_TRASH_OBJECT
    (altk_table_new());
  ALTK_CONTAINER_ADD(table, priv->table);
  /* labels */
  _create_label(table, "E", 0, 0, 1, 1);
  _create_label(table, "Q", 1, 1, 2, 2);
  _create_label(table, "W", 2, 1, 3, 2);
  _create_label(table, "S", 3, 1, 4, 2);
  _create_label(table, "T", 4, 1, 5, 2);
  _create_label(table, "L", 0, 2, 1, 3);
  _create_label(table, "A", 0, 3, 1, 4);
  _create_label(table, "T", 0, 4, 1, 5);
  /* values */
  _create_value(table, MB_POP_EGG,            1, 0, 5, 1);
  _create_value(table, MB_POP_LARVAE_QUEEN,   1, 2, 2, 3);
  _create_value(table, MB_POP_LARVAE_WORKER,  2, 2, 3, 3);
  _create_value(table, MB_POP_LARVAE_SOLDIER, 3, 2, 4, 3);
  _create_value(table, MB_POP_ADULT_QUEEN,    1, 3, 2, 4);
  _create_value(table, MB_POP_ADULT_WORKER,   2, 3, 3, 4);
  _create_value(table, MB_POP_ADULT_SOLDIER,  3, 3, 4, 4);
  /* totals */
  _create_total(table, MB_POP_FLAG_LARVAE,  4, 2, 5, 3);
  _create_total(table, MB_POP_FLAG_ADULT,   4, 3, 5, 4);
  _create_total(table, MB_POP_FLAG_QUEEN,   1, 4, 2, 5);
  _create_total(table, MB_POP_FLAG_WORKER,  2, 4, 3, 5);
  _create_total(table, MB_POP_FLAG_SOLDIER, 3, 4, 4, 5);
  /* grand total [fixme] include eggs ? */
  _create_total(table, MB_POP_FLAG_LARVAE | MB_POP_FLAG_ADULT, 4, 4, 5, 5);
}



/* mbtk_pop_table_init:
 */
static void mbtk_pop_table_init ( LObject *obj )
{
  MBTK_POP_TABLE(obj)->private = g_new0(Private, 1);
  l_trash_push();
  _create(ALTK_WIDGET(obj));
  l_trash_pop();
}



/* mbtk_pop_table_new:
 */
AltkWidget *mbtk_pop_table_new ( void )
{
  return ALTK_WIDGET(l_object_new(MBTK_CLASS_POP_TABLE, NULL));
}



/* _on_pop_notify:
 */
static void _on_pop_notify ( MbObject *colony,
                             AltkWidget *table )
{
  Private *priv = PRIVATE(table);
  gint tp;
  gchar text[MB_COUNT_CHARS+1];
  GList *l;
  for (tp = 0; tp < MB_POP_TYPE_COUNT; tp++)
    {
      mb_count_print(MBC_COLONY(colony)->pop[tp], text);
      altk_label_set_text(ALTK_LABEL(priv->values[tp].label), text);
    }
  for (l = priv->totals; l; l = l->next)
    {
      Total *total = l->data;
      mb_count_print(mb_pop_total(MBC_COLONY(colony)->pop, total->pop_flags), text);
      altk_label_set_text(ALTK_LABEL(total->label), text);
    }
}



/* mbtk_pop_table_set_colony:
 */
void mbtk_pop_table_set_colony ( MbtkPopTable *table,
                                 MbObject *colony )
{
  Private *priv = PRIVATE(table);
  ASSERT(MB_IS_COLONY(colony));
  ASSERT(!priv->colony); /* [todo] */
  priv->colony = l_object_ref(colony);
  l_signal_connect(L_OBJECT(colony),
                   "pop_notify",
                   (LSignalHandler) _on_pop_notify,
                   table,
                   NULL);
}
