/* mbcworld.c -
 */

#include "client/mbcworld.h"
#include "client/mbcworld.inl"



/* mbc_world_new:
 */
MbObject *mbc_world_new ( MbObject *game,
                          MbObjectID id,
                          guint width,
                          guint height )
{
  return MB_OBJECT(l_object_new_give(MBC_CLASS_WORLD,
                                     "id", l_int_new(id),
                                     "game", l_object_ref(game),
                                     "width", l_int_new(width),
                                     "height", l_int_new(height),
                                     NULL));
}
