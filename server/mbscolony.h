/* mbscolony.h -
 */

#ifndef _MBSCOLONY_H_
#define _MBSCOLONY_H_

#include "server/mbsbase.h"
#include "server/mbstask.h"
#include "server/mbscolony-def.h"

G_BEGIN_DECLS

struct _MbsSector;



/* MbsColony:
 */
struct _MbsColony
{
  MBS_COLONY_INSTANCE_HEADER;

  struct _MbsSector *sector;
  guint owner;
  MbPopTree *pop_tree;
  MbPopTree *pop_adj;
  GList *tasks;
  /* pop cast priorities */
  MbPriority *prio_pop_queen;
  MbPriority *prio_pop_worker;
  MbPriority *prio_pop_soldier;
  gint64 score_pop_queen;
  gint64 score_pop_worker;
  gint64 score_pop_soldier;
};



/* MbsColonyClass:
 */
struct _MbsColonyClass
{
  MBS_COLONY_CLASS_HEADER;
};



MbsColony *mbs_colony_new ( struct _MbsSector *sector,
                            guint owner );
MbsTask *mbs_colony_select_task ( MbsColony *colony,
                                  MbPopType pop_type );
void mbs_colony_adjust_pop ( MbsColony *colony,
                             MbPopType type,
                             guint birthdate,
                             gint64 count );
void mbs_colony_update_pop_tree ( MbsColony *colony );



G_END_DECLS

#endif /* ifndef _MBSCOLONY_H_ */
