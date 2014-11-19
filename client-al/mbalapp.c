/* mbalapp.c -
 */

#include "client-al/alprivate.h"
#include "client-al/mbalapp.h"
#include "client-al/mbtkdialog.h"
#include "client-al/mbalapp.inl"



static gint run ( MbApp *app );



/* mbal_app_class_init:
 */
static void mbal_app_class_init ( LObjectClass *cls )
{
  MB_APP_CLASS(cls)->run = run;
}



/* mbal_app_new:
 */
MbApp *mbal_app_new ( void )
{
  MbApp *app = MB_APP(l_object_new(MBAL_CLASS_APP, NULL));
  return app;
}



/* run:
 */
static gint run ( MbApp *app )
{
  /* create a display and the dialog */
  MBAL_APP(app)->display = altk_display_new(); /* [fixme] unref */
  MBAL_APP(app)->dialog = mbtk_dialog_new(MBAL_APP(app)->display);
  altk_widget_show_all(MBAL_APP(app)->dialog);
  altk_display_open(MBAL_APP(app)->display);
  /* setup and start the game */
  mbc_app_setup_solo_game(MBC_APP(app));
  mbs_game_start(MBS_GAME(MBC_APP(app)->game));
  /* go */
  return MB_APP_CLASS(parent_class)->run(app);
}
