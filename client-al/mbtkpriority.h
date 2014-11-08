/* mbtkpriority.h -
 */

#ifndef _MBTKPRIORITY_H_
#define _MBTKPRIORITY_H_

#include "client-al/mbtkbase.h"
#include "client-al/mbtkpriority-def.h"

G_BEGIN_DECLS



/* MbtkPriority:
 */
struct _MbtkPriority
{
  MBTK_PRIORITY_INSTANCE_HEADER;

  MbcProxy *proxy;
  AltkWidget *label;
};



/* MbtkPriorityClass:
 */
struct _MbtkPriorityClass
{
  MBTK_PRIORITY_CLASS_HEADER;
};



AltkWidget *mbtk_priority_new ( MbcProxy *priority );



G_END_DECLS

#endif /* ifndef _MBTKPRIORITY_H_ */
