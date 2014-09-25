/* mbsmain.c -
 */

#include "server/srvprivate.h"
#include "common/marabunta-common.h"
#include "server/mbsserver.h"
#include "server/mbsgame.h"



/* MainData:
 */
typedef struct _MainData
{
  MbsServer *server;
  MbsGame *game;
  GMainLoop *loop;
  GHashTable *client_map;
}
  MainData;



/* Client:
 */
typedef struct _Client
{
  guint clid;
  MbMessageReader *reader;
  LPacker *packer;
  LUnpacker *unpacker;
}
  Client;



static gboolean _on_client_read ( MbsServer *server,
                                  guint clid,
                                  LStream *stream,
                                  gpointer data,
                                  GError **error )
{
  gchar buf[65536];
  LStreamStatus s;
  gint64 w;
  s = l_stream_read(stream, buf, 65536, &w, error);
  switch (s) {
  case L_STREAM_STATUS_OK:
    CL_DEBUG("got %d bytes from client %d", (gint) w, clid);
    break;
  case L_STREAM_STATUS_EOF:
    CL_DEBUG("[TODO] got EOF from client %d", clid);
    return G_SOURCE_REMOVE;
  default:
    CL_ERROR("[TODO] s = %d", s);
  }
  return G_SOURCE_CONTINUE;
}



static void _process_read ( MainData *data,
                            Client *client )
{
  LObject *msg;
  GError *err = NULL;
  while ((msg = l_unpacker_recv(client->unpacker, &err)))
    {
      CL_DEBUG("[TODO] handle message: %s", l_object_to_string(msg));
      l_object_unref(msg);
    }
  if (err) {
    CL_DEBUG("[TODO] read error");
  }
}



static void _process_write ( MainData *data,
                             Client *client )
{
  CL_DEBUG("[TODO] process write...");
}



static void _on_server_event ( MbsServerEvent *event,
							   MainData *data )
{
  switch (event->type)
	{
	case MBS_SERVER_EVENT_ACCEPT:
      {
        Client *client = g_new0(Client, 1);
        CL_DEBUG("client accepted: %d", event->accept.clid);
        client->clid = event->accept.clid;
        client->packer = l_packer_new(event->accept.stream);
        client->unpacker = l_unpacker_new(event->accept.stream);
        g_hash_table_insert(data->client_map, GUINT_TO_POINTER(client->clid), client);
        mbs_server_add_watch(data->server, event->accept.clid, G_IO_IN);
      }
	  break;
    case MBS_SERVER_EVENT_READY:
      {
        Client *client = g_hash_table_lookup(data->client_map, GUINT_TO_POINTER(event->ready.clid));
        ASSERT(client);
        if (event->ready.condition & G_IO_IN)
          _process_read(data, client);
        if (event->ready.condition & G_IO_OUT)
          _process_write(data, client);
      }
      break;
    /* case MBS_SERVER_EVENT_MESSAGE: */
    /*   switch (event->message.message->key) */
    /*     { */
    /*     case MB_MESSAGE_KEY_JOIN: */
    /*       mbs_game_add_player(data->game, event->message.clid, "player1"); */
    /*       CL_DEBUG("client %d joined the game", event->message.clid); */
    /*       /\* [FIXME] *\/ */
    /*       mbs_game_start(data->game); */
    /*       break; */
    /*     default: */
    /*       CL_DEBUG("[TODO] messsage: %d", event->message.message->key); */
    /*     } */
    /*   break; */
	default:
	  CL_DEBUG("[TODO] server event %d", event->type);
	}
}



/* main:
 */
int main ()
{
  MainData *data;
  /* CL_DEBUG("hello!"); */
  data = g_new0(MainData, 1);
  data->client_map = g_hash_table_new(NULL, NULL);
  data->game = mbs_game_new();
  data->server = mbs_server_new((MbsServerHandler) _on_server_event, data);
  mbs_server_start(data->server);
  data->loop = g_main_loop_new(NULL, FALSE);
  g_main_loop_run(data->loop);
  return 0;
}
