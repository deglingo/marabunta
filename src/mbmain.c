/* mbmain.c -
 */

#include "mbbase.h"
#include "mbgame.h"
#include "mbtkdialog.h"



/* main:
 */
gint main ()
{
  AltkWidget *dlg;
  AltkDisplay *display;
  MbGame *game;
  altk_init();
  CL_DEBUG("hello!");
  game = mb_game_new();
  mb_game_setup(game);
  display = altk_display_new();
  dlg = altk_dialog_new(display);
  altk_display_open(display);
  altk_widget_show_all(dlg);
  altk_main();
  return 0;
}
