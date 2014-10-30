/* mbaldialog.c -
 */

#include "client-al/alprivate.h"
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
  altk_widget_set_name(map_frame, "map-frame");
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



static AltkWidget *map_create ( void )
{
  guint width=3, height=2;
  guint x, y;
  AltkWidget *table;
  table = L_TRASH_OBJECT
    (altk_table_new());
  for (y = 0; y < height; y++)
    {
      for (x = 0; x < width; x++)
        {
          gchar *label = L_TRASH_GPOINTER
            (g_strdup_printf("[%d,%d]", x, y));
          AltkWidget *sector = L_TRASH_OBJECT
            (altk_button_new_with_label(label));
          altk_table_attach(ALTK_TABLE(table),
                            sector,
                            y, x, y+1, x+1, 0);
        }
    }
  altk_widget_show_all(table);
  return table;
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



/* mbal_dialog_setup_game:
 */
void mbal_dialog_setup_game ( AltkWidget *dialog )
{
  AltkWidget *map_frame;
  l_trash_push();
  /* map */
  map_frame = altk_widget_find(dialog, "map-frame");
  ALTK_CONTAINER_ADD(map_frame, map_create());
  /* cleanup */
  l_trash_pop();
}
