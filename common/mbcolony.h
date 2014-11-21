/* mbcolony.h -
 */

#ifndef _MBCOLONY_H_
#define _MBCOLONY_H_

#include "common/mbobject.h"
#include "common/mbcolony-def.h"

G_BEGIN_DECLS



/* MbColony:
 */
struct _MbColony
{
  MB_COLONY_INSTANCE_HEADER;

  /* set by MbSector */
  MbObject *sector;
  /* owner player */
  MbObject *owner;
  /* top task group */
  MbObject *top_task;
  /* rooms */
  MbObject *rooms[MB_ROOM_TYPE_COUNT];
  /* stock */
  GPtrArray *stock;
};



/* MbColonyClass:
 */
struct _MbColonyClass
{
  MB_COLONY_CLASS_HEADER;
};



#define MB_COLONY_SECTOR(c) (MB_COLONY(c)->sector)
#define MB_COLONY_OWNER(c) (MB_COLONY(c)->owner)
#define MB_COLONY_TOP_TASK(c) (MB_COLONY(c)->top_task)
#define MB_COLONY_ROOM(c, tp) (MB_COLONY(c)->rooms[tp])

void mb_colony_set_owner ( MbColony *colony,
                           MbObject *player );
void mb_colony_set_top_task ( MbColony *colony,
                              MbObject *task );
void mb_colony_add_room ( MbColony *colony,
                          MbObject *room );
void mb_colony_set_stock ( MbColony *colony,
                           gint rsc_index,
                           gint64 qtty );
void mb_colony_add_stock ( MbColony *colony,
                           gint rsc_index,
                           gint64 qtty );
gint64 mb_colony_get_stock ( MbColony *colony,
                             gint rsc_index );



G_END_DECLS

#endif /* ifndef _MBCOLONY_H_ */
