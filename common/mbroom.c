/* mbroom.c -
 */

#include "common/private.h"
#include "common/mbroom.h"
#include "common/mbcolony.h"
#include "common/mbgame.h"
#include "common/mbtask.h"
#include "common/mbroom.inl"



/* Properties:
 */
enum
  {
    PROP_0,
    PROP_TYPE,
    N_PROPS,
  };

static LParamSpec *pspecs[N_PROPS];



static void set_property ( LObject *obj,
                           LParamSpec *pspec,
                           LObject *value );
static LObject *get_property ( LObject *obj,
                               LParamSpec *pspec );



/* mb_room_class_init:
 */
static void mb_room_class_init ( LObjectClass *cls )
{
  cls->set_property = set_property;
  cls->get_property = get_property;

  pspecs[PROP_TYPE] =
    l_param_spec_int("type",
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
    case PROP_TYPE:
      MB_ROOM(obj)->type = L_INT_VALUE(value);
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



/* mb_room_set_colony:
 */
void mb_room_set_colony ( MbRoom *room,
                          MbObject *colony )
{
  MbRoomTypeInfo *info;
  ASSERT(MB_IS_COLONY(colony));
  ASSERT(!room->colony);
  room->colony = colony;
  info = mb_game_get_room_type_info(MB_GAME(MB_OBJECT_GAME(room)),
                                    MB_ROOM_TYPE(room));
  ASSERT(info);
  if (info->work_task)
    {
      MbObject *task = mb_task_find(MB_TASK(MB_COLONY_TOP_TASK(colony)),
                                    info->work_task);
      ASSERT(task);
      room->work_task = l_object_ref(task);
    }
}
