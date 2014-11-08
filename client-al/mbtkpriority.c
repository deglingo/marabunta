/* mbtkpriority.c -
 */

#include "client-al/mbtkpriority.h"
#include "client-al/mbtkpriority.inl"



static void _on_inc_clicked ( AltkWidget *button,
                              AltkWidget *prio )
{
  mbc_priority_proxy_request_inc_value
    (MBC_PRIORITY_PROXY(MBTK_PRIORITY(prio)->proxy));
}



static void _on_dec_clicked ( AltkWidget *button,
                              AltkWidget *prio )
{
  mbc_priority_proxy_request_dec_value
    (MBC_PRIORITY_PROXY(MBTK_PRIORITY(prio)->proxy));
}



static void _on_value_notify ( MbcPriorityProxy *proxy,
                               MbtkPriority *widget )
{
  gchar text[12];
  sprintf(text, "%d", proxy->value);
  altk_label_set_text(ALTK_LABEL(widget->label), text);
}



/* mbtk_priority_new:
 */
AltkWidget *mbtk_priority_new ( MbcProxy *proxy )
{
  MbtkPriority *prio;
  AltkWidget *box, *inc, *dec;
  gchar value[3];
  prio = MBTK_PRIORITY_NEW(NULL);
  prio->proxy = l_object_ref(proxy);
  l_trash_push();
  box = L_TRASH_OBJECT
    (altk_box_new(ALTK_HORIZONTAL));
  ALTK_CONTAINER_ADD(prio, box);
  sprintf(value, "%d", MBC_PRIORITY_PROXY(proxy)->value);
  prio->label = L_TRASH_OBJECT
    (altk_label_new(value));
  altk_box_pack_start(ALTK_BOX(box), prio->label, 0);
  inc = L_TRASH_OBJECT
    (altk_button_new_with_label("+"));
  altk_box_pack_start(ALTK_BOX(box), inc, 0);
  l_signal_connect(L_OBJECT(inc), "clicked", 0,
                   (LSignalHandler) _on_inc_clicked,
                   prio, NULL);
  dec = L_TRASH_OBJECT
    (altk_button_new_with_label("-"));
  altk_box_pack_start(ALTK_BOX(box), dec, 0);
  l_signal_connect(L_OBJECT(dec), "clicked", 0,
                   (LSignalHandler) _on_dec_clicked,
                   prio, NULL);
  l_signal_connect(L_OBJECT(proxy),
                   "notify", g_quark_from_string("value"),
                   (LSignalHandler) _on_value_notify,
                   prio, NULL);
  l_trash_pop();
  return ALTK_WIDGET(prio);
}
