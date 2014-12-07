/* mbcolony.h -
 */

#ifndef _MBCOLONY_H_
#define _MBCOLONY_H_

#include "mbbase.h"
#include "mbpoptype.h"
#include "mbcolony-def.h"

G_BEGIN_DECLS



/* MbColony:
 */
struct _MbColony
{
  MB_COLONY_INSTANCE_HEADER;

  struct _MbSector *sector;
  struct _MbPopTree *pop_tree;
  struct _MbPopTree *adj_tree;
  gint64 pop[MB_POP_TYPE_COUNT];
  gint64 adj_pop[MB_POP_TYPE_COUNT];
  /* tasks */
  struct _MbTask *t_top;
  struct _MbTask *t_spawn;
  struct _MbTask *t_build;
  struct _MbPriority *hatch_priority[3];
  /* rooms */
  GList *rooms;
};



/* MbColonyClass:
 */
struct _MbColonyClass
{
  MB_COLONY_CLASS_HEADER;
};



MbColony *mb_colony_new ( void );
void mb_colony_update ( MbColony *colony );
void mb_colony_adjust_pop ( MbColony *colony,
                            MbPopType type,
                            guint birthdate,
                            gint64 count );
void mb_colony_add_room ( MbColony *colony,
                          struct _MbRoom *room );
struct _MbRoom *mb_colony_get_room ( MbColony *colony,
                                     MbRoomType type );



G_END_DECLS

#endif /* ifndef _MBCOLONY_H_ */
