/* mbsmain.c -
 */

#include <stdio.h>

#include "server/mbsgame.h"



/* main:
 */
int main ()
{
  MbsGame *game;
  printf("marabunta-server: hello!\n");
  game = mbs_game_new();
  return 0;
}
