/* mbsvein.h -
 */

#ifndef _MBSVEIN_H_
#define _MBSVEIN_H_

#include "server/mbsbase.h"
#include "server/mbsvein-def.h"

G_BEGIN_DECLS



/* MbsVein:
 */
struct _MbsVein
{
  MBS_VEIN_INSTANCE_HEADER;
};



/* MbsVeinClass:
 */
struct _MbsVeinClass
{
  MBS_VEIN_CLASS_HEADER;
};



MbObject *mbs_vein_new ( MbObject *game,
                         MbObject *resource,
                         gint depth,
                         gint64 qtty );



G_END_DECLS

#endif /* ifndef _MBSVEIN_H_ */
