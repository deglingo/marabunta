/* mbmain.c -
 */

#include "mbbase.h"
#include "mbgame.h"
#include "mbtkdialog.h"
#include <sys/resource.h>



/* main:
 */
gint main ()
{
  AltkWidget *dlg;
  AltkDisplay *display;
  MbGame *game;
  struct rlimit core_limit;
  core_limit.rlim_cur = RLIM_INFINITY;
  core_limit.rlim_max = RLIM_INFINITY;
  setrlimit(RLIMIT_CORE, &core_limit);
  altk_init();
  CL_DEBUG("hello!");
  game = mb_game_new();
  mb_game_setup(game);
  display = altk_display_new();
  dlg = mbtk_dialog_new(display);
  mbtk_dialog_set_game(MBTK_DIALOG(dlg), game);
  altk_widget_show_all(dlg);
  altk_display_open(display);
  mb_game_start(game);
  altk_main();
  return 0;
}
