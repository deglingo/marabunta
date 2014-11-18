/* mbtkinfopanel.c -
 */

#include "client-al/alprivate.h"
#include "client-al/mbtkinfopanel.h"
#include "client-al/mbtktaskview.h"
#include "client-al/mbtkinfopanel.inl"



/* Page:
 */
typedef struct _Page
{
  gchar *name;
  AltkWidget *panel;
  AltkWidget *frame;
  AltkWidget *body;
}
  Page;



/* Private:
 */
typedef struct _Private
{
  MbcProxy *colony;
  AltkWidget *button_box;
  GList *pages;
  Page *current_page;
}
  Private;

#define PRIVATE(panel) ((Private *)(MBTK_INFO_PANEL(panel)->private))



/* mbtk_info_panel_init:
 */
static void mbtk_info_panel_init ( LObject *obj )
{
  MBTK_INFO_PANEL(obj)->private = g_new0(Private, 1);
}



static void _set_page ( MbtkInfoPanel *panel,
                        Page *page )
{
  Private *priv = PRIVATE(panel);
  ASSERT(page->panel == ALTK_WIDGET(panel));
  if (priv->current_page == page)
    return;
  if (priv->current_page)
    altk_widget_hide(priv->current_page->frame);
  priv->current_page = page;
  altk_widget_show(page->frame);
}



static void _on_page_button_clicked ( AltkWidget *button,
                                      Page *page )
{
  _set_page(MBTK_INFO_PANEL(page->panel), page);
}



static Page *_create_page ( AltkWidget *panel,
                            const gchar *name,
                            const gchar *title,
                            const gchar *label )
{
  Private *priv = PRIVATE(panel);
  Page *page;
  AltkWidget *box, *button, *title_label;
  page = g_new0(Page, 1);
  page->panel = panel;
  page->name = g_strdup(name);
  /* button */
  button = L_TRASH_OBJECT
    (altk_button_new_with_label(label));
  ALTK_BOX_ADD(priv->button_box, button, 0);
  l_signal_connect(L_OBJECT(button),
                   "clicked", 0,
                   (LSignalHandler) _on_page_button_clicked,
                   page,
                   NULL);
  /* page frame */
  page->frame = L_TRASH_OBJECT
    (altk_frame_new(name));
  altk_widget_set_enable_show_all(page->frame, FALSE);
  /* box */
  box = L_TRASH_OBJECT
    (altk_box_new(ALTK_VERTICAL));
  ALTK_CONTAINER_ADD(page->frame, box);
  /* page title */
  title_label = L_TRASH_OBJECT
    (altk_label_new(title));
  ALTK_BOX_ADD(box, title_label, 0);
  /* body */
  page->body = L_TRASH_OBJECT
    (altk_frame_new("body"));
  ALTK_BOX_ADD(box, page->body, ALTK_PACK_EXPAND_FILL);
  /* register */
  priv->pages = g_list_append(priv->pages, page);
  return page;
}



Page *_get_page ( MbtkInfoPanel *panel,
                  const gchar *name )
{
  GList *l;
  for (l = PRIVATE(panel)->pages; l; l = l->next)
    {
      Page *page = l->data;
      if (!strcmp(page->name, name))
        return page;
    }
  return NULL;
}



void _mine_page_set_colony ( MbtkInfoPanel *panel,
                             Page *page,
                             MbcProxy *colony )
{
  MbcTaskProxy *t_mine;
  GList *l;
  AltkWidget *box, *tview;
  /* box */
  box = altk_box_new(ALTK_VERTICAL);
  ALTK_CONTAINER_ADD(page->body, box);
  /* main task */
  t_mine = mbc_task_proxy_find(MBC_TASK_PROXY(MBC_COLONY_PROXY(colony)->top_task),
                               "work/mine");
  ASSERT(t_mine);
  tview = L_TRASH_OBJECT
    (mbtk_task_view_new(MBC_PROXY(t_mine)));
  ALTK_BOX_ADD(box, tview, 0);
  /* sub tasks */
  for (l = t_mine->children; l; l = l->next)
    {
      tview = L_TRASH_OBJECT
        (mbtk_task_view_new(l->data));
      ALTK_BOX_ADD(box, tview, 0);
    }
}



void _food_page_set_colony ( MbtkInfoPanel *panel,
                             Page *page,
                             MbcProxy *colony )
{
  MbcTaskProxy *t_farm;
  GList *l;
  AltkWidget *box, *tview;
  /* box */
  box = altk_box_new(ALTK_VERTICAL);
  ALTK_CONTAINER_ADD(page->body, box);
  /* main task */
  t_farm = mbc_task_proxy_find(MBC_TASK_PROXY(MBC_COLONY_PROXY(colony)->top_task),
                               "work/farm");
  ASSERT(t_farm);
  tview = L_TRASH_OBJECT
    (mbtk_task_view_new(MBC_PROXY(t_farm)));
  ALTK_BOX_ADD(box, tview, 0);
  /* sub tasks */
  for (l = t_farm->children; l; l = l->next)
    {
      tview = L_TRASH_OBJECT
        (mbtk_task_view_new(l->data));
      ALTK_BOX_ADD(box, tview, 0);
    }
}



/* mbtk_info_panel_new:
 */
AltkWidget *mbtk_info_panel_new ( void )
{
  AltkWidget *panel, *top_box;
  Page *page;
  Private *priv;
  panel = ALTK_WIDGET(MBTK_INFO_PANEL_NEW(NULL));
  priv = PRIVATE(panel);
  l_trash_push();
  top_box = L_TRASH_OBJECT
    (altk_box_new(ALTK_HORIZONTAL));
  ALTK_CONTAINER_ADD(panel, top_box);
  /* button_box */
  priv->button_box = L_TRASH_OBJECT
    (altk_box_new(ALTK_VERTICAL));
  /* _add_page(panel, top_box, but_box, "Pop", "P"); */
  page = _create_page(panel, "pop", "Pop", "P");
  ALTK_BOX_ADD(top_box, page->frame, ALTK_PACK_EXPAND_FILL);
  page = _create_page(panel, "food", "Food", "F");
  ALTK_BOX_ADD(top_box, page->frame, ALTK_PACK_EXPAND_FILL);
  page = _create_page(panel, "mine", "Mine", "M");
  ALTK_BOX_ADD(top_box, page->frame, ALTK_PACK_EXPAND_FILL);
  /* add button box */
  ALTK_BOX_ADD(top_box, priv->button_box, ALTK_PACK_ANCHOR_TOP);
  l_trash_pop();
  _set_page(MBTK_INFO_PANEL(panel), priv->pages->data);
  return panel;
}



/* mbtk_info_panel_set_colony:
 */
void mbtk_info_panel_set_colony ( MbtkInfoPanel *panel,
                                  MbcProxy *colony )
{
  Private *priv = PRIVATE(panel);
  ASSERT(colony); /* [todo] */
  ASSERT(MBC_IS_COLONY_PROXY(colony));
  ASSERT(!priv->colony); /* [todo] */
  l_trash_push();
  priv->colony = l_object_ref(colony);
  _food_page_set_colony(panel, _get_page(panel, "food"), colony);
  _mine_page_set_colony(panel, _get_page(panel, "mine"), colony);
  l_trash_pop();
}
