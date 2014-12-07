/* mbtaskfood.c -
 */

#include "mbtaskfood.h"
#include "mbtaskfood.inl"



static void _process ( MbTask *task );



/* mb_task_food_class_init:
 */
static void mb_task_food_class_init ( LObjectClass *cls )
{
  MB_TASK_CLASS(cls)->process = _process;
}



/* mb_task_food_new:
 */
MbTask *mb_task_food_new ( MbTask *parent )
{
  MbTask *task;
  task = mb_task_new(MB_CLASS_TASK_FOOD,
                     parent,
                     "food",
                     MB_POP_ADULT_WORKER);
  return task;

}



/* _process:
 */
static void _process ( MbTask *task )
{
  CL_DEBUG("[TODO] process food: %" G_GINT64_FORMAT, task->workers);
}
