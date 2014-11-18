/* mbgame.c -
 */

#include "common/private.h"
#include "common/mbgame.h"
#include "common/mbplayer.h"
#include "common/mbworld.h"
#include "common/mbgame.inl"



/* mb_game_init:
 */
static void mb_game_init ( LObject *obj )
{
  MB_GAME(obj)->id_counter = 1;
  MB_GAME(obj)->object_map =
    g_hash_table_new_full(NULL,
                          NULL,
                          NULL,
                          (GDestroyNotify) l_object_unref);
}



/* mb_game_new:
 */
MbObject *mb_game_new ( guint id )
{
  MbObject *game;
  game = mb_object_new(MB_CLASS_GAME, NULL, id);
  return game;
}



/* mb_game_next_id:
 */
MbObjectID mb_game_next_id ( MbGame *game )
{
  return g_atomic_int_add(&game->id_counter, 1);
}



/* mb_game_register_object:
 */
void mb_game_register_object ( MbGame *game,
                               MbObject *object )
{
  ASSERT(object);
  ASSERT(!object->game);
  ASSERT(object->id != 0);
  ASSERT(!mb_game_lookup_object(game, MB_OBJECT_ID(object)));
  object->game = MB_OBJECT(game);
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
MbObject *mb_game_add_player ( MbGame *game,
                               guint id,
                               const gchar *name )
{
  MbObject *player;
  player = mb_player_new(MB_OBJECT(game), id, name);
  game->players = g_list_append(game->players, player);
  return player;
}



/* mb_game_setup:
 */
void mb_game_setup ( MbGame *game )
{
  ASSERT(!game->world);
  game->world = mb_world_new(MB_OBJECT(game), 0);
}
