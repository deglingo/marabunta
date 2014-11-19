/* mbscolony.h -
 */

#ifndef _MBSCOLONY_H_
#define _MBSCOLONY_H_

#include "server/mbsbase.h"
#include "server/mbspoptree.h"
#include "server/mbscolony-def.h"

G_BEGIN_DECLS



/* MbsColony:
 */
struct _MbsColony
{
  MBS_COLONY_INSTANCE_HEADER;

  MbsPopTree *pop_tree;
  MbsPopTree *adj_tree;
};



/* MbsColonyClass:
 */
struct _MbsColonyClass
{
  MBS_COLONY_CLASS_HEADER;
};



MbObject *mbs_colony_new ( MbObject *game );
void mbs_colony_update ( MbsColony *colony );



G_END_DECLS

#endif /* ifndef _MBSCOLONY_H_ */
