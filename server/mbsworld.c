/* mbsworld.c -
 */

#include "server/mbsworld.h"
#include "server/mbsworld.inl"



/* mbs_world_new:
 */
MbObject *mbs_world_new ( MbObject *game,
                          guint width,
                          guint height )
{
  return MB_OBJECT(l_object_new_give(MBS_CLASS_WORLD,
                                     "game", l_object_ref(game),
                                     /* "width", l_int_new(width), */
                                     /* "height", l_int_new(height), */
                                     NULL));
}
