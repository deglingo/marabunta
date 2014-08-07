/* mbsmain.c -
 */

#include "server/mbsserver.h"
#include "server/mbsgame.h"



/* main:
 */
int main ()
{
  MbsGame *game;
  MbsServer *server;
  GMainLoop *loop;
  printf("marabunta-server: hello!\n");
  game = mbs_game_new();
  server = mbs_server_new();
  mbs_server_start(server);
  loop = g_main_loop_new(NULL, FALSE);
  g_main_loop_run(loop);
  return 0;
}
