/* mbgame.c -
 */

#include "mbgame.h"
#include "mbworld.h"
#include "mbsector.h"
#include "mbcolony.h"
#include "mbgame.inl"



#define GAME_FPS ((gdouble) 5.0)



/* Private:
 */
typedef struct _Private
{
  MbWorld *world;
  GTimer *timer;
  guint sim_time;
  gdouble next_frame;
}
  Private;

#define PRIVATE(game) ((Private *)(MB_GAME(game)->private))



/* mb_game_init:
 */
static void mb_game_init ( LObject *obj )
{
  MB_GAME(obj)->private = g_new0(Private, 1);
  PRIVATE(obj)->timer = g_timer_new();
}



/* mb_game_new:
 */
MbGame *mb_game_new ( void )
{
  return MB_GAME(l_object_new(MB_CLASS_GAME, NULL));
}



/* mb_game_setup:
 */
void mb_game_setup ( MbGame *game )
{
  Private *priv = PRIVATE(game);
  MbColony *col;
  ASSERT(!priv->world); /* [todo] */
  priv->world = mb_world_new(3, 2);
  col = mb_colony_new();
  mb_sector_set_colony(MB_WORLD_SECTOR(priv->world, 0, 0), col);
  l_object_unref(col);
}



/* _update:
 */
static void _update ( MbGame *game )
{
  Private *priv = PRIVATE(game);
  CL_DEBUG("update");
  priv->sim_time++;
}



/* _game_timer:
 */
static gboolean _game_timer ( MbGame *game )
{
  Private *priv = PRIVATE(game);
  gdouble elapsed = g_timer_elapsed(priv->timer, NULL);
  while (elapsed >= priv->next_frame)
    {
      _update(game);
      priv->next_frame = ((gdouble) priv->sim_time) / GAME_FPS;
    }
  return G_SOURCE_CONTINUE;
}



/* mb_game_start:
 */
void mb_game_start ( MbGame *game )
{
  Private *priv = PRIVATE(game);
  g_timeout_add_full(MB_PRIORITY_GAME_TIMER,
                     10,
                     (GSourceFunc) _game_timer,
                     game,
                     NULL);
  g_timer_start(priv->timer);
}
