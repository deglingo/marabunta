/* mbscontrol.h -
 */

#ifndef _MBSCONTROL_H_
#define _MBSCONTROL_H_

#include "server/mbsobject.h"
#include "server/mbscontrol-def.h"

G_BEGIN_DECLS



/* MbsControl:
 */
struct _MbsControl
{
  MBS_CONTROL_INSTANCE_HEADER;
};



/* MbsControlClass:
 */
struct _MbsControlClass
{
  MBS_CONTROL_CLASS_HEADER;
};



G_END_DECLS

#endif /* ifndef _MBSCONTROL_H_ */
