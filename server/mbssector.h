/* mbssector.h -
 */

#ifndef _MBSSECTOR_H_
#define _MBSSECTOR_H_

#include "server/mbsbase.h"
#include "server/mbssector-def.h"

G_BEGIN_DECLS



/* MbsSector:
 */
struct _MbsSector
{
  MBS_SECTOR_INSTANCE_HEADER;
};



/* MbsSectorClass:
 */
struct _MbsSectorClass
{
  MBS_SECTOR_CLASS_HEADER;
};



MbObject *mbs_sector_new ( MbObject *game );
void mbs_sector_update ( MbsSector *sector );



G_END_DECLS

#endif /* ifndef _MBSSECTOR_H_ */
