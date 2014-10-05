/* mbaldialog.c -
 */

#include "client-al/mbaldialog.h"



static AltkWidget *header_box_create ( void )
{
  AltkWidget *box, *lbl1, *lbl2;
  box = L_TRASH_OBJECT
    (altk_box_new(ALTK_HORIZONTAL));
  /* sim-time label */
  lbl1 = L_TRASH_OBJECT
    (altk_label_new("Time:"));
  altk_box_pack_start(ALTK_BOX(box), lbl1, 0);
  lbl2 = L_TRASH_OBJECT
    (altk_label_new("0"));
  altk_widget_set_name(lbl2, "sim-time");
  altk_box_pack_start(ALTK_BOX(box), lbl2, 0);
  /* ok */
  return box;
}



static AltkWidget *side_panel_create ( void )
{
  AltkWidget *box, *map_frame;
  box = L_TRASH_OBJECT
    (altk_box_new(ALTK_VERTICAL));
  map_frame = L_TRASH_OBJECT
    (altk_frame_new("map"));
  altk_box_pack_start(ALTK_BOX(box), map_frame, 0);
  return box;
}



static AltkWidget *body_box_create ( void )
{
  AltkWidget *box;
  box = L_TRASH_OBJECT
    (altk_box_new(ALTK_HORIZONTAL));
  altk_box_pack_start(ALTK_BOX(box), side_panel_create(), 0);
  return box;
}



/* mbal_dialog_create:
 */
AltkWidget *mbal_dialog_create ( AltkDisplay *display )
{
  AltkWidget *dlg, *top;
  l_trash_push();
  /* dialog */
  dlg = altk_dialog_new(display);
  /* top box */
  top = L_TRASH_OBJECT
    (altk_box_new(ALTK_VERTICAL));
  ALTK_CONTAINER_ADD(dlg, top);
  altk_box_pack_start(ALTK_BOX(top), header_box_create(), 0);
  altk_box_pack_start(ALTK_BOX(top), body_box_create(), 0);
  /* cleanup */
  l_trash_pop();
  return dlg;
}
