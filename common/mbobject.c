/* mbobject.c -
 */

#include "common/private.h"
#include "common/mbobject.h"
#include "common/mbgame.h"
#include "common/mbobject.inl"



/* Properties:
 */
enum
  {
    PROP_0,
    PROP_ID,
    PROP_GAME,
    N_PROPS,
  };

static LParamSpec *pspecs[N_PROPS];



static volatile guint id_counter = 1;

static void set_property ( LObject *obj,
                           LParamSpec *pspec,
                           LObject *value );
static LObject *get_property ( LObject *obj,
                               LParamSpec *pspec );



/* mb_object_class_init:
 */
static void mb_object_class_init ( LObjectClass *cls )
{
  cls->set_property = set_property;
  cls->get_property = get_property;

  pspecs[PROP_ID] =
    l_param_spec_int("id",
                     0);
  
  pspecs[PROP_GAME] =
    l_param_spec_object("game",
                        cls /* [FIXME] MB_CLASS_GAME */);

  l_object_class_install_properties(cls, N_PROPS, pspecs);
}



/* mb_object_init:
 */
static void mb_object_init ( LObject *obj )
{
  /* [FIXME] should be a construct property */
  MB_OBJECT(obj)->id = g_atomic_int_add(&id_counter, 1);
}



/* set_property:
 */
static void set_property ( LObject *obj,
                           LParamSpec *pspec,
                           LObject *value )
{
  switch (pspec->param_id)
    {
    case PROP_ID:
      /* [FIXME] use the constructor for this */
      ASSERT(!MB_OBJECT(obj)->game);
      MB_OBJECT(obj)->id = L_INT_VALUE(value);
      break;
    case PROP_GAME:
      ASSERT(MB_IS_GAME(value));
      ASSERT(!MB_OBJECT(obj)->game);
      MB_OBJECT(obj)->game = l_object_ref(value);
      mb_game_register_object(MB_GAME(value), MB_OBJECT(obj));
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
