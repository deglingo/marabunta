/* mbsgame.c -
 */

#include "server/srvprivate.h"
#include "server/mbsgame.h"
#include "server/mbsgame.inl"



/* Private:
 */
typedef struct _Private
{
  int dummy;
}
  Private;

#define PRIVATE(game) ((Private *)(MBS_GAME(game)->private))



/* mbs_game_init:
 */
static void mbs_game_init ( LObject *obj )
{
  MBS_GAME(obj)->private = g_new0(Private, 1);
}



/* mbs_game_new:
 */
MbObject *mbs_game_new ( void )
{
  return mb_object_new(MBS_CLASS_GAME, NULL, 0);
}
