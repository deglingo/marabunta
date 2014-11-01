/* mbalapp.c -
 */

#include "client-al/alprivate.h"
#include "client-al/mbalapp.h"
#include "client-al/mbtkdialog.h"
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



/* _on_game_started:
 */
static void _on_game_started ( MbcGameProxy *proxy,
                               MbalApp *app )
{
  /* setup the dialog */
  mbtk_dialog_setup(MBTK_DIALOG(app->dialog), proxy);
}



/* _run:
 */
static gint _run ( MbApp *app )
{
  /* create a display and the dialog */
  MBAL_APP(app)->display = altk_display_new(); /* [fixme] unref */
  MBAL_APP(app)->dialog = mbtk_dialog_new(MBAL_APP(app)->display);
  /* setup the game */
  mbc_app_setup_solo_game(MBC_APP(app));
  l_signal_connect(L_OBJECT(MBC_APP(app)->game_proxy),
                   "started", 0,
                   (LSignalHandler) _on_game_started,
                   app, NULL);
  /* [fixme] */
  mbs_game_start(MBC_APP(app)->game);
  /* open the display */
  altk_widget_show_all(MBAL_APP(app)->dialog);
  altk_display_open(MBAL_APP(app)->display);
  /* go */
  return MB_APP_CLASS(parent_class)->run(app);
}
