/* mbtkcolonyview.c -
 */

#include "client-al/alprivate.h"
#include "client-al/mbtkcolonyview.h"
#include "client-al/mbtkcolonyview.inl"



/* Private:
 */
typedef struct _Private
{
  AltkWidget *title;
  MbcProxy *colony;
}
  Private;

#define PRIVATE(obj) ((Private *)(MBTK_COLONY_VIEW(obj)->private))



/* mbtk_colony_view_init:
 */
static void mbtk_colony_view_init ( LObject *obj )
{
  MBTK_COLONY_VIEW(obj)->private = g_new0(Private, 1);
}



/* mbtk_colony_view_new:
 */
AltkWidget *mbtk_colony_view_new ( void )
{
  AltkWidget *view, *box;
  Private *priv;
  l_trash_push();
  view = ALTK_WIDGET(MBTK_COLONY_VIEW_NEW(NULL));
  priv = PRIVATE(view);
  box = L_TRASH_OBJECT
    (altk_box_new(ALTK_VERTICAL));
  ALTK_CONTAINER_ADD(view, box);
  priv->title = L_TRASH_OBJECT
    (altk_label_new("TITLE"));
  altk_box_pack_start(ALTK_BOX(box), priv->title, 0);
  l_trash_pop();
  return view;
}



/* mbtk_colony_view_set_colony:
 */
void mbtk_colony_view_set_colony ( MbtkColonyView *view,
                                   MbcProxy *colony )
{
  Private *priv = PRIVATE(view);
  gchar *title;
  ASSERT(MBC_IS_COLONY_PROXY(colony));
  ASSERT(!priv->colony); /* [todo] */
  priv->colony = l_object_ref(colony);
  title = l_object_to_string(L_OBJECT(colony));
  altk_label_set_text(ALTK_LABEL(priv->title), title);
  g_free(title);
}
