/* mbpriority.c -
 */

#include "mbpriority.h"
#include "mbpriority.inl"



/* Properties:
 */
enum
  {
    PROP_0,
    PROP_VALUE,
    N_PROPS,
  };

static LParamSpec *pspecs[N_PROPS];



/* mb_priority_class_init:
 */
static void mb_priority_class_init ( LObjectClass *cls )
{
  pspecs[PROP_VALUE] =
    l_param_spec_int("value",
                     0);

  l_object_class_install_properties(cls, N_PROPS, pspecs);
}



/* mb_priority_new:
 */
MbPriority *mb_priority_new ( MbPriorityValue value )
{
  MbPriority *prio;
  prio = MB_PRIORITY(l_object_new(MB_CLASS_PRIORITY, NULL));
  prio->value = value;
  return prio;
}



/* mb_priority_score:
 */
gint64 mb_priority_score ( MbPriority *priority )
{
  return priority->score;
}



/* mb_priority_next_score:
 */
gint64 mb_priority_next_score ( MbPriority *priority,
                                gint64 workers )
{
  return (workers + priority->remainder) / priority->value;
}



/* mb_priority_update_score:
 */
gint64 mb_priority_update_score ( MbPriority *priority,
                                  gint64 workers )
{
  priority->score += (workers + priority->remainder) / priority->value;
  priority->remainder = (workers + priority->remainder) % priority->value;
  return priority->score;
}



/* mb_priority_adjust_score:
 */
void mb_priority_adjust_score ( MbPriority *priority,
                                gint64 adj )
{
  priority->score += adj;
}



/* mb_priority_request_set_value:
 */
void mb_priority_request_set_value ( MbPriority *priority,
                                     MbPriorityValue value )
{
  /* [FIXME] */
  value = CLAMP(value, 1, 99);
  if (value == priority->value)
    return;
  priority->value = value;
  l_object_notify(L_OBJECT(priority), pspecs[PROP_VALUE]);
}
