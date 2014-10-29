/* mbcapp.c -
 */

#include "client/cliprivate.h"
#include "client/mbcapp.h"
#include "client/mbcapp.inl"

#include <sys/resource.h>



/* Signals:
 */
enum
  {
    SIG_GAME_STARTED,
    SIG_COUNT,
  };

static LSignalID signals[SIG_COUNT] = { 0, };



static gint _run ( MbcApp *app,
                   gint argc,
                   gchar **argv );
static void _setup_game ( MbcApp *app );
static void _on_client_ready ( MbcClient *client,
                               GIOCondition condition,
                               LStream *stream,
                               gpointer data );
static void _message_handler ( LptTree *tree,
                               LptClient *client,
                               LObject *message,
                               gpointer data );



/* mbc_app_class_init:
 */
static void mbc_app_class_init ( LObjectClass *cls )
{
  MBC_APP_CLASS(cls)->run = _run;
  MBC_APP_CLASS(cls)->setup_game = _setup_game;

  signals[SIG_GAME_STARTED] =
    l_signal_new(cls,
                 "game_started");
}



/* mbc_app_init:
 */
static void mbc_app_init ( LObject *object )
{
  MbcApp *app = MBC_APP(object);
  struct rlimit limit;
  limit.rlim_cur = RLIM_INFINITY;
  limit.rlim_max = RLIM_INFINITY;
  if (setrlimit(RLIMIT_CORE, &limit) < 0)
    {
      CL_DEBUG("ERROR: setrlimit() failed: %s", strerror(errno));
    }
  /* [FIXME] */
  LPT_CLASS_NSPEC_INT;
  LPT_CLASS_NSPEC_DIR;
  app->client = mbc_client_new(_on_client_ready, app);
  app->tree = lpt_tree_new();
  lpt_tree_set_message_handler(app->tree, _message_handler, app, NULL);
  app->tclient_server = lpt_tree_add_client(app->tree, "server", NULL, NULL);
}



/* run:
 */
gint mbc_app_run ( MbcApp *app,
                   gint argc,
                   gchar **argv )
{
  return MBC_APP_GET_CLASS(app)->run(app, argc, argv);
}



/* _setup_solo_game:
 */
static void _setup_solo_game ( MbcApp *app )
{
  app->game = mbs_game_new(NULL);
  app->player = mbs_game_add_player(app->game, "Player1", NULL, NULL);
}



/* _run:
 */
static gint _run ( MbcApp *app,
                   gint argc,
                   gchar **argv )
{
  GMainLoop *loop;
  /* [TEMP] */
  _setup_solo_game(app);
  loop = g_main_loop_new(NULL, FALSE);
  g_main_loop_run(loop);
  return 0;
}



/* _on_client_ready:
 */
static void _on_client_ready ( MbcClient *client,
                               GIOCondition condition,
                               LStream *stream,
                               gpointer data )
{
  MbcApp *app = MBC_APP(data);
  GError *error = NULL;
  if (condition & G_IO_IN)
    {
      LObject *msg;
      while ((msg = l_unpacker_recv(app->unpacker, &error)))
        {
          ASSERT(L_IS_TUPLE(msg));
          ASSERT(L_TUPLE_SIZE(msg) >= 1);
          ASSERT(L_IS_INT(L_TUPLE_ITEM(msg, 0)));
          switch (L_INT_VALUE(L_TUPLE_ITEM(msg, 0)))
            {
            case MB_MESSAGE_KEY_LPT_EVENT:
              ASSERT(L_TUPLE_SIZE(msg) == 2);
              lpt_tree_handle_message(app->tree, app->tclient_server, L_TUPLE_ITEM(msg, 1));
              break;
            default:
              CL_DEBUG("[TODO] message: %s", l_object_to_string(msg));
            }
          l_object_unref(msg);
        }
      if (error)
        CL_GERROR(error);
    }
  if (condition & G_IO_OUT)
    {
      gboolean r;
      r = l_packer_send(app->packer, &error);
      if (error)
        CL_ERROR("[TODO] write error");
      if (r)
        mbc_client_remove_watch(client, G_IO_OUT);
    }
}



/* _send:
 */
static void _send ( MbcApp *app,
                    LObject *msg )
{
  l_packer_add(app->packer, msg);
  mbc_client_add_watch(app->client, G_IO_OUT);
}



/* _message_handler:
 */
static void _message_handler ( LptTree *tree,
                               LptClient *client,
                               LObject *message,
                               gpointer data )
{
  MbcApp *app = data;
  LTuple *mbmsg = l_tuple_new(2);
  /* CL_DEBUG("lpt_event: %s", l_object_to_string(message)); */
  l_tuple_give_item(mbmsg, 0, L_OBJECT(l_int_new(MB_MESSAGE_KEY_LPT_EVENT)));
  l_tuple_give_item(mbmsg, 1, l_object_ref(message));
  _send(app, L_OBJECT(mbmsg));
  l_object_unref(mbmsg);
}



/* mbc_app_connect:
 */
void mbc_app_connect ( MbcApp *app )
{
  CL_DEBUG("connecting to server...");
  mbc_client_connect(app->client, "localhost", 6666);
  app->packer = l_packer_new(app->client->stream);
  app->unpacker = l_unpacker_new(app->client->stream);
}



/* _wait_tree:
 */
static gboolean _wait_tree ( MbcApp *app )
{
  LptNode *n = lpt_tree_get_node(app->tree, "/game");
  if (n)
    {
      CL_DEBUG("got game node!!");
      MBC_APP_GET_CLASS(app)->setup_game(app);
      return G_SOURCE_REMOVE;
    }
  else
    {
      return G_SOURCE_CONTINUE;
    }
}



/* mbc_app_join_game:
 */
void mbc_app_join_game ( MbcApp *app )
{
  LTuple *msg = l_tuple_newl_give(1, l_int_new(MB_MESSAGE_KEY_JOIN), NULL);
  _send(app, L_OBJECT(msg));
  l_object_unref(msg);
  lpt_tree_connect_share(app->tree, app->tclient_server, "GAME", "/game", 0);
  /* [FIXME] */
  g_timeout_add_full(0, 10, (GSourceFunc) _wait_tree, app, NULL);
}



/* _setup_game:
 */
static void _setup_game ( MbcApp *app )
{
  /* app->world = mb_world_new(); */
  /* mb_world_bind(app->world, lpt_tree_get_node(app->tree, "/game/world")); */
}
