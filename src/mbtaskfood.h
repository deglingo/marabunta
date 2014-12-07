/* mbtaskfood.h -
 */

#ifndef _MBTASKFOOD_H_
#define _MBTASKFOOD_H_

#include "mbtask.h"
#include "mbtaskfood-def.h"

G_BEGIN_DECLS



/* MbTaskFood:
 */
struct _MbTaskFood
{
  MB_TASK_FOOD_INSTANCE_HEADER;
};



/* MbTaskFoodClass:
 */
struct _MbTaskFoodClass
{
  MB_TASK_FOOD_CLASS_HEADER;
};



MbTask *mb_task_food_new ( MbTask *parent );



G_END_DECLS

#endif /* ifndef _MBTASKFOOD_H_ */
