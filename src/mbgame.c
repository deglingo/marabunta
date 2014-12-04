/* mbgame.c -
 */

#include "mbgame.h"
#include "mbgame.inl"



/* mb_game_new:
 */
MbGame *mb_game_new ( void )
{
  return MB_GAME(l_object_new(MB_CLASS_GAME, NULL));
}



/* mb_game_setup:
 */
void mb_game_setup ( MbGame *game )
{
  CL_DEBUG("[TODO] setup game");
}
