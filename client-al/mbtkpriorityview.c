/* mbtkpriorityview.c -
 */

#include "client-al/alprivate.h"
#include "client-al/mbtkpriorityview.h"
#include "client-al/mbtkpriorityview.inl"



/* Private:
 */
typedef struct _Private
{
  MbObject *priority;
  AltkWidget *value_label;
  AltkWidget *inc_button;
  AltkWidget *dec_button;
}
  Private;

#define PRIVATE(view) ((Private *)(MBTK_PRIORITY_VIEW(view)->private))



/* mbtk_priority_view_init:
 */
static void mbtk_priority_view_init ( LObject *obj )
{
  MBTK_PRIORITY_VIEW(obj)->private = g_new0(Private, 1);
}



static void _update_value_label ( MbtkPriorityView *view )
{
  Private *priv = PRIVATE(view);
  gchar text[2];
  if (priv->priority)
    {
      ASSERT(MB_PRIORITY_VALUE(priv->priority) >= 0 &&
             MB_PRIORITY_VALUE(priv->priority) < 10);
      sprintf(text, "%d", MB_PRIORITY_VALUE(priv->priority));
    }
  else
    {
      sprintf(text, "?");
    }
  altk_label_set_text(ALTK_LABEL(priv->value_label), text);
}



static void _on_inc_clicked ( AltkWidget *button,
                              AltkWidget *view )
{
  Private *priv = PRIVATE(view);
  if (priv->priority)
    /* [FIXME] request */
    mb_priority_set_value(MB_PRIORITY(priv->priority),
                          MB_PRIORITY_VALUE(priv->priority) + 1);
}



static void _on_dec_clicked ( AltkWidget *button,
                              AltkWidget *view )
{
  Private *priv = PRIVATE(view);
  if (priv->priority)
    mb_priority_set_value(MB_PRIORITY(priv->priority),
                          MB_PRIORITY_VALUE(priv->priority) - 1);
}



/* _create_view:
 */
static void _create_view ( AltkWidget *view )
{
  Private *priv = PRIVATE(view);
  AltkWidget *box;
  /* top box */
  box = L_TRASH_OBJECT
    (altk_box_new(ALTK_HORIZONTAL));
  ALTK_CONTAINER_ADD(view, box);
  /* value label */
  priv->value_label = L_TRASH_OBJECT
    (altk_label_new("0"));
  ALTK_BOX_ADD(box, priv->value_label, 0);
  /* inc button */
  priv->inc_button = L_TRASH_OBJECT
    (altk_button_new_with_label("+"));
  ALTK_BOX_ADD(box, priv->inc_button, 0);
  l_signal_connect(L_OBJECT(priv->inc_button),
                   "clicked",
                   (LSignalHandler) _on_inc_clicked,
                   view,
                   NULL);
  /* dec button */
  priv->dec_button = L_TRASH_OBJECT
    (altk_button_new_with_label("-"));
  ALTK_BOX_ADD(box, priv->dec_button, 0);
  l_signal_connect(L_OBJECT(priv->dec_button),
                   "clicked",
                   (LSignalHandler) _on_dec_clicked,
                   view,
                   NULL);
  /* set value */
  _update_value_label(MBTK_PRIORITY_VIEW(view));
}



/* mbtk_priority_view_new:
 */
AltkWidget *mbtk_priority_view_new ( void )
{
  AltkWidget *view;
  view = ALTK_WIDGET(l_object_new(MBTK_CLASS_PRIORITY_VIEW, NULL));
  l_trash_push();
  _create_view(view);
  l_trash_pop();
  return view;
}



static void _on_priority_value_notify ( MbObject *priority,
                                        MbtkPriorityView *view )
{
  _update_value_label(view);
}



/* mbtk_priority_view_set_priority:
 */
void mbtk_priority_view_set_priority ( MbtkPriorityView *view,
                                       MbObject *priority )
{
  Private *priv = PRIVATE(view);
  ASSERT(!priv->priority); /* [todo] */
  priv->priority = l_object_ref(priority);
  _update_value_label(view);
  l_signal_connect(L_OBJECT(priority),
                   "notify:value",
                   (LSignalHandler) _on_priority_value_notify,
                   view,
                   NULL);
}
