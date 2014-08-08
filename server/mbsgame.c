/* mbsgame.c -
 */

#include "server/mbsgame.h"



/* mbs_game_new:
 */
MbsGame *mbs_game_new ( void )
{
  MbsGame *game;
  game = g_new0(MbsGame, 1);
  return game;
}
