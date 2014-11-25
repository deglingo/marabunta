/* mbtechnology.h -
 */

#ifndef _MBTECHNOLOGY_H_
#define _MBTECHNOLOGY_H_

#include "common/mbobject.h"
#include "common/mbtechnology-def.h"

G_BEGIN_DECLS



/* MbTechnology:
 */
struct _MbTechnology
{
  MB_TECHNOLOGY_INSTANCE_HEADER;

  gchar *name;
};



/* MbTechnologyClass:
 */
struct _MbTechnologyClass
{
  MB_TECHNOLOGY_CLASS_HEADER;
};



#define MB_TECHNOLOGY_NAME(tech) (MB_TECHNOLOGY(tech)->name)



G_END_DECLS

#endif /* ifndef _MBTECHNOLOGY_H_ */
