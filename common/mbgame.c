/* mbgame.c -
 */

#include "common/private.h"
#include "common/mbgame.h"
#include "common/mbplayer.h"
#include "common/mbresource.h"
#include "common/mbworld.h"
#include "common/mbstate.h"
#include "common/mbtechnology.h"
#include "common/mbgame.inl"



/* Properties:
 */
enum
  {
    PROP_0,
    PROP_FRAME_COUNT,
    N_PROPS,
  };

static LParamSpec *pspecs[N_PROPS];



/* Signals:
 */
enum
  {
    SIG_STARTED,
    SIG_COUNT,
  };

static LSignalHandler signals[SIG_COUNT];



static void add_player ( MbGame *game,
                         MbObject *player );



/* mb_game_class_init:
 */
static void mb_game_class_init ( LObjectClass *cls )
{
  /* [TODO] */
  /* cls->set_property = set_property; */
  /* cls->get_property = get_property; */

  MB_GAME_CLASS(cls)->add_player = add_player;
  
  pspecs[PROP_FRAME_COUNT] =
    l_param_spec_int("frame_count",
                     0);

  l_object_class_install_properties(cls, N_PROPS, pspecs);

  signals[SIG_STARTED] =
    l_signal_new(cls,
                 "started");
}



/* mb_game_init:
 */
static void mb_game_init ( LObject *obj )
{
  MB_GAME(obj)->object_map =
    g_hash_table_new_full(NULL,
                          NULL,
                          NULL,
                          (GDestroyNotify) l_object_unref);
  MB_GAME(obj)->resources =
    g_hash_table_new_full(g_str_hash,
                          g_str_equal,
                          NULL,
                          (GDestroyNotify) l_object_unref);
  MB_GAME(obj)->technologies =
    g_hash_table_new_full(g_str_hash,
                          g_str_equal,
                          NULL,
                          (GDestroyNotify) l_object_unref);
  MB_GAME(obj)->room_types = g_ptr_array_new();
  g_ptr_array_add(MB_GAME(obj)->room_types, NULL);
}



/* mb_game_started:
 */
void mb_game_started ( MbGame *game )
{
  l_signal_emit(L_OBJECT(game),
                signals[SIG_STARTED],
                0);
}



/* mb_game_register_object:
 */
void mb_game_register_object ( MbGame *game,
                               MbObject *object )
{
  ASSERT(object);
  ASSERT(object->game == MB_OBJECT(game));
  ASSERT(object->id != 0);
  ASSERT(!mb_game_lookup_object(game, MB_OBJECT_ID(object)));
  g_hash_table_insert(game->object_map,
                      GUINT_TO_POINTER(MB_OBJECT_ID(object)),
                      l_object_ref(object));
}



/* mb_game_lookup_object:
 */
MbObject *mb_game_lookup_object ( MbGame *game,
                                  MbObjectID id )
{
  return g_hash_table_lookup(game->object_map,
                             GUINT_TO_POINTER(id));
}



/* mb_game_add_player:
 */
void mb_game_add_player ( MbGame *game,
                          MbObject *player )
{
  ASSERT(MB_OBJECT_GAME(player) == MB_OBJECT(game));
  ASSERT(MB_IS_PLAYER(player));
  MB_GAME_GET_CLASS(game)->add_player(game, player);
}



/* add_player:
 */
static void add_player ( MbGame *game,
                         MbObject *player )
{
  /* note: game already owns the ref !? */
  game->players = g_list_append(game->players, player);
}



/* mb_game_register_resource:
 */
void mb_game_register_resource ( MbGame *game,
                                 MbObject *resource )
{
  ASSERT(MB_IS_RESOURCE(resource));
  ASSERT(MB_OBJECT_GAME(resource) == MB_OBJECT(game));
  ASSERT(!mb_game_lookup_resource(game, MB_RESOURCE_NAME(resource)));
  l_object_ref(resource);
  g_hash_table_insert(game->resources,
                      MB_RESOURCE_NAME(resource),
                      resource);
}



/* mb_game_lookup_resource:
 */
MbObject *mb_game_lookup_resource ( MbGame *game,
                                    const gchar *name )
{
  return g_hash_table_lookup(game->resources, name);
}



/* mb_game_register_technology:
 */
void mb_game_register_technology ( MbGame *game,
                                   MbObject *technology )
{
  ASSERT(MB_IS_TECHNOLOGY(technology));
  ASSERT(MB_OBJECT_GAME(technology) == MB_OBJECT(game));
  ASSERT(!mb_game_lookup_technology(game, MB_TECHNOLOGY_NAME(technology)));
  g_hash_table_insert(game->technologies,
                      MB_TECHNOLOGY_NAME(technology),
                      l_object_ref(technology));
}



/* mb_game_lookup_technology:
 */
MbObject *mb_game_lookup_technology ( MbGame *game,
                                      const gchar *name )
{
  return g_hash_table_lookup(game->technologies, name);
}



/* mb_game_next_room_type:
 */
MbRoomType mb_game_next_room_type ( MbGame *game )
{
  return game->room_types->len;
}



/* mb_game_register_room_type:
 */
void mb_game_register_room_type ( MbGame *game,
                                  MbRoomType type,
                                  const gchar *nick,
                                  const gchar *name )
{
  MbRoomTypeInfo *info;
  ASSERT(type > 0 && !mb_game_get_room_type_info(game, type));
  if (type >= game->room_types->len)
    g_ptr_array_set_size(game->room_types, type+1);
  info = g_new0(MbRoomTypeInfo, 1);
  info->type = type;
  info->nick = g_strdup(nick);
  info->name = g_strdup(name);
  game->room_types->pdata[type] = info;
}



/* mb_game_get_room_type_info:
 */
MbRoomTypeInfo *mb_game_get_room_type_info ( MbGame *game,
                                             MbRoomType type )
{
  if (type > 0 && type < game->room_types->len)
    return game->room_types->pdata[type];
  else
    return NULL;
}



/* mb_game_set_world:
 */
void mb_game_set_world ( MbGame *game,
                         MbObject *world )
{
  ASSERT(MB_OBJECT_GAME(world) == MB_OBJECT(game));
  ASSERT(MB_IS_WORLD(world));
  ASSERT(!game->world);
  /* note: game already owns the ref !? */
  game->world = world;
}



/* mb_game_set_frame_count:
 */
void mb_game_set_frame_count ( MbGame *game,
                               guint frame_count )
{
  if (frame_count != game->frame_count)
    {
      game->frame_count = frame_count;
      l_object_notify(L_OBJECT(game),
                      pspecs[PROP_FRAME_COUNT]);
    }
}
