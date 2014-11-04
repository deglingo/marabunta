/* mbsobject.h -
 */

#ifndef _MBSOBJECT_H_
#define _MBSOBJECT_H_

#include "server/mbsbase.h"
#include "server/mbsobject-def.h"

G_BEGIN_DECLS



/* MbsObject:
 */
struct _MbsObject
{
  MBS_OBJECT_INSTANCE_HEADER;
};



/* MbsObjectClass:
 */
struct _MbsObjectClass
{
  MBS_OBJECT_CLASS_HEADER;
};



G_END_DECLS

#endif /* ifndef _MBSOBJECT_H_ */
