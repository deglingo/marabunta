/* mbsmain.c -
 */

#include "server/srvprivate.h"
#include "common/marabunta-common.h"
#include "server/mbsserver.h"
#include "server/mbsgame.h"



static void _on_server_event ( MbsServerEvent *event,
							   gpointer data )
{
  switch (event->type)
	{
	case MBS_SERVER_EVENT_ACCEPT:
	  /* CL_DEBUG("client accepted: %d", event->accept.clid); */
	  break;
	default:
	  CL_DEBUG("[TODO] server event %d", event->type);
	}
}



/* main:
 */
int main ()
{
  MbsGame *game;
  MbsServer *server;
  GMainLoop *loop;
  /* CL_DEBUG("hello!"); */
  game = mbs_game_new();
  server = mbs_server_new(_on_server_event, game);
  mbs_server_start(server);
  loop = g_main_loop_new(NULL, FALSE);
  g_main_loop_run(loop);
  return 0;
}
