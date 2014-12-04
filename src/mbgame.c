/* mbgame.c -
 */

#include "mbgame.h"
#include "mbworld.h"
#include "mbsector.h"
#include "mbcolony.h"
#include "mbgame.inl"



/* Private:
 */
typedef struct _Private
{
  MbWorld *world;
}
  Private;

#define PRIVATE(game) ((Private *)(MB_GAME(game)->private))



/* mb_game_init:
 */
static void mb_game_init ( LObject *obj )
{
  MB_GAME(obj)->private = g_new0(Private, 1);
}



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
  Private *priv = PRIVATE(game);
  MbColony *col;
  ASSERT(!priv->world); /* [todo] */
  priv->world = mb_world_new(3, 2);
  col = mb_colony_new();
  mb_sector_set_colony(MB_WORLD_SECTOR(priv->world, 0, 0), col);
  l_object_unref(col);
}
