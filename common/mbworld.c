/* mbworld.c -
 */

#include "common/private.h"
#include "common/mbworld.h"
#include "common/mbsector.h"
#include "common/mbworld.inl"



/* Properties:
 */
enum
  {
    PROP_0,
    PROP_WIDTH,
    PROP_HEIGHT,
    N_PROPS,
  };

static LParamSpec *pspecs[N_PROPS];



static void set_property ( LObject *obj,
                           LParamSpec *pspec,
                           LObject *value );
static LObject *get_property ( LObject *obj,
                               LParamSpec *pspec );



/* mb_world_class_init:
 */
static void mb_world_class_init ( LObjectClass *cls )
{
  cls->set_property = set_property;
  cls->get_property = get_property;

  pspecs[PROP_WIDTH] =
    l_param_spec_int("width",
                     0);

  pspecs[PROP_HEIGHT] =
    l_param_spec_int("height",
                     0);

  l_object_class_install_properties(cls, N_PROPS, pspecs);
}



/* mb_world_new:
 */
MbObject *mb_world_new ( MbObject *game,
                         MbObjectID id )
{
  CL_ERROR("[TODO]");
  return NULL;
  /* MbObject *world; */
  /* world = mb_object_new(MB_CLASS_WORLD, game, id); */
  /* return world; */
}



/* _init_sectors:
 */
static void _init_sectors ( MbWorld *world )
{
  guint y;
  if (world->width == 0 || world->height == 0)
    return;
  ASSERT(!world->sectors);
  CL_DEBUG("init world: %dx%d", world->width, world->height);
  world->sectors = g_new(MbObject **, world->height);
  for (y = 0; y < world->height; y++)
    world->sectors[y] = g_new0(MbObject *, world->width);
}



/* set_property:
 */
static void set_property ( LObject *obj,
                           LParamSpec *pspec,
                           LObject *value )
{
  switch (pspec->param_id)
    {
    case PROP_WIDTH:
      ASSERT(MB_WORLD(obj)->width == 0);
      MB_WORLD(obj)->width = L_INT_VALUE(value);
      _init_sectors(MB_WORLD(obj));
      break;
    case PROP_HEIGHT:
      ASSERT(MB_WORLD(obj)->height == 0);
      MB_WORLD(obj)->height = L_INT_VALUE(value);
      _init_sectors(MB_WORLD(obj));
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



/* mb_world_add_sector:
 */
void mb_world_add_sector ( MbWorld *world,
                           MbObject *sector,
                           guint x,
                           guint y )
{
  ASSERT(MB_IS_SECTOR(sector));
  ASSERT(x < world->width);
  ASSERT(y < world->height);
  ASSERT(!world->sectors[y][x]);
  ASSERT(!MB_SECTOR(sector)->world);
  world->sectors[y][x] = l_object_ref(sector);
  MB_SECTOR(sector)->world = MB_OBJECT(world); /* [fixme] weakref ? */
  MB_SECTOR(sector)->x = x;
  MB_SECTOR(sector)->y = y;
}
