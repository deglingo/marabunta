/* mbtask.c -
 */

#include "common/private.h"
#include "common/mbtask.h"
#include "common/mbtask.inl"



/* Properties:
 */
enum
  {
    PROP_0,
    PROP_NAME,
    PROP_GROUP,
    PROP_POP_FLAGS,
    N_PROPS,
  };

static LParamSpec *pspecs[N_PROPS];



static void set_property ( LObject *obj,
                           LParamSpec *pspec,
                           LObject *value );
static LObject *get_property ( LObject *obj,
                               LParamSpec *pspec );



/* mb_task_class_init:
 */
static void mb_task_class_init ( LObjectClass *cls )
{
  cls->set_property = set_property;
  cls->get_property = get_property;

  pspecs[PROP_NAME] =
    l_param_spec_string("name",
                        "");
  
  pspecs[PROP_GROUP] =
    /* [fixme] bool */
    l_param_spec_int("group",
                     0);
  
  pspecs[PROP_POP_FLAGS] =
    l_param_spec_int("pop_flags",
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
    case PROP_NAME:
      ASSERT(!MB_TASK(obj)->name);
      MB_TASK(obj)->name = g_strdup(L_STRING(value)->str);
      break;
    case PROP_GROUP:
      MB_TASK(obj)->group = L_INT_VALUE(value) ? TRUE : FALSE;
      break;
    case PROP_POP_FLAGS:
      MB_TASK(obj)->pop_flags = L_INT_VALUE(value);
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



/* mb_task_add_workers:
 */
void mb_task_add_workers ( MbTask *task,
                           gint64 count )
{
  CL_ERROR("[TODO]");
}



/* mb_task_add:
 */
void mb_task_add ( MbTask *task,
                   MbObject *child )
{
  ASSERT(MB_IS_TASK(child));
  ASSERT(task->group);
  ASSERT(!MB_TASK(child)->parent);
  l_object_ref(child);
  task->children = g_list_append(task->children, child);
  MB_TASK(child)->parent = task;
}
