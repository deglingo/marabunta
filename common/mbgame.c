/* mbgame.c -
 */

#include "common/private.h"
#include "common/mbgame.h"
#include "common/mbplayer.h"
#include "common/mbworld.h"
#include "common/mbstate.h"
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
