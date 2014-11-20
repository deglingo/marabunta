/* mbtkinfopanel.c -
 */

#include "client-al/alprivate.h"
#include "client-al/mbtkinfopanel.h"
#include "client-al/mbtkinfopanel.inl"



/* Page:
 */
typedef struct _Page
{
  gchar *name;
  AltkWidget *panel;
  AltkWidget *top;
  AltkWidget *body;
}
  Page;



/* Private:
 */
typedef struct _Private
{
  MbObject *colony;
  GList *pages;
  Page *current_page;
  AltkWidget *button_box;
  AltkWidget *pages_box;
}
  Private;

#define PRIVATE(panel) ((Private *)(MBTK_INFO_PANEL(panel)->private))



/* mbtk_info_panel_init:
 */
static void mbtk_info_panel_init ( LObject *obj )
{
  MBTK_INFO_PANEL(obj)->private = g_new0(Private, 1);
}



/* _set_page:
 */
static void _set_page ( MbtkInfoPanel *panel,
                        Page *page )
{
  Private *priv = PRIVATE(panel);
  if (priv->current_page == page)
    return;
  if (priv->current_page)
    altk_widget_hide(priv->current_page->top);
  if ((priv->current_page = page))
    altk_widget_show(page->top);
}



/* _on_page_button_clicked:
 */
static void _on_page_button_clicked ( AltkWidget *button,
                                      Page *page )
{
  _set_page(MBTK_INFO_PANEL(page->panel), page);
}



/* _create_page:
 */
static Page *_create_page ( AltkWidget *panel,
                            const gchar *name,
                            const gchar *title,
                            const gchar *but_label )
{
  Private *priv = PRIVATE(panel);
  Page *page = g_new0(Page, 1);
  AltkWidget *but, *top_box, *title_label;
  priv->pages = g_list_append(priv->pages, page);
  page->panel = panel;
  page->name = g_strdup(name);
  /* button */
  but = L_TRASH_OBJECT
    (altk_button_new_with_label(but_label));
  ALTK_BOX_ADD(priv->button_box, but, 0);
  l_signal_connect(L_OBJECT(but),
                   "clicked", 0,
                   (LSignalHandler) _on_page_button_clicked,
                   page,
                   NULL);
  /* top frame */
  page->top = L_TRASH_OBJECT
    (altk_frame_new(""));
  altk_widget_set_enable_show_all(page->top, FALSE);
  ALTK_BOX_ADD(priv->pages_box, page->top, ALTK_PACK_EXPAND_FILL);
  /* top box */
  top_box = L_TRASH_OBJECT
    (altk_box_new(ALTK_VERTICAL));
  ALTK_CONTAINER_ADD(page->top, top_box);
  /* title */
  title_label = L_TRASH_OBJECT
    (altk_label_new(title));
  ALTK_BOX_ADD(top_box, title_label, 0);
  /* body */
  page->body = L_TRASH_OBJECT
    (altk_frame_new(""));
  ALTK_BOX_ADD(top_box, page->body, ALTK_PACK_EXPAND_FILL);
  /* ok */
  return page;
}



/* _create_food_page:
 */
static void _create_food_page ( AltkWidget *panel )
{
  /* Private *priv = PRIVATE(panel); */
  /* Page *page; */
  /* page = */ _create_page(panel, "food", "Food", "F");
}



/* _create_mine_page:
 */
static void _create_mine_page ( AltkWidget *panel )
{
  _create_page(panel, "mine", "Mine", "M");
}



/* _create_panel:
 */
static void _create_panel ( AltkWidget *panel )
{
  Private *priv = PRIVATE(panel);
  AltkWidget *top_box;
  /* top_box */
  top_box = L_TRASH_OBJECT
    (altk_box_new(ALTK_HORIZONTAL));
  ALTK_CONTAINER_ADD(panel, top_box);
  /* page box */
  priv->pages_box = L_TRASH_OBJECT
    (altk_box_new(ALTK_HORIZONTAL));
  ALTK_BOX_ADD(top_box, priv->pages_box, ALTK_PACK_EXPAND_FILL);
  /* button_box */
  priv->button_box = L_TRASH_OBJECT
    (altk_box_new(ALTK_VERTICAL));
  ALTK_BOX_ADD(top_box, priv->button_box, ALTK_PACK_ANCHOR_TOP);
  /* create the pages */
  _create_food_page(panel);
  _create_mine_page(panel);
  /* open the first page */
  _set_page(MBTK_INFO_PANEL(panel), priv->pages->data);
}



/* mbtk_info_panel_new:
 */
AltkWidget *mbtk_info_panel_new ( void )
{
  AltkWidget *panel;
  panel = ALTK_WIDGET(l_object_new(MBTK_CLASS_INFO_PANEL, NULL));
  l_trash_push();
  _create_panel(panel);
  l_trash_pop();
  return panel;
}



/* mbtk_info_panel_set_colony:
 */
void mbtk_info_panel_set_colony ( MbtkInfoPanel *panel,
                                  MbObject *colony )
{
  Private *priv = PRIVATE(panel);
  ASSERT(!priv->colony); /* [todo] */
  priv->colony = l_object_ref(colony);
}
