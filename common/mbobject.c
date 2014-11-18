/* mbobject.c -
 */

#include "common/private.h"
#include "common/mbobject.h"
#include "common/mbgame.h"
#include "common/mbobject.inl"



/* mb_object_new:
 */
MbObject *mb_object_new ( LObjectClass *cls,
                          MbObject *game,
                          MbObjectID id )
{
  MbObject *obj;
  ASSERT(l_object_issubclass(L_OBJECT(cls), L_OBJECT(MB_CLASS_OBJECT)));
  obj = MB_OBJECT(l_object_new(cls, NULL));
  if (game) {
    ASSERT(MB_IS_GAME(game));
  } else {
    ASSERT(MB_IS_GAME(obj));
    game = obj;
  }
  if (id == 0)
    id = mb_game_next_id(MB_GAME(game));
  obj->id = id;
  mb_game_register_object(MB_GAME(game), obj);
  return obj;
}
