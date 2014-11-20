/* mbtask.c -
 */

#include "common/private.h"
#include "common/mbtask.h"
#include "common/mbpriority.h"
#include "common/mbtask.inl"



/* Properties:
 */
enum
  {
    PROP_0,
    PROP_NAME,
    PROP_ISGROUP,
    PROP_PRIORITY,
    N_PROPS,
  };

static LParamSpec *pspecs[N_PROPS];



static void set_property ( LObject *obj,
                           LParamSpec *pspec,
                           LObject *value );
static LObject *get_property ( LObject *obj,
                               LParamSpec *pspec );
static void add ( MbTask *task,
                  MbObject *child );
static void add_workers ( MbTask *task,
                          gint64 workers );



/* mb_task_class_init:
 */
static void mb_task_class_init ( LObjectClass *cls )
{
  cls->set_property = set_property;
  cls->get_property = get_property;
  
  MB_TASK_CLASS(cls)->add = add;
  MB_TASK_CLASS(cls)->add_workers = add_workers;
  
  pspecs[PROP_NAME] =
    l_param_spec_string("name",
                        "");
  
  pspecs[PROP_ISGROUP] =
    /* [fixme] bool */
    l_param_spec_int("isgroup",
                     0);
  
  pspecs[PROP_PRIORITY] =
    l_param_spec_object("priority",
                        MB_CLASS_PRIORITY);
  
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
    case PROP_PRIORITY:
      ASSERT(!MB_TASK_PRIORITY(obj));
      MB_TASK(obj)->priority = l_object_ref(value);
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
  MB_TASK_GET_CLASS(task)->add(task, child);
}



/* add:
 */
static void add ( MbTask *task,
                  MbObject *child )
{
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



/* mb_task_add_workers:
 */
void mb_task_add_workers ( MbTask *task,
                           gint64 workers )
{
  ASSERT(!MB_TASK_ISGROUP(task));
  MB_TASK_GET_CLASS(task)->add_workers(task, workers);
}



/* add_workers:
 */
static void add_workers ( MbTask *task,
                          gint64 workers )
{
  task->workers += workers;
  ASSERT(task->workers >= 0); /* ?? */
  if (MB_TASK_PARENT(task))
    add_workers(MB_TASK_PARENT(task), workers);
}
