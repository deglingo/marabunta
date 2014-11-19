/* mbscolony.c -
 */

#include "server/mbscolony.h"
#include "server/mbscolony.inl"



/* mbs_colony_new:
 */
MbObject *mbs_colony_new ( MbObject *game )
{
  return MB_OBJECT(l_object_new(MBS_CLASS_COLONY,
                                "game", game,
                                NULL));
}
