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
  struct _MbTask *t_top;
};



/* MbColonyClass:
 */
struct _MbColonyClass
{
  MB_COLONY_CLASS_HEADER;
};



MbColony *mb_colony_new ( void );
void mb_colony_update ( MbColony *colony );



G_END_DECLS

#endif /* ifndef _MBCOLONY_H_ */
