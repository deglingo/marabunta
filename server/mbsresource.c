/* mbsresource.c -
 */

#include "server/mbsresource.h"
#include "server/mbsresource.inl"



/* mbs_resource_new:
 */
MbObject *mbs_resource_new ( MbObject *game,
                             const gchar *name,
                             MbResourceFlags flags )
{
  return MB_OBJECT(l_object_new_give(MBS_CLASS_RESOURCE,
                                     "game", l_object_ref(game),
                                     "name", l_string_new(name),
                                     "flags", l_int_new(flags),
                                     NULL));
}
