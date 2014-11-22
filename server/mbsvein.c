/* mbsvein.c -
 */

#include "server/mbsvein.h"
#include "server/mbsvein.inl"



/* mbs_vein_new:
 */
MbObject *mbs_vein_new ( MbObject *game,
                         MbObject *resource,
                         gint depth,
                         gint64 qtty )
{
  return MB_OBJECT(l_object_new_give(MBS_CLASS_VEIN,
                                     "game", l_object_ref(game),
                                     "resource", l_object_ref(resource),
                                     "depth", l_int_new(depth),
                                     "qtty", l_int_new(qtty), /* [FIXME] int64 !! */
                                     NULL));
}
