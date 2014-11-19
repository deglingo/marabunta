/* mbpriority.c -
 */

#include "common/private.h"
#include "common/mbpriority.h"
#include "common/mbpriority.inl"



/* Properties:
 */
enum
  {
    PROP_0,
    PROP_VALUE,
    N_PROPS,
  };

static LParamSpec *pspecs[N_PROPS];



static void set_property ( LObject *obj,
                           LParamSpec *pspec,
                           LObject *value );
static LObject *get_property ( LObject *obj,
                               LParamSpec *pspec );



/* mb_priority_class_init:
 */
static void mb_priority_class_init ( LObjectClass *cls )
{
  cls->set_property = set_property;
  cls->get_property = get_property;

  pspecs[PROP_VALUE] =
    l_param_spec_int("value",
                     0);

  l_object_class_install_properties(cls, N_PROPS, pspecs);
}



/* set_property:
 */
static void set_property ( LObject *obj,
                           LParamSpec *pspec,
                           LObject *value )
{
  switch (pspec->param_id)
    {
    case PROP_VALUE:
      mb_priority_set_value(MB_PRIORITY(obj), L_INT_VALUE(value));
      break;
    default:
      L_OBJECT_SET_PROPERTY_ERROR(obj, pspec);
    }
}



/* get_property:
 */
static LObject *get_property ( LObject *obj,
                               LParamSpec *pspec )
{
  L_OBJECT_GET_PROPERTY_ERROR(obj, pspec);
}



/* mb_priority_set_value:
 */
void mb_priority_set_value ( MbPriority *priority,
                             MbPriorityValue value )
{
  /* [fixme] check bounds */
  if (value != priority->value)
    {
      priority->value = value;
      l_object_notify(L_OBJECT(priority), pspecs[PROP_VALUE]);
    }
}



/* mb_priority_adjust_score:
 */
void mb_priority_adjust_score ( MbPriority *priority,
                                gint64 workers )
{
  /* score <- score + workers / priority.value */
  priority->score.score += (workers + priority->score.remainder) / priority->value;
  priority->score.remainder = (workers + priority->score.remainder) % priority->value;
}
