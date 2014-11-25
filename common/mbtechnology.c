/* mbtechnology.c -
 */

#include "common/private.h"
#include "common/mbtechnology.h"
#include "common/mbtechnology.inl"



/* Properties:
 */
enum
  {
    PROP_0,
    PROP_NAME,
    N_PROPS,
  };

static LParamSpec *pspecs[N_PROPS];



static void set_property ( LObject *obj,
                           LParamSpec *pspec,
                           LObject *value );
static LObject *get_property ( LObject *obj,
                               LParamSpec *pspec );



/* mb_technology_class_init:
 */
static void mb_technology_class_init ( LObjectClass *cls )
{
  cls->set_property = set_property;
  cls->get_property = get_property;

  pspecs[PROP_NAME] =
    l_param_spec_string("name",
                        "");

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
      ASSERT(!MB_TECHNOLOGY_NAME(obj));
      MB_TECHNOLOGY(obj)->name = g_strdup(L_STRING(value)->str);
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
