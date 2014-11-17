/* mbtkinfopanel.c -
 */

#include "client-al/mbtkinfopanel.h"
#include "client-al/mbtkinfopanel.inl"



/* PageInfo:
 */
typedef struct _PageInfo
{
  AltkWidget *panel;
  AltkWidget *page;
}
  PageInfo;



/* Private:
 */
typedef struct _Private
{
  GList *pages;
  PageInfo *current_page;
}
  Private;

#define PRIVATE(panel) ((Private *)(MBTK_INFO_PANEL(panel)->private))



/* mbtk_info_panel_init:
 */
static void mbtk_info_panel_init ( LObject *obj )
{
  MBTK_INFO_PANEL(obj)->private = g_new0(Private, 1);
}



static void _on_page_button_clicked ( AltkWidget *button,
                                      PageInfo *info )
{
  Private *priv = PRIVATE(info->panel);
  if (priv->current_page == info)
    return;
  if (priv->current_page)
    altk_widget_hide(priv->current_page->page);
  priv->current_page = info;
  altk_widget_show(priv->current_page->page);
}



static void _add_page ( AltkWidget *panel,
                        AltkWidget *top_box,
                        AltkWidget *but_box,
                        const gchar *label,
                        AltkWidget *page)
{
  Private *priv = PRIVATE(panel);
  PageInfo *info = g_new0(PageInfo, 1);
  AltkWidget *button;
  info->panel = panel;
  button = L_TRASH_OBJECT
    (altk_button_new_with_label(label));
  ALTK_BOX_ADD(but_box, button, 0);
  info->page = page;
  altk_widget_show_all(info->page);
  altk_widget_hide(info->page);
  altk_widget_set_enable_show_all(info->page, FALSE);
  priv->pages = g_list_append(priv->pages, info);
  ALTK_BOX_ADD(top_box, info->page, ALTK_PACK_EXPAND_FILL);
  l_signal_connect(L_OBJECT(button),
                   "clicked", 0,
                   (LSignalHandler) _on_page_button_clicked,
                   info, NULL);
}



AltkWidget *_create_mine_page ( void )
{
  AltkWidget *page, *box, *title;
  page = altk_frame_new("Mine");
  box = L_TRASH_OBJECT
    (altk_box_new(ALTK_VERTICAL));
  ALTK_CONTAINER_ADD(page, box);
  title = L_TRASH_OBJECT
    (altk_label_new("Mine"));
  ALTK_BOX_ADD(box, title, 0);
  return page;
}



/* mbtk_info_panel_new:
 */
AltkWidget *mbtk_info_panel_new ( void )
{
  AltkWidget *panel, *top_box, *but_box;
  panel = ALTK_WIDGET(MBTK_INFO_PANEL_NEW(NULL));
  l_trash_push();
  top_box = L_TRASH_OBJECT
    (altk_box_new(ALTK_HORIZONTAL));
  ALTK_CONTAINER_ADD(panel, top_box);
  but_box = L_TRASH_OBJECT
    (altk_box_new(ALTK_VERTICAL));
  /* _add_page(panel, top_box, but_box, "Pop", "P"); */
  _add_page(panel, top_box, but_box, "M",
            L_TRASH_OBJECT(_create_mine_page()));
  ALTK_BOX_ADD(top_box, but_box, ALTK_PACK_ANCHOR_TOP);
  l_trash_pop();
  return panel;
}
