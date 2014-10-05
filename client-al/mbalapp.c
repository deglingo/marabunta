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



/* mbal_app_class_init:
 */
static void mbal_app_class_init ( LObjectClass *cls )
{
  MBC_APP_CLASS(cls)->run = _run;
}



/* mbal_app_new:
 */
MbalApp *mbal_app_new ( void )
{
  MbalApp *app = MBAL_APP(l_object_new(MBAL_CLASS_APP, NULL));
  return app;
}



/* _wait_tree:
 */
static gboolean _wait_tree ( MbalApp *app )
{
  LptNode *n = lpt_tree_get_node(MBC_APP(app)->tree, "/game");
  if (n)
    {
      CL_DEBUG("got game node!!");
      mbal_dialog_setup_game(app->dialog, MBC_APP(app)->tree);
      return G_SOURCE_REMOVE;
    }
  else
    {
      return G_SOURCE_CONTINUE;
    }
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
  /* [FIXME] */
  g_timeout_add_full(0, 10, (GSourceFunc) _wait_tree, app, NULL);
  return MBC_APP_CLASS(parent_class)->run(app, argc, argv);
}
