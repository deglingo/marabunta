/* mbstechnology.h -
 */

#ifndef _MBSTECHNOLOGY_H_
#define _MBSTECHNOLOGY_H_

#include "server/mbsbase.h"
#include "server/mbstechnology-def.h"

G_BEGIN_DECLS



/* MbsTechnology:
 */
struct _MbsTechnology
{
  MBS_TECHNOLOGY_INSTANCE_HEADER;
};



/* MbsTechnologyClass:
 */
struct _MbsTechnologyClass
{
  MBS_TECHNOLOGY_CLASS_HEADER;
};



MbObject *mbs_technology_new ( MbObject *game,
                               const gchar *name );



G_END_DECLS

#endif /* ifndef _MBSTECHNOLOGY_H_ */
