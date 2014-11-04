/* mbsobject.c -
 */

#include "server/mbsobject.h"
#include "server/mbsobject.inl"



static MbsObjectID id_counter = 1;



/* mbs_object_init:
 */
static void mbs_object_init ( LObject *obj )
{
  MBS_OBJECT(obj)->id = g_atomic_int_add(&id_counter, 1);
}
