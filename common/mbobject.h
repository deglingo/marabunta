/* mbobject.h -
 */

#ifndef _MBOBJECT_H_
#define _MBOBJECT_H_

#include "common/mbbase.h"
#include "common/mbobject-def.h"

G_BEGIN_DECLS



typedef guint MbObjectID;



/* MbObject:
 */
struct _MbObject
{
  MB_OBJECT_INSTANCE_HEADER;

  MbObjectID id;
  MbObject *game;
  gboolean client;
};



/* MbObjectClass:
 */
struct _MbObjectClass
{
  MB_OBJECT_CLASS_HEADER;
};



#define MB_OBJECT_ID(obj) (MB_OBJECT(obj)->id)

MbObject *mb_object_new ( LObjectClass *cls,
                          MbObject *game,
                          MbObjectID id );



G_END_DECLS

#endif /* ifndef _MBOBJECT_H_ */
