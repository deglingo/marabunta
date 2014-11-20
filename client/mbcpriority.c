/* mbcpriority.c -
 */

#include "client/mbcpriority.h"
#include "client/mbcgame.h"
#include "client/mbcpriority.inl"



/* mbc_priority_new:
 */
MbObject *mbc_priority_new ( MbObject *game,
                             MbObjectID id,
                             MbPriorityValue value )
{
  return MB_OBJECT(l_object_new_give(MBC_CLASS_PRIORITY,
                                     "id", l_int_new(id),
                                     "game", l_object_ref(game),
                                     "value", l_int_new(value),
                                     NULL));
}



/* mbc_priority_request_set_value:
 */
void mbc_priority_request_set_value ( MbcPriority *priority,
                                      MbPriorityValue value )
{
  MbState *state = mb_state_new();
  MbStateRequestPriorityValue *st_req;
  st_req = mb_state_next(state, MB_STATE_REQUEST_PRIORITY_VALUE);
  st_req->priority_id = MB_OBJECT_ID(priority);
  st_req->value = value;
  mbc_game_send_state(MBC_GAME(MB_OBJECT_GAME(priority)), state);
  l_object_unref(state);
}
