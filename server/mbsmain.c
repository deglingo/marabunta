/* mbsmain.c -
 */

#include "common/marabunta-common.h"
#include "server/mbsserver.h"
#include "server/mbsgame.h"



static void _on_server_event ( MbsServerEvent *event,
							   gpointer data )
{
  switch (event->type)
	{
	case MBS_SERVER_EVENT_ACCEPT:
	  printf("client accepted: %d\n", event->accept.clid);
	  break;
	default:
	  fprintf(stderr, "[TODO] server event %d\n", event->type);
	}
}



/* main:
 */
int main ()
{
  MbsGame *game;
  MbsServer *server;
  GMainLoop *loop;
  TRACE("hello!");
  game = mbs_game_new();
  server = mbs_server_new(_on_server_event, game);
  mbs_server_start(server);
  printf("marabunta-server: start...\n");
  loop = g_main_loop_new(NULL, FALSE);
  g_main_loop_run(loop);
  return 0;
}
