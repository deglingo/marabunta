/* mbccolony.c -
 */

#include "client/mbccolony.h"
#include "client/mbccolony.inl"

#include <string.h>



/* Signals:
 */
enum
  {
    SIG_POP_NOTIFY,
    SIG_COUNT,
  };

static LSignalID signals[SIG_COUNT];



/* mbc_colony_class_init:
 */
static void mbc_colony_class_init ( LObjectClass *cls )
{
  signals[SIG_POP_NOTIFY] =
    l_signal_new(cls,
                 "pop_notify");
}



/* mbc_colony_new:
 */
MbObject *mbc_colony_new ( MbObject *game,
                           MbObjectID id )
{
  return MB_OBJECT(l_object_new_give(MBC_CLASS_COLONY,
                                     "id", l_int_new(id),
                                     "game", l_object_ref(game),
                                     NULL));
}



/* mbc_colony_set_pop:
 */
void mbc_colony_set_pop ( MbcColony *colony,
                          gint64 *pop )
{
  memcpy(colony->pop, pop, sizeof(gint64) * MB_POP_TYPE_COUNT);
  l_signal_emit(L_OBJECT(colony), signals[SIG_POP_NOTIFY], 0);
}
