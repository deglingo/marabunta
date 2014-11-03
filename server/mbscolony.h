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
  GList *tasks;
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



G_END_DECLS

#endif /* ifndef _MBSCOLONY_H_ */
