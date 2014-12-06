/* mbtkinfopanel.c -
 */

#include "mbtkinfopanel.h"
#include "mbtkroomspage.h"
#include "mbtkinfopanel.inl"



/* Page:
 */
typedef struct _Page
{
  AltkWidget *panel;
  AltkWidget *widget;
}
  Page;



/* Private:
 */
typedef struct _Private
{
  MbSector *sector;
  AltkWidget *top_box;
  AltkWidget *button_box;
  GList *pages;
  Page *current_page;
}
  Private;

#define PRIVATE(p) ((Private *)(MBTK_INFO_PANEL(p)->private))



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
  if (page == priv->current_page)
    return;
  if (priv->current_page)
    {
      altk_widget_hide(priv->current_page->widget);
    }
  if ((priv->current_page = page))
    {
      mbtk_info_page_setup(MBTK_INFO_PAGE(page->widget));
      altk_widget_show(page->widget);
    }
}



/* _add_page:
 */
static void _add_page ( AltkWidget *panel,
                        const gchar *button_label,
                        AltkWidget *page_widget )
{
  Private *priv = PRIVATE(panel);
  Page *page;
  AltkWidget *but;
  page = g_new0(Page, 1);
  page->panel = panel;
  page->widget = l_object_ref(page_widget);
  but = L_TRASH_OBJECT
    (altk_button_new_with_label(button_label));
  ALTK_BOX_ADD(priv->button_box, but, 0);
  ALTK_BOX_ADD(priv->top_box, page_widget, ALTK_PACK_EXPAND_FILL);
  priv->pages = g_list_append(priv->pages, page);
  altk_widget_set_enable_show_all(page_widget, FALSE);
}



/* _create_panel:
 */
static void _create_panel ( AltkWidget *panel )
{
  Private *priv = PRIVATE(panel);
  priv->top_box = L_TRASH_OBJECT
    (altk_box_new(ALTK_HORIZONTAL));
  ALTK_CONTAINER_ADD(panel, priv->top_box);
  priv->button_box = L_TRASH_OBJECT
    (altk_box_new(ALTK_VERTICAL));
  _add_page(panel,
            "R",
            L_TRASH_OBJECT(mbtk_rooms_page_new()));
  ALTK_BOX_ADD(priv->top_box, priv->button_box, ALTK_PACK_ANCHOR_TOP);
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
  _set_page(MBTK_INFO_PANEL(panel), PRIVATE(panel)->pages->data);
  return panel;
}



/* mbtk_info_panel_set_sector:
 */
void mbtk_info_panel_set_sector ( MbtkInfoPanel *panel,
                                  MbSector *sector )
{
  Private *priv = PRIVATE(panel);
  GList *l;
  if (priv->sector == sector)
    return;
  ASSERT(!priv->sector); /* [todo] */
  if ((priv->sector = sector))
    l_object_ref(priv->sector);
  for (l = priv->pages; l; l = l->next)
    {
      Page *page = l->data;
      mbtk_info_page_setup(MBTK_INFO_PAGE(page->widget));
      mbtk_info_page_set_sector(MBTK_INFO_PAGE(page->widget), sector);
    }
  if (priv->current_page)
    mbtk_info_page_setup(MBTK_INFO_PAGE(priv->current_page->widget));
}
