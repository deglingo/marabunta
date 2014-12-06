/* mbtkinfopanel.c -
 */

#include "mbtkinfopanel.h"
#include "mbtkroomspage.h"
#include "mbtkinfopanel.inl"



/* Private:
 */
typedef struct _Private
{
  AltkWidget *top_box;
  AltkWidget *button_box;
  GList *pages;
}
  Private;

#define PRIVATE(p) ((Private *)(MBTK_INFO_PANEL(p)->private))



/* mbtk_info_panel_init:
 */
static void mbtk_info_panel_init ( LObject *obj )
{
  MBTK_INFO_PANEL(obj)->private = g_new0(Private, 1);
}



/* _add_page:
 */
static void _add_page ( AltkWidget *panel,
                        const gchar *button_label,
                        AltkWidget *page_widget )
{
  Private *priv = PRIVATE(panel);
  AltkWidget *but;
  but = L_TRASH_OBJECT
    (altk_button_new_with_label(button_label));
  ALTK_BOX_ADD(priv->button_box, but, 0);
  ALTK_BOX_ADD(priv->top_box, page_widget, ALTK_PACK_EXPAND_FILL);
  priv->pages = g_list_append(priv->pages, page_widget);
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
  return panel;
}
