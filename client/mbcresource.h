/* mbcresource.h -
 */

#ifndef _MBCRESOURCE_H_
#define _MBCRESOURCE_H_

#include "client/mbcbase.h"
#include "client/mbcresource-def.h"

G_BEGIN_DECLS



/* MbcResource:
 */
struct _MbcResource
{
  MBC_RESOURCE_INSTANCE_HEADER;
};



/* MbcResourceClass:
 */
struct _MbcResourceClass
{
  MBC_RESOURCE_CLASS_HEADER;
};



MbObject *mbc_resource_new ( MbObject *game,
                             MbObjectID id,
                             const gchar *name,
                             MbResourceFlags flags );



G_END_DECLS

#endif /* ifndef _MBCRESOURCE_H_ */
