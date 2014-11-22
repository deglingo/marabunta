/* mbvein.c -
 */

#include "common/private.h"
#include "common/mbvein.h"
#include "common/mbresource.h"
#include "common/mbvein.inl"



/* Properties:
 */
enum
  {
    PROP_0,
    PROP_RESOURCE,
    PROP_DEPTH,
    PROP_QTTY,
    N_PROPS,
  };

static LParamSpec *pspecs[N_PROPS];



static void set_property ( LObject *obj,
                           LParamSpec *pspec,
                           LObject *value );
static LObject *get_property ( LObject *obj,
                               LParamSpec *pspec );



/* mb_vein_class_init:
 */
static void mb_vein_class_init ( LObjectClass *cls )
{
  cls->set_property = set_property;
  cls->get_property = get_property;

  pspecs[PROP_RESOURCE] =
    l_param_spec_object("resource",
                        MB_CLASS_RESOURCE);
  
  pspecs[PROP_DEPTH] =
    l_param_spec_int("depth",
                     0);
  
  pspecs[PROP_QTTY] =
    /* [FIXME] int64 !! */
    l_param_spec_int("qtty",
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
    case PROP_RESOURCE:
      ASSERT(!MB_VEIN(obj)->resource);
      MB_VEIN(obj)->resource = l_object_ref(value);
      break;
    case PROP_DEPTH:
      MB_VEIN(obj)->depth = L_INT_VALUE(value);
      break;
    case PROP_QTTY:
      MB_VEIN(obj)->qtty = L_INT_VALUE(value);
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
