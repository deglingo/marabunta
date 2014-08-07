/* mbsmain.c -
 */

#include <stdio.h>

#include "server/mbsserver.h"
#include "server/mbsgame.h"



/* main:
 */
int main ()
{
  MbsGame *game;
  MbsServer *server;
  printf("marabunta-server: hello!\n");
  game = mbs_game_new();
  server = mbs_server_new();
  return 0;
}
