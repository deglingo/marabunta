/* mbcresource.c -
 */

#include "client/mbcresource.h"
#include "client/mbcresource.inl"



/* mbc_resource_new:
 */
MbObject *mbc_resource_new ( MbObject *game,
                             MbObjectID id,
                             const gchar *name,
                             MbResourceFlags flags )
{
  return MB_OBJECT(l_object_new_give(MBC_CLASS_RESOURCE,
                                     "id", l_int_new(id),
                                     "game", l_object_ref(game),
                                     "name", l_string_new(name),
                                     "flags", l_int_new(flags),
                                     NULL));
}
