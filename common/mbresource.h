/* mbresource.h -
 */

#ifndef _MBRESOURCE_H_
#define _MBRESOURCE_H_

#include "common/mbobject.h"
#include "common/mbresource-def.h"

G_BEGIN_DECLS



/* MbResource:
 */
struct _MbResource
{
  MB_RESOURCE_INSTANCE_HEADER;

  gchar *name;
  MbResourceFlags flags;
  gint index; /* set by MbGame */
};



/* MbResourceClass:
 */
struct _MbResourceClass
{
  MB_RESOURCE_CLASS_HEADER;
};



#define MB_RESOURCE_NAME(rsc) (MB_RESOURCE(rsc)->name)
#define MB_RESOURCE_FLAGS(rsc) (MB_RESOURCE(rsc)->flags)
#define MB_RESOURCE_INDEX(rsc) (MB_RESOURCE(rsc)->index)



G_END_DECLS

#endif /* ifndef _MBRESOURCE_H_ */
