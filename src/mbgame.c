/* mbgame.c -
 */

#include "mbgame.h"
#include "mbworld.h"
#include "mbsector.h"
#include "mbcolony.h"
#include "mbgame.inl"



#define GAME_FPS ((gdouble) 5.0)



/* Properties:
 */
enum
  {
    PROP_0,
    PROP_SIM_TIME,
    N_PROPS,
  };

static LParamSpec *pspecs[N_PROPS];



/* mb_game_class_init:
 */
static void mb_game_class_init ( LObjectClass *cls )
{
  pspecs[PROP_SIM_TIME] =
    l_param_spec_int("sim_time",
                     0);

  l_object_class_install_properties(cls, N_PROPS, pspecs);
}



/* mb_game_init:
 */
static void mb_game_init ( LObject *obj )
{
  MB_GAME(obj)->timer = g_timer_new();
  MB_GAME(obj)->room_types = g_ptr_array_new();
  g_ptr_array_add(MB_GAME(obj)->room_types, NULL);
  MB_GAME(obj)->room_types_map =
    g_hash_table_new(g_str_hash,
                     g_str_equal);
}



/* mb_game_new:
 */
MbGame *mb_game_new ( void )
{
  return MB_GAME(l_object_new(MB_CLASS_GAME, NULL));
}



/* mb_game_register_room_type:
 */
MbRoomType mb_game_register_room_type ( MbGame *game,
                                        const gchar *nick,
                                        const gchar *name,
                                        gdouble x,
                                        gdouble y,
                                        gdouble width,
                                        gdouble height,
                                        const gchar *work_task )
{
  MbRoomTypeInfo *info;
  ASSERT(!mb_game_lookup_room_type(game, nick));
  info = g_new0(MbRoomTypeInfo, 1);
  info->type = game->room_types->len;
  info->nick = g_strdup(nick);
  info->name = g_strdup(name);
  info->x = x;
  info->y = y;
  info->width = width;
  info->height = height;
  info->work_task = g_strdup(work_task);
  g_ptr_array_add(game->room_types, info);
  g_hash_table_insert(game->room_types_map,
                      info->nick,
                      info);
  return info->type;
}



/* mb_game_lookup_room_type:
 */
MbRoomType mb_game_lookup_room_type ( MbGame *game,
                                      const gchar *nick )
{
  MbRoomTypeInfo *info;
  if ((info = g_hash_table_lookup(game->room_types_map, nick)))
    return info->type;
  else
    return 0;
}



/* mb_game_setup:
 */
void mb_game_setup ( MbGame *game )
{
  MbColony *col;
  ASSERT(!game->world); /* [todo] */
  mb_game_register_room_type(game,
                             "royal-chamber",
                             "Royal chamber",
                             0.5, 0.5, 0.1, 0.1,
                             "spawn");
  game->world = mb_world_new(3, 2);
  game->world->game = game; /* [fixme] ref ? */
  col = mb_colony_new();
  mb_sector_set_colony(MB_WORLD_SECTOR(game->world, 0, 0), col);
  l_object_unref(col);
}



/* _update:
 */
static void _update ( MbGame *game )
{
  /* CL_DEBUG("update"); */
  game->sim_time++;
  l_object_notify(L_OBJECT(game), pspecs[PROP_SIM_TIME]);
  mb_world_update(game->world);
}



/* _game_timer:
 */
static gboolean _game_timer ( MbGame *game )
{
  gdouble elapsed = g_timer_elapsed(game->timer, NULL);
  while (elapsed >= game->next_frame)
    {
      _update(game);
      game->next_frame = ((gdouble) game->sim_time) / GAME_FPS;
    }
  return G_SOURCE_CONTINUE;
}



/* mb_game_start:
 */
void mb_game_start ( MbGame *game )
{
  g_timeout_add_full(MB_PRIORITY_GAME_TIMER,
                     10,
                     (GSourceFunc) _game_timer,
                     game,
                     NULL);
  g_timer_start(game->timer);
}
