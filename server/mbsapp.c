/* mbsmain.c -
 */

#include "server/srvprivate.h"
#include "common/marabunta-common.h"
#include "server/mbsapp.h"
#include "server/mbsserver.h"
#include "server/mbsgame.h"
#include "server/mbsapp.inl"



/* Client:
 */
typedef struct _Client
{
  MbsApp *app;
  MbsClientID clid;
  MbsPlayerID playerid;
  MbMessageReader *reader;
  LPacker *packer;
  LUnpacker *unpacker;
}
  Client;



/* mbs_app_new:
 */
MbsApp *mbs_app_new ( void )
{
  return MBS_APP(l_object_new(MBS_CLASS_APP, NULL));
}



static void _process_message ( MbsApp *app,
                               Client *cli,
                               LObject *msg )
{
  gint key;
  ASSERT(L_IS_TUPLE(msg));
  ASSERT(L_TUPLE_SIZE(msg) >= 1);
  ASSERT(L_IS_INT(L_TUPLE_ITEM(msg, 0)));
  key = L_INT_VALUE(L_TUPLE_ITEM(msg, 0));
  switch (key)
    {
    case MB_MESSAGE_KEY_JOIN:
      ASSERT(cli->playerid == 0);
      cli->playerid = mbs_game_add_player(app->game, "player", cli, NULL); /* [fixme] name */
      CL_DEBUG("client %p joined the game (player %p)",
               cli->clid, cli->playerid);
      /* [FIXME] */
      mbs_game_start(app->game);
      break;
    case MB_MESSAGE_KEY_LPT_EVENT:
      mbs_game_lpt_event(app->game, cli->playerid, L_TUPLE_ITEM(msg, 1));
      break;
    default:
      CL_DEBUG("ERROR: unknown message key: %d", key);
    }
}



static void _process_read ( MbsApp *app,
                            Client *client )
{
  LObject *msg;
  GError *err = NULL;
  while ((msg = l_unpacker_recv(client->unpacker, &err)))
    {
      _process_message(app, client, msg);
      l_object_unref(msg);
    }
  if (err) {
    if (err->domain == L_PACK_ERROR && err->code == L_PACK_ERROR_EOF)
      {
        CL_DEBUG("[TODO] got EOF from client %p", client->clid);
        mbs_server_remove_watch(app->server, client->clid, G_IO_IN | G_IO_OUT);
      }
    else
      {
        CL_GERROR(err);
        abort();
      }
  }
}



static void _process_write ( MbsApp *app,
                             Client *client )
{
  gboolean r;
  GError *err = NULL;
  r = l_packer_send(client->packer, &err);
  ASSERT(!err);
  if (r) {
    mbs_server_remove_watch(app->server, client->clid, G_IO_OUT);
  }
}



/* _on_server_event:
 */
static void _on_server_event ( MbsServerEvent *event,
							   MbsApp *app )
{
  switch (event->type)
	{
	case MBS_SERVER_EVENT_ACCEPT:
      {
        Client *client = g_new0(Client, 1);
        CL_DEBUG("client accepted: %p", event->accept.client);
        client->app = app;
        client->clid = event->accept.client;
        mbs_client_set_data(client->clid, client, NULL);
        client->packer = l_packer_new(event->accept.stream);
        client->unpacker = l_unpacker_new(event->accept.stream);
        mbs_server_add_watch(app->server, client->clid, G_IO_IN);
      }
	  break;
    case MBS_SERVER_EVENT_READY:
      {
        Client *client = mbs_client_get_data(event->ready.client);
        ASSERT(client);
        if (event->ready.condition & G_IO_IN) {
          /* CL_DEBUG("client ready (in) : %p", client->clid); */
          _process_read(app, client);
        }
        if (event->ready.condition & G_IO_OUT) {
          /* CL_DEBUG("client ready (out) : %p", client->clid); */
          _process_write(app, client);
        }
      }
      break;
	default:
	  CL_DEBUG("[TODO] server event %d", event->type);
	}
}



/* mbs_app_send:
 */
void mbs_app_send ( MbsApp *app,
                    guint clid,
                    LObject *msg )
{
  CL_ERROR("[TODO]");
  /* Client *client; */
  /* client = g_hash_table_lookup(app->client_map, GUINT_TO_POINTER(clid)); */
  /* ASSERT(client); */
  /* l_packer_add(client->packer, msg); */
  /* mbs_server_add_watch(app->server, clid, G_IO_OUT); */
}



/* /\* main: */
/*  *\/ */
/* gint main ( gint argc, */
/*             gchar **argv ) */
/* { */
/*   MbsApp *app; */
/*   gint r; */
/*   los_init(); */
/*   app = mbs_app_new(); */
/*   r = mbs_app_main(app, argc, argv); */
/*   l_object_unref(app); */
/*   return r; */
/* } */



/* _send:
 */
static void _send ( MbsApp *app,
                    Client *client,
                    LObject *packet )
{
  l_packer_add(client->packer, packet);
  mbs_server_add_watch(app->server, client->clid, G_IO_OUT);
}



/* _lptree_message_handler:
 */
static void _lptree_message_handler ( LptTree *tree,
                                      LObject *message,
                                      gpointer player_data )
{
  Client *client = player_data;
  LTuple *packet = l_tuple_new(2);
  l_tuple_give_item(packet, 0, L_OBJECT(l_int_new(MB_MESSAGE_KEY_LPT_EVENT)));
  l_tuple_give_item(packet, 1, l_object_ref(message));
  _send(client->app, client, L_OBJECT(packet));
  l_object_unref(packet);
}



/* mbs_app_main:
 */
gint mbs_app_main ( MbsApp *app,
                    gint argc,
                    gchar **argv )
{
  lptree_init();
  /* init */
  app->loop = g_main_loop_new(NULL, FALSE);
  app->server = mbs_server_new
    ((MbsServerHandler) _on_server_event, app);
  app->game = mbs_game_new(_lptree_message_handler);
  /* start */
  mbs_server_start(app->server);
  g_main_loop_run(app->loop);
  return 0;
}
