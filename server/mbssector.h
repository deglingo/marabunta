/* mbssector.h -
 */

#ifndef _MBSSECTOR_H_
#define _MBSSECTOR_H_

#include "server/mbsbase.h"
#include "server/mbscolony.h"
#include "server/mbssector-def.h"

G_BEGIN_DECLS



/* MbsSector:
 */
struct _MbsSector
{
  MBS_SECTOR_INSTANCE_HEADER;

  MbsColony *colony;
};



/* MbsSectorClass:
 */
struct _MbsSectorClass
{
  MBS_SECTOR_CLASS_HEADER;
};



MbsSector *mbs_sector_new ( void );
void mbs_sector_create_colony ( MbsSector *sector,
                                guint player );



G_END_DECLS

#endif /* ifndef _MBSSECTOR_H_ */
