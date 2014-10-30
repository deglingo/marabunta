/* mbalapp.c -
 */

#include "client-al/alprivate.h"
#include "client-al/mbalapp.h"
#include "client-al/mbaldialog.h"
#include "client-al/mbalapp.inl"



extern const char DIALOG_MAIN[];

static void _setup ( MbApp *app );
static gint _run ( MbApp *app );



/* mbal_app_class_init:
 */
static void mbal_app_class_init ( LObjectClass *cls )
{
  MB_APP_CLASS(cls)->setup = _setup;
  MB_APP_CLASS(cls)->run = _run;
}



/* mbal_app_new:
 */
MbApp *mbal_app_new ( void )
{
  MbApp *app = MB_APP(l_object_new(MBAL_CLASS_APP, NULL));
  return app;
}



/* _setup:
 */
static void _setup ( MbApp *app )
{
}



/* _run:
 */
static gint _run ( MbApp *app )
{
  /* create a display and the dialog */
  MBAL_APP(app)->display = altk_display_new(); /* [fixme] unref */
  MBAL_APP(app)->dialog = mbal_dialog_create(MBAL_APP(app)->display);
  /* setup the game */
  mbc_app_setup_solo_game(MBC_APP(app));
  /* setup the dialog */
  mbal_dialog_setup(MBAL_APP(app)->dialog, MBAL_APP(app));
  /* [fixme] */
  mbs_game_start(MBC_APP(app)->game);
  /* open the display */
  altk_widget_show_all(MBAL_APP(app)->dialog);
  altk_display_open(MBAL_APP(app)->display);
  /* go */
  return MB_APP_CLASS(parent_class)->run(app);
}
