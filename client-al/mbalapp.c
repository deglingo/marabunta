/* mbalapp.c -
 */

#include "client-al/alprivate.h"
#include "client-al/mbalapp.h"
#include "client-al/mbaldialog.h"
#include "client-al/mbalapp.inl"



extern const char DIALOG_MAIN[];

static gint _run ( MbcApp *app,
                   gint argc,
                   gchar **argv );
static void _setup_game ( MbcApp *app );



/* mbal_app_class_init:
 */
static void mbal_app_class_init ( LObjectClass *cls )
{
  MBC_APP_CLASS(cls)->run = _run;
  MBC_APP_CLASS(cls)->setup_game = _setup_game;
}



/* mbal_app_new:
 */
MbalApp *mbal_app_new ( void )
{
  MbalApp *app = MBAL_APP(l_object_new(MBAL_CLASS_APP, NULL));
  return app;
}



/* _run:
 */
static gint _run ( MbcApp *app,
                   gint argc,
                   gchar **argv )
{
  AltkDisplay *display;
  altk_init();
  display = altk_display_new();
  MBAL_APP(app)->dialog = mbal_dialog_create(display);
  altk_widget_show_all(MBAL_APP(app)->dialog);
  altk_display_open(display);
  mbc_app_connect(MBC_APP(app));
  mbc_app_join_game(MBC_APP(app));
  return MBC_APP_CLASS(parent_class)->run(app, argc, argv);
}



/* _setup_game:
 */
static void _setup_game ( MbcApp *app )
{
  MBC_APP_CLASS(parent_class)->setup_game(app);
  mbal_dialog_setup_game(MBAL_APP(app)->dialog, app->tree);
}
