/* mbcolony.c -
 */

#include "common/private.h"
#include "common/mbcolony.h"
#include "common/mbsector.h"
#include "common/mbgame.h"
#include "common/mbplayer.h"
#include "common/mbresource.h"
#include "common/mbtask.h"
#include "common/mbcolony.inl"



/* Signals:
 */
enum
  {
    SIG_ADD_ROOM,
    SIG_TASK_ADDED,
    SIG_POP_NOTIFY,
    SIG_STOCK_NOTIFY,
    SIG_COUNT,
  };

static LSignalID signals[SIG_COUNT];



/* StockNode:
 */
typedef struct _StockNode
{
  MbObject *rsc;
  gint64 qtty;
}
  StockNode;



static void set_sector ( MbColony *colony,
                         MbObject *sector );
static void add_room ( MbColony *colony,
                       MbObject *room );



static void stock_node_free ( StockNode *node )
{
  g_free(node);
}



/* mb_colony_class_init:
 */
static void mb_colony_class_init ( LObjectClass *cls )
{
  MB_COLONY_CLASS(cls)->set_sector = set_sector;
  MB_COLONY_CLASS(cls)->add_room = add_room;

  signals[SIG_ADD_ROOM] =
    l_signal_new(cls,
                 "add_room",
                 0,
                 0,
                 NULL,
                 NULL,
                 NULL,
                 NULL);

  signals[SIG_TASK_ADDED] =
    l_signal_new(cls,
                 "task_added",
                 0,
                 0,
                 NULL,
                 NULL,
                 NULL,
                 NULL);
  
  signals[SIG_POP_NOTIFY] =
    l_signal_new(cls,
                 "pop_notify",
                 0,
                 0,
                 NULL,
                 NULL,
                 NULL,
                 NULL);

  signals[SIG_STOCK_NOTIFY] =
    l_signal_new(cls,
                 "stock_notify",
                 0,
                 0,
                 NULL,
                 NULL,
                 NULL,
                 NULL);
}



/* mb_colony_init:
 */
static void mb_colony_init ( LObject *obj )
{
  MB_COLONY(obj)->stock =
    g_hash_table_new_full(NULL,
                          NULL,
                          NULL,
                          (GDestroyNotify) stock_node_free);
}



/* mb_colony_set_sector:
 */
void mb_colony_set_sector ( MbColony *colony,
                            MbObject *sector )
{
  ASSERT(!colony->sector);
  ASSERT(MB_IS_SECTOR(sector));
  ASSERT(MB_SECTOR_COLONY(sector) == MB_OBJECT(colony));
  MB_COLONY_GET_CLASS(colony)->set_sector(colony, sector);
}



/* set_sector:
 */
static void set_sector ( MbColony *colony,
                         MbObject *sector )
{
  colony->sector = sector;
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
  mb_task_set_colony(MB_TASK(task), MB_OBJECT(colony));
  mb_colony_task_added(colony, task);
}



/* mb_colony_task_added:
 */
void mb_colony_task_added ( MbColony *colony,
                            MbObject *task )
{
  /* [FIXME] param */
  l_signal_emit(L_OBJECT(colony),
                signals[SIG_TASK_ADDED],
                0,
                NULL);
}



/* mb_colony_add_room:
 */
void mb_colony_add_room ( MbColony *colony,
                          MbObject *room )
{
  ASSERT(MB_IS_ROOM(room));
  ASSERT(!MB_ROOM(room)->colony);
  /* [FIXME] check room type */
  /* [fixme] signal method */
  MB_COLONY_GET_CLASS(colony)->add_room(colony, room);
  l_signal_emit(L_OBJECT(colony),
                signals[SIG_ADD_ROOM],
                0,
                NULL);
}



/* add_room:
 */
static void add_room ( MbColony *colony,
                       MbObject *room )
{
  colony->rooms = g_list_append(colony->rooms, l_object_ref(room));
  mb_room_set_colony(MB_ROOM(room), MB_OBJECT(colony));
}



/* mb_colony_pop_notify:
 */
void mb_colony_pop_notify ( MbColony *colony )
{
  l_signal_emit(L_OBJECT(colony), signals[SIG_POP_NOTIFY], 0, NULL);
}



/* mb_colony_set_stock:
 */
void mb_colony_set_stock ( MbColony *colony,
                           MbObjectID rscid,
                           gint64 qtty )
{
  StockNode *node;
  if ((node = g_hash_table_lookup(colony->stock, GUINT_TO_POINTER(rscid))))
    {
      if (node->qtty == qtty)
        return;
    }
  else
    {
      if (qtty == 0)
        return;
      node = g_new(StockNode, 1);
      node->rsc = mb_game_lookup_object(MB_GAME(MB_OBJECT_GAME(colony)), rscid); /* [fixme] ref ? */
      ASSERT(node->rsc && MB_IS_RESOURCE(node->rsc));
      g_hash_table_insert(colony->stock, GUINT_TO_POINTER(rscid), node);
    }
  node->qtty = qtty;
  l_signal_emit(L_OBJECT(colony),
                signals[SIG_STOCK_NOTIFY],
                MB_RESOURCE_QNAME(node->rsc),
                NULL);
}



/* mb_colony_add_stock:
 */
void mb_colony_add_stock ( MbColony *colony,
                           MbObjectID rscid,
                           gint64 qtty )
{
  /* [FIXME] */
  mb_colony_set_stock(colony,
                      rscid,
                      mb_colony_get_stock(colony, rscid) + qtty);
}



/* mb_colony_get_stock:
 */
gint64 mb_colony_get_stock ( MbColony *colony,
                             MbObjectID rscid )
{
  StockNode *node;
  if ((node = g_hash_table_lookup(colony->stock, GUINT_TO_POINTER(rscid))))
    {
      return node->qtty;
    }
  else
    {
      return 0;
    }
}



/* mb_colony_stock_iter_init:
 */
void mb_colony_stock_iter_init ( MbColonyStockIter *iter,
                                 MbColony *colony )
{
  g_hash_table_iter_init(&iter->hash_iter, colony->stock);
}



/* mb_colony_stock_iter_next:
 */
gboolean mb_colony_stock_iter_next ( MbColonyStockIter *iter,
                                     MbObject **resource,
                                     gint64 *qtty )
{
  gpointer key, value;
  if (g_hash_table_iter_next(&iter->hash_iter, &key, &value))
    {
      *resource = ((StockNode *) value)->rsc;
      *qtty = ((StockNode *) value)->qtty;
      return TRUE;
    }
  else
    {
      return FALSE;
    }
}



/* mb_colony_stock_size:
 */
gint mb_colony_stock_size ( MbColony *colony )
{
  return g_hash_table_size(colony->stock);
}
