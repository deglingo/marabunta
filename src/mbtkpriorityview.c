/* mbtkpriorityview.c -
 */

#include "mbtkpriorityview.h"
#include "mbtkpriorityview.inl"



/* Private:
 */
typedef struct _Private
{
  LSignalHandlerGroup *sig_group;
  MbPriority *priority;
  AltkWidget *value_label;
  AltkWidget *inc_button;
  AltkWidget *dec_button;
}
  Private;

#define PRIVATE(view) ((Private *)(MBTK_PRIORITY_VIEW(view)->private))



/* mbtk_priority_view_class_init:
 */
static void mbtk_priority_view_class_init ( LObjectClass *cls )
{
}



/* mbtk_priority_view_init:
 */
static void mbtk_priority_view_init ( LObject *obj )
{
  MBTK_PRIORITY_VIEW(obj)->private = g_new0(Private, 1);
  PRIVATE(obj)->sig_group = l_signal_handler_group_new();
}



/* _create_view:
 */
static void _create_view ( AltkWidget *view )
{
  Private *priv = PRIVATE(view);
  AltkWidget *box;
  box = L_TRASH_OBJECT
    (altk_box_new(ALTK_HORIZONTAL));
  ALTK_CONTAINER_ADD(view, box);
  priv->value_label = L_TRASH_OBJECT
    (altk_label_new(""));
  altk_label_set_min_chars(ALTK_LABEL(priv->value_label), 2);
  ALTK_BOX_ADD(box, priv->value_label, 0);
  priv->inc_button = L_TRASH_OBJECT
    (altk_button_new_with_label("+"));
  ALTK_BOX_ADD(box, priv->inc_button, 0);
  priv->dec_button = L_TRASH_OBJECT
    (altk_button_new_with_label("-"));
  ALTK_BOX_ADD(box, priv->dec_button, 0);
}



/* mbtk_priority_view_new:
 */
AltkWidget *mbtk_priority_view_new ( struct _MbPriority *priority )
{
  AltkWidget *view;
  view = ALTK_WIDGET(l_object_new(MBTK_CLASS_PRIORITY_VIEW, NULL));
  if (priority)
    mbtk_priority_view_set_priority(MBTK_PRIORITY_VIEW(view), priority);
  l_trash_push();
  _create_view(view);
  l_trash_pop();
  return view;
}



/* _on_value_notify:
 */
static void _on_value_notify ( MbPriority *priority,
                               MbtkPriorityView *view )
{
  Private *priv = PRIVATE(view);
  gchar text[3];
  ASSERT(priority->value > 0 && priority->value < 100);
  sprintf(text, "%02d", priority->value);
  altk_label_set_text(ALTK_LABEL(priv->value_label), text);
}



/* _on_inc_button_clicked:
 */
static void _on_inc_button_clicked ( AltkWidget *button,
                                     MbtkPriorityView *view )
{
  Private *priv = PRIVATE(view);
  ASSERT(priv->priority);
  mb_priority_request_set_value(priv->priority,
                                priv->priority->value + 1);
}



/* _on_dec_button_clicked:
 */
static void _on_dec_button_clicked ( AltkWidget *button,
                                     MbtkPriorityView *view )
{
  Private *priv = PRIVATE(view);
  ASSERT(priv->priority);
  mb_priority_request_set_value(priv->priority,
                                priv->priority->value - 1);
}



/* mbtk_priority_view_set_priority:
 */
void mbtk_priority_view_set_priority ( MbtkPriorityView *view,
                                       struct _MbPriority *priority )
{
  Private *priv = PRIVATE(view);
  if (priority == priv->priority)
    return;
  ASSERT(!priv->priority); /* [todo] */
  priv->priority = l_object_ref(priority);
  l_signal_handler_group_add
    (priv->sig_group,
     l_signal_connect(L_OBJECT(priority),
                      "notify:value",
                      (LSignalHandler) _on_value_notify,
                      view,
                      NULL));
  _on_value_notify(priority, view);
  l_signal_handler_group_add
    (priv->sig_group,
     l_signal_connect(L_OBJECT(priv->inc_button),
                      "clicked",
                      (LSignalHandler) _on_inc_button_clicked,
                      view,
                      NULL));
  l_signal_handler_group_add
    (priv->sig_group,
     l_signal_connect(L_OBJECT(priv->dec_button),
                      "clicked",
                      (LSignalHandler) _on_dec_button_clicked,
                      view,
                      NULL));
}
