/* mbcolony.c -
 */

#include "common/private.h"
#include "common/mbcolony.h"
#include "common/mbgame.h"
#include "common/mbplayer.h"
#include "common/mbresource.h"
#include "common/mbtask.h"
#include "common/mbcolony.inl"



/* Signals:
 */
enum
  {
    SIG_STOCK_NOTIFY,
    SIG_COUNT,
  };

static LSignalID signals[SIG_COUNT];



/* mb_colony_class_init:
 */
static void mb_colony_class_init ( LObjectClass *cls )
{
  l_signal_new(cls,
               "stock_notify");
}



/* mb_colony_init:
 */
static void mb_colony_init ( LObject *obj )
{
  MB_COLONY(obj)->stock = g_ptr_array_new();
}



/* mb_colony_set_owner:
 */
void mb_colony_set_owner ( MbColony *colony,
                           MbObject *player )
{
  ASSERT(MB_IS_PLAYER(player));
  ASSERT(!colony->owner); /* [TODO] */
  colony->owner = l_object_ref(player);
}



/* mb_colony_set_top_task:
 */
void mb_colony_set_top_task ( MbColony *colony,
                              MbObject *task )
{
  ASSERT(MB_IS_TASK(task));
  ASSERT(!colony->top_task);
  ASSERT(!MB_TASK_COLONY(task));
  colony->top_task = l_object_ref(task);
  MB_TASK(task)->colony = MB_OBJECT(colony);
}



/* mb_colony_add_room:
 */
void mb_colony_add_room ( MbColony *colony,
                          MbObject *room )
{
  ASSERT(MB_IS_ROOM(room));
  ASSERT(!MB_ROOM(room)->colony);
  ASSERT(!colony->rooms[MB_ROOM_TYPE(room)]);
  colony->rooms[MB_ROOM_TYPE(room)] = l_object_ref(room);
  MB_ROOM(room)->colony = MB_OBJECT(colony);
}



/* mb_colony_set_stock:
 */
void mb_colony_set_stock ( MbColony *colony,
                           gint rsc_index,
                           gint64 qtty )
{
  MbObject *rsc;
  ASSERT(rsc_index >= 0);
  if (mb_colony_get_stock(colony, rsc_index) == qtty)
    return;
  rsc = mb_game_get_resource(MB_GAME(MB_OBJECT_GAME(colony)), rsc_index);
  ASSERT(rsc);
  if (colony->stock->len <= rsc_index)
    g_ptr_array_set_size(colony->stock, rsc_index+1);
  if (!colony->stock->pdata[rsc_index])
    colony->stock->pdata[rsc_index] = g_new(gint64, 1);
  *((gint64 *)(colony->stock->pdata[rsc_index])) = qtty;
  l_signal_emit(L_OBJECT(colony),
                signals[SIG_STOCK_NOTIFY],
                MB_RESOURCE_QNAME(rsc));
}



/* mb_colony_add_stock:
 */
void mb_colony_add_stock ( MbColony *colony,
                           gint rsc_index,
                           gint64 qtty )
{
  ASSERT(rsc_index >= 0);
  mb_colony_set_stock(colony, rsc_index,
                      mb_colony_get_stock(colony, rsc_index) + qtty);
}



/* mb_colony_get_stock:
 */
gint64 mb_colony_get_stock ( MbColony *colony,
                             gint rsc_index )
{
  ASSERT(rsc_index >= 0);
  if (rsc_index < colony->stock->len && colony->stock->pdata[rsc_index])
    return *((gint64 *)(colony->stock->pdata[rsc_index]));
  else
    return 0;
}
