/* mbsresource.h -
 */

#ifndef _MBSRESOURCE_H_
#define _MBSRESOURCE_H_

#include "server/mbsobject.h"
#include "server/mbsresource-def.h"

G_BEGIN_DECLS



/* MbsResource:
 */
struct _MbsResource
{
  MBS_RESOURCE_INSTANCE_HEADER;

  gchar *name;
  MbResourceFlags flags;
};



/* MbsResourceClass:
 */
struct _MbsResourceClass
{
  MBS_RESOURCE_CLASS_HEADER;
};



MbsResource *mbs_resource_new ( const gchar *name,
                                MbResourceFlags flags );



G_END_DECLS

#endif /* ifndef _MBSRESOURCE_H_ */
