/* mbcpriority.h -
 */

#ifndef _MBCPRIORITY_H_
#define _MBCPRIORITY_H_

#include "client/mbcbase.h"
#include "client/mbcpriority-def.h"

G_BEGIN_DECLS



/* MbcPriority:
 */
struct _MbcPriority
{
  MBC_PRIORITY_INSTANCE_HEADER;
};



/* MbcPriorityClass:
 */
struct _MbcPriorityClass
{
  MBC_PRIORITY_CLASS_HEADER;
};



MbObject *mbc_priority_new ( MbObject *game,
                             MbObjectID id,
                             MbPriorityValue value );
void mbc_priority_request_set_value ( MbcPriority *priority,
                                      MbPriorityValue value );



G_END_DECLS

#endif /* ifndef _MBCPRIORITY_H_ */
