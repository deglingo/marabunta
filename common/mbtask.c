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
    PROP_ISGROUP,
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
  
  pspecs[PROP_ISGROUP] =
    /* [fixme] bool */
    l_param_spec_int("isgroup",
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
    case PROP_ISGROUP:
      MB_TASK(obj)->isgroup = L_INT_VALUE(value) ? TRUE : FALSE;
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
  ASSERT(task->isgroup);
  ASSERT(!MB_TASK(child)->parent);
  ASSERT(!MB_TASK_COLONY(child));
  ASSERT(MB_TASK_COLONY(task));
  l_object_ref(child);
  task->children = g_list_append(task->children, child);
  MB_TASK(child)->parent = task;
  MB_TASK(child)->colony = MB_TASK_COLONY(task);
}



/* mb_task_get_child:
 */
MbObject *mb_task_get_child ( MbTask *task,
                              const gchar *name )
{
  GList *l;
  ASSERT(task->isgroup);
  for (l = task->children; l; l = l->next)
    {
      if (!strcmp(MB_TASK_NAME(l->data), name))
        return l->data;
    }
  return NULL;
}



/* mb_task_find:
 */
MbObject *mb_task_find ( MbTask *task,
                         const gchar *path )
{
  const gchar *p = path;
  gchar comp[MB_TASK_MAX_NAME+1];
  MbObject *t;
  ASSERT(task);
  t = MB_OBJECT(task);
  while ((mb_task_path_next(&p, comp)))
    {
      if (!(t = mb_task_get_child(MB_TASK(t), comp)))
        return NULL;
    }
  return t;
}
