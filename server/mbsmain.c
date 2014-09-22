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
}
  MainData;



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



static void _on_server_event ( MbsServerEvent *event,
							   MainData *data )
{
  switch (event->type)
	{
	case MBS_SERVER_EVENT_ACCEPT:
	  CL_DEBUG("client accepted: %d", event->accept.clid);
      mbs_server_add_watch(data->server, event->accept.clid, G_IO_IN);
	  break;
    case MBS_SERVER_EVENT_MESSAGE:
      switch (event->message.message->key)
        {
        case MB_MESSAGE_KEY_JOIN:
          mbs_game_add_player(data->game, event->message.clid, "player1");
          CL_DEBUG("client %d joined the game", event->message.clid);
          /* [FIXME] */
          mbs_game_start(data->game);
          break;
        default:
          CL_DEBUG("[TODO] messsage: %d", event->message.message->key);
        }
      break;
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
  data->game = mbs_game_new();
  data->server = mbs_server_new((MbsServerHandler) _on_server_event, data);
  mbs_server_start(data->server);
  data->loop = g_main_loop_new(NULL, FALSE);
  g_main_loop_run(data->loop);
  return 0;
}
