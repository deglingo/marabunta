/* mbapp.h -
 */

#ifndef _MBAPP_H_
#define _MBAPP_H_

#include "common/mbbase.h"
#include "common/mbapp-def.h"

G_BEGIN_DECLS



/* MbApp:
 */
struct _MbApp
{
  MB_APP_INSTANCE_HEADER;
};



/* MbAppClass:
 */
struct _MbAppClass
{
  MB_APP_CLASS_HEADER;
};



G_END_DECLS

#endif /* ifndef _MBAPP_H_ */
