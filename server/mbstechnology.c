/* mbstechnology.c -
 */

#include "server/mbstechnology.h"
#include "server/mbstechnology.inl"



/* mbs_technology_new:
 */
MbObject *mbs_technology_new ( MbObject *game,
                               const gchar *name )
{
  return MB_OBJECT(l_object_new_give(MBS_CLASS_TECHNOLOGY,
                                     "game", l_object_ref(game),
                                     "name", l_string_new(name),
                                     NULL));
}
