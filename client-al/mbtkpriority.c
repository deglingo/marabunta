/* mbtkpriority.c -
 */

#include "client-al/mbtkpriority.h"
#include "client-al/mbtkpriority.inl"



/* mbtk_priority_new:
 */
AltkWidget *mbtk_priority_new ( MbcProxy *proxy )
{
  MbtkPriority *prio;
  AltkWidget *box, *label, *inc, *dec;
  gchar value[3];
  prio = MBTK_PRIORITY_NEW(NULL);
  prio->proxy = l_object_ref(proxy);
  l_trash_push();
  box = L_TRASH_OBJECT
    (altk_box_new(ALTK_HORIZONTAL));
  ALTK_CONTAINER_ADD(prio, box);
  sprintf(value, "%d", MBC_PRIORITY_PROXY(proxy)->value);
  label = L_TRASH_OBJECT
    (altk_label_new(value));
  altk_box_pack_start(ALTK_BOX(box), label, 0);
  inc = L_TRASH_OBJECT
    (altk_button_new_with_label("+"));
  altk_box_pack_start(ALTK_BOX(box), inc, 0);
  dec = L_TRASH_OBJECT
    (altk_button_new_with_label("-"));
  altk_box_pack_start(ALTK_BOX(box), dec, 0);
  l_trash_pop();
  return ALTK_WIDGET(prio);
}
